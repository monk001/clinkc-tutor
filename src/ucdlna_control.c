// Copyright 2015 zhiyong.any@gmail.com

#include <ucdlna/ucdlna_control.h>

#include <string.h>

#include <cybergarage/upnp/ccontrolpoint.h>
#include <cybergarage/upnp/control/ccontrol.h>
#include <cybergarage/upnp/ssdp/cssdp_server.h>

#include <cybergarage/util/ccond.h>
#include <cybergarage/util/cmutex.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/cthread.h>
#include <cybergarage/util/ctime.h>

#include "ucdlna_log.h"

static const char LOG_TAG[] = "ucdlna.control";

static char PLAYER_TYPE[] = "urn:schemas-upnp-org:device:MediaRenderer:";

typedef struct _UCDlnaPlayerNode {
    UCDlnaPlayer player;
    struct _UCDlnaPlayerNode* next;
} UCDlnaPlayerNode;

static CgThread* sThread = NULL;
static CgCond* sCond = NULL;
static CgMutex* sMutex = NULL;
static UCDlnaPlayerNode* sFirstPlayer = NULL;
static ucdlna_player_message_listener sPlayerMessageListener = NULL;

static UCDlnaPlayerNode* UCDlnaPlayerNode_new(char* id, char* friendlyName, char* iconUri) {
    UCDlnaPlayerNode* n = (UCDlnaPlayerNode*) malloc(sizeof(UCDlnaPlayerNode));
    n->player.id = cg_strdup(id);
    n->player.friendlyName = cg_strdup(friendlyName);
    n->player.iconUri = cg_strdup(iconUri);
    n->next = NULL;
    return n;
}

static void UCDlnaPlayerNode_delete(UCDlnaPlayerNode* n) {
    free(n->player.id);
    free(n->player.friendlyName);
    free(n->player.iconUri);
    free(n);
}

static UCDlnaPlayerNode* UCDlnaPlayerNode_add(UCDlnaPlayerNode* head, char* id, char* friendlyName, char* iconUri) {
    UCDlnaPlayerNode* n = UCDlnaPlayerNode_new(id, friendlyName, iconUri);
    n->next = head;
    return n;
}

static UCDlnaPlayerNode* UCDlnaPlayerNode_find(UCDlnaPlayerNode* head, char const* id) {
    while (head && strcmp(head->player.id, id))
        head = head->next;
    return head;
}

static UCDlnaPlayerNode* UCDlnaPlayerNode_remove(UCDlnaPlayerNode* head, UCDlnaPlayerNode* node) {
    if (head == node) {
        head = head->next;
    } else {
        UCDlnaPlayerNode* next = head;
        while (next->next != node)
            next = next->next;
        next->next = node->next;
    }
    UCDlnaPlayerNode_delete(node);
    return head;
}

static void UCDlnaPlayerNode_dump(UCDlnaPlayerNode* head) {
    if (head) {
        int index = 1;
        UCDLNA_LOGV(LOG_TAG, "DLNA MediaRenderer list:");
        while (head) {
            UCDLNA_LOGV(LOG_TAG, " player %d - friendlyName: %s, id: %s",
                    index, head->player.friendlyName, head->player.id);
            head = head->next;
            ++index;
        }
    } else {
        UCDLNA_LOGV(LOG_TAG, "DLNA MediaRenderer list is empty!");
    }
}

static void release_resource(void) {
    while (sFirstPlayer) {
        UCDlnaPlayerNode* next = sFirstPlayer->next;
        UCDlnaPlayerNode_delete(sFirstPlayer);
        sFirstPlayer = next;
    }
}

void ucdlna_control_init(void) {
    if (sMutex)
        return;
    ucdlna_log_init();
    sMutex = cg_mutex_new();
    sCond = cg_cond_new();
}

void ucdlna_control_uinit(void) {
    ucdlna_control_stop();
    if (sCond) {
        cg_cond_delete(sCond);
        sCond = NULL;
    }
    if (sMutex) {
        cg_mutex_delete(sMutex);
        sMutex = NULL;
    }
    release_resource();
    ucdlna_log_uinit();
}

static char const*const deviceStatusDesc(CgUpnpDeviceStatus status) {
    switch (status) {
        case CgUpnpDeviceStatusAdded: return "added";
        case CgUpnpDeviceStatusUpdated: return "updated";
        case CgUpnpDeviceStatusInvalid: return "invalid";
        case CgUpnpDeviceStatusRemoved: return "removed";
    }
}

static CgString* get_dev_icon(CgUpnpDevice* dev) {
    CgUpnpIcon* icon = cg_upnp_device_geticon(dev, 0);
    if (icon) {
        CgString* buf = cg_string_new();
        if (cg_upnp_device_getabsoluteiconurl(dev, icon, buf))
            return buf;
        else
            cg_string_delete(buf);
    }
    return NULL;
}

static char* sActionsNeed[] = {
    "SetAVTransportURI", "Play", "Pause"
};

static BOOL device_is_valid_player(CgUpnpDevice* dev) {
    int i;
    char* type;
    CgUpnpService* svc;
    type = cg_upnp_device_getdevicetype(dev);
    if (cg_strncmp(type, PLAYER_TYPE, cg_strlen(PLAYER_TYPE)))
        return FALSE;
    svc = cg_upnp_device_getservicebyexacttype(dev, "urn:schemas-upnp-org:service:AVTransport:1");
    if (!svc)
        return FALSE;
    for (i = 0; i < sizeof(sActionsNeed) / sizeof(sActionsNeed[0]); ++i) {
        char* action = sActionsNeed[i];
        if (!cg_upnp_service_hasactionbyname(svc, action)) {
            UCDLNA_LOGI(LOG_TAG, "service has not action - %s", action);
            return FALSE;
        }
    }
    return TRUE;
}

static void on_device_message(CgUpnpControlPoint *ctrlPoint,
        char *udn, CgUpnpDeviceStatus status) {
#if 0
    UCDLNA_LOGV(LOG_TAG, "on device message - id: %s, type: %s",
            udn, deviceStatusDesc(status));
#else
    char* id = udn;
    if (status == CgUpnpDeviceStatusAdded) {
        CgUpnpDevice* dev = cg_upnp_controlpoint_getdevicebyudn(ctrlPoint, udn);
        if (!dev || !device_is_valid_player(dev)) {
            return;
        } else {
            UCDlnaPlayerNode* node;
            cg_mutex_lock(sMutex);
            char *friendlyName = cg_upnp_device_getfriendlyname(dev);
            CgString *cgIconUri = get_dev_icon(dev);
            char *iconUri = cgIconUri ? cg_string_getvalue(cgIconUri) : "";
            UCDLNA_LOGV(LOG_TAG,
                    "on device add, name: %s, type: %s, icon: %s, id: %s",
                    friendlyName, cg_upnp_device_getdevicetype(dev), iconUri, udn);
            node = UCDlnaPlayerNode_find(sFirstPlayer, id);
            if (node) {
                UCDLNA_LOGI(LOG_TAG, "device add had add, name: %s/%s",
                        node->player.friendlyName, friendlyName);
            } else {
                sFirstPlayer = UCDlnaPlayerNode_add(sFirstPlayer, id, friendlyName, iconUri);
                cg_string_delete(cgIconUri);
                UCDlnaPlayerNode_dump(sFirstPlayer);
                if (sPlayerMessageListener)
                    sPlayerMessageListener(&sFirstPlayer->player, UCDlnaPlayerMessageIdAdd);
            }
            cg_mutex_unlock(sMutex);
        }
    } else if (status == CgUpnpDeviceStatusInvalid || status == CgUpnpDeviceStatusRemoved) {
        cg_mutex_lock(sMutex);
        UCDlnaPlayerNode* node = UCDlnaPlayerNode_find(sFirstPlayer, id);
        if (node) {
            UCDLNA_LOGI(LOG_TAG, "on device(%s) remove", node->player.friendlyName);
            if (sPlayerMessageListener)
                sPlayerMessageListener(&node->player, UCDlnaPlayerMessageIdRemove);
            sFirstPlayer = UCDlnaPlayerNode_remove(sFirstPlayer, node);
            UCDlnaPlayerNode_dump(sFirstPlayer);
        } else {
            //UCDLNA_LOGW(LOG_TAG, "can not found device for %s", id);
        }
        cg_mutex_unlock(sMutex);
    }
#endif
}

static void on_ssdp_notify(CgUpnpSSDPPacket *ssdpPkt)
{
    if (cg_upnp_ssdp_packet_isdiscover(ssdpPkt)) {
        UCDLNA_LOGV(LOG_TAG,"ssdp:discover : ST = %s",
            cg_upnp_ssdp_packet_getst(ssdpPkt));
    } else if (cg_upnp_ssdp_packet_isalive(ssdpPkt)) {
        UCDLNA_LOGV(LOG_TAG, "ssdp:alive : uuid = %s, NT = %s, location = %s",
            cg_upnp_ssdp_packet_getusn(ssdpPkt),
            cg_upnp_ssdp_packet_getnt(ssdpPkt),
            cg_upnp_ssdp_packet_getlocation(ssdpPkt));
    } else if (cg_upnp_ssdp_packet_isbyebye(ssdpPkt)) {
        UCDLNA_LOGV(LOG_TAG, "ssdp:byebye : uuid = %s, NT = %s",
            cg_upnp_ssdp_packet_getusn(ssdpPkt),
            cg_upnp_ssdp_packet_getnt(ssdpPkt));
    }
    cg_upnp_ssdp_packet_print(ssdpPkt);
}

static void on_device_event(CgUpnpProperty *prop) {
    UCDLNA_LOGV(LOG_TAG, "Property Changed (%s) = %s",
        cg_upnp_property_getname(prop),
        cg_upnp_property_getvalue(prop));
}

static void ucdlna_control_thread(CgThread *thread) {
    CgUpnpControlPoint* ctrlPoint;

    UCDLNA_LOGV(LOG_TAG, "ucdlna control start...");

    ctrlPoint = cg_upnp_controlpoint_new();

    cg_upnp_controlpoint_setssdplistener(ctrlPoint, on_ssdp_notify);
    cg_upnp_controlpoint_seteventlistener(ctrlPoint, on_device_event);
    cg_upnp_controlpoint_setdevicelistener(ctrlPoint, on_device_message);

    if (cg_upnp_controlpoint_start(ctrlPoint)) {
        //cg_upnp_controlpoint_search(ctrlPoint, "urn:schemas-upnp-org:device:MediaRenderer:1");
        //cg_upnp_controlpoint_search(ctrlPoint, "urn:schemas-upnp-org:device:MediaRenderer:2");
        //cg_upnp_controlpoint_search(ctrlPoint, "upnp:rootdevice");
        //cg_upnp_controlpoint_search(ctrlPoint, "ssdp:all");

        while (cg_thread_isrunnable(thread)) {
            cg_upnp_controlpoint_search(ctrlPoint, "urn:schemas-upnp-org:device:MediaRenderer:1");
            cg_mutex_lock(sMutex);
            cg_cond_wait(sCond, sMutex, 10);
            if (!cg_thread_isrunnable(thread)) {
                cg_mutex_unlock(sMutex);
                break;
            }
            UCDlnaPlayerNode_dump(sFirstPlayer);
            cg_mutex_unlock(sMutex);
        }

        cg_upnp_controlpoint_stop(ctrlPoint);

        UCDLNA_LOGV(LOG_TAG, "ucdlna control stop.");
    } else {
        UCDLNA_LOGE(LOG_TAG, "Couldn't start this control point !!");
    }

    cg_upnp_controlpoint_delete(ctrlPoint);
}

int ucdlna_control_start(void) {
    TRACE;
    if (sThread)
        return 0;
    sThread = cg_thread_new();
    cg_thread_setaction(sThread, ucdlna_control_thread);
    if (cg_thread_start(sThread) == FALSE) {
        if (sThread) {
            cg_thread_delete(sThread);
            sThread = NULL;
        }
        return -1;
    }
    return 0;
}

void ucdlna_control_stop(void) {
    TRACE;
    if (sThread) {
        cg_thread_delete(sThread);
        sThread = NULL;
    }
    release_resource();
}

void ucdlna_set_player_message_listener(ucdlna_player_message_listener listener) {
    sPlayerMessageListener = listener;
}

int ucdlna_control_setUri(const char* playerId, const char* uri) {
    TRACE;
    return 0;
}

int ucdlna_control_play(const char* playerId) {
    TRACE;
    return 0;
}

int ucdlna_control_pause(const char* playerId) {
    TRACE;
    return 0;
}
