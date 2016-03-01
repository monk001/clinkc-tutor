// Copyright 2015 zhiyong.any@gmail.com

#include <ucdlna/ucdlna_control.h>

#include <cybergarage/util/ctime.h>

static void player_message_listener(
        UCDlnaPlayer const* player, UCDlnaPlayerMessageId msgId) {
}

int main(int argc, char* argv[]) {
    ucdlna_control_init();
    ucdlna_set_player_message_listener(player_message_listener);

#if 1
    if (!ucdlna_control_start()) {
        cg_sleep(10000 * 1000);
        ucdlna_control_stop();
    }
#else
    while (!ucdlna_control_start()) {
        cg_sleep(6000);
        ucdlna_control_stop();
        cg_sleep(1000);
    }
#endif

    ucdlna_control_uinit();

    return 0;
}
