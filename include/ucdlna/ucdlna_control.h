// Copyright 2015 zhiyong.any@gmail.com

#ifndef _UCDLNA_CONTROL_H_
#define _UCDLNA_CONTROL_H_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _UCDlnaPlayer {
    char* id;
    char* friendlyName;
    char* iconUri;
} UCDlnaPlayer;

typedef enum _UCDlnaPlayerMessageId {
    UCDlnaPlayerMessageIdAdd,
    UCDlnaPlayerMessageIdRemove,
} UCDlnaPlayerMessageId;

typedef void (*ucdlna_player_message_listener)
        (UCDlnaPlayer const*, UCDlnaPlayerMessageId);

void UCDLNA_EXPORT ucdlna_control_init(void);
void UCDLNA_EXPORT ucdlna_control_uinit(void);
int UCDLNA_EXPORT ucdlna_control_start(void);
void UCDLNA_EXPORT ucdlna_control_stop(void);
void UCDLNA_EXPORT ucdlna_set_player_message_listener(ucdlna_player_message_listener listener);
int UCDLNA_EXPORT ucdlna_control_setUri(const char* playerId, const char* uri);
int UCDLNA_EXPORT ucdlna_control_play(const char* playerId);
int UCDLNA_EXPORT ucdlna_control_pause(const char* playerId);

#ifdef __cplusplus
}
#endif

#endif /* _UCDLNA_CONTROL_H_ */
