// Copyright 2015 zhiyong.any@gmail.com

#include "ucdlna_log.h"

#ifdef __ANDROID__
# include <android/log.h>
#else
# include <stdio.h>
# include <sys/syscall.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# include <cybergarage/util/cmutex.h>
#endif


#ifndef __ANDROID__

static const char sPrioDesc[] = "??VDIWEFS";

#endif

#ifdef __ANDROID__

int ucdlna_log_vprint(int prio, const char *tag, const char *fmt, va_list ap) {
    return __android_log_vprint(prio, tag, fmt, ap);
}

void ucdlna_log_init(void) {
}

void ucdlna_log_uinit(void) {
}

#else

static CgMutex* sLogMutex = NULL;

void ucdlna_log_init(void) {
    if (!sLogMutex)
        sLogMutex = cg_mutex_new();
}

void ucdlna_log_uinit(void) {
    if (sLogMutex) {
        cg_mutex_delete(sLogMutex);
        sLogMutex = NULL;
    }
}

int ucdlna_log_vprint(int prio, const char *tag, const char *fmt, va_list ap) {
    int res;
    struct timeval t;
    struct tm* tm;

    cg_mutex_lock(sLogMutex);

    if (gettimeofday(&t, 0)) {
        t.tv_sec = time(NULL);
        t.tv_usec = 0;
    }
    tm = localtime(&t.tv_sec);
    res = printf("%02d-%0d %02d:%02d:%02d.%03d %d %d %c %s: ",
                 tm->tm_mon + 1, tm->tm_mday,
                 tm->tm_hour, tm->tm_min, tm->tm_sec, t.tv_usec / 1000,
                 (int) getpid(), (int) syscall(SYS_gettid),
                 sPrioDesc[prio], tag);
    res += vprintf(fmt, ap);
    res += printf("\n");

    cg_mutex_unlock(sLogMutex);

    return res;
}

#endif

int ucdlna_log_write(int prio, const char *tag, const char *text) {
    return ucdlna_log_print(prio, tag, "%s", text);
}

int ucdlna_log_print(int prio, const char *tag, const char *fmt, ...) {
    int res;
    va_list ap;
    va_start(ap, fmt);
    res = ucdlna_log_vprint(prio, tag, fmt, ap);
    va_end(ap);
    return res;
}
