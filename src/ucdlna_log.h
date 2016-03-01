// Copyright 2015 zhiyong.any@gmail.com

#ifndef _UCDLNA_LOG_H_
#define _UCDLNA_LOG_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void ucdlna_log_init(void);
void ucdlna_log_uinit(void);

/*
 * uc log priority values, in ascending priority order.
 *   value same as android/log.h
 */
#define UCDLNA_LOG_PRIO_UNKNOWN 0
#define UCDLNA_LOG_PRIO_DEFAULT 1 /* only for UCDLNA_LOG_MIN_PRIORITY */
#define UCDLNA_LOG_PRIO_VERBOSE 2
#define UCDLNA_LOG_PRIO_DEBUG   3
#define UCDLNA_LOG_PRIO_INFO    4
#define UCDLNA_LOG_PRIO_WARN    5
#define UCDLNA_LOG_PRIO_ERROR   6
#define UCDLNA_LOG_PRIO_FATAL   7
#define UCDLNA_LOG_PRIO_SILENT  8 /* only for UCDLNA_LOG_MIN_PRIORITY */

#ifndef UCDLNA_LOG_MIN_PRIORITY
# ifdef NDEBUG
#   define UCDLNA_LOG_MIN_PRIORITY UCDLNA_LOG_PRIO_VERBOSE
# else
#   define UCDLNA_LOG_MIN_PRIORITY UCDLNA_LOG_PRIO_VERBOSE
# endif
#endif

/*
 * Send a simple string to the log.
 */
int ucdlna_log_write(int prio, const char *tag, const char *text);

/*
 * Send a formatted string to the log, used like printf(fmt,...)
 */
int ucdlna_log_print(int prio, const char *tag,  const char *fmt, ...)
#if defined(__GNUC__)
    __attribute__ ((format(printf, 3, 4)))
#endif
    ;

/*
 * A variant of ucdlna_log_print() that takes a va_list to list
 * additional parameters.
 */
int ucdlna_log_vprint(int prio, const char *tag, const char *fmt, va_list ap);

/* --------------------------------------------------------------------- */

#define __UCDLNA_LOG_CONDITION(cond) (__builtin_expect((cond)!=0, 0))
#define __UCDLNA_LOG(priority, tag, ...) ucdlna_log_print(UCDLNA_##priority, tag, __VA_ARGS__)

/*
 * Simplified macro to send a verbose log message
 */
#if UCDLNA_LOG_PRIO_VERBOSE < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGV(tag, ...) ((void)0)
# define UCDLNA_LOGV_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGV(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_VERBOSE, tag, __VA_ARGS__))
# define UCDLNA_LOGV_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_VERBOSE, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a debug log message
 */
#if UCDLNA_LOG_PRIO_DEBUG < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGD(tag, ...) ((void)0)
# define UCDLNA_LOGD_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGD(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_DEBUG, tag, __VA_ARGS__))
# define UCDLNA_LOGD_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_DEBUG, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an info log message
 */
#if UCDLNA_LOG_PRIO_INFO < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGI(tag, ...) ((void)0)
# define UCDLNA_LOGI_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGI(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_INFO, tag, __VA_ARGS__))
# define UCDLNA_LOGI_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_INFO, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning log message
 */
#if UCDLNA_LOG_PRIO_WARN < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGW(tag, ...) ((void)0)
# define UCDLNA_LOGW_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGW(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_WARN, tag, __VA_ARGS__))
# define UCDLNA_LOGW_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_WARN, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message
 */
#if UCDLNA_LOG_PRIO_ERROR < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGE(tag, ...) ((void)0)
# define UCDLNA_LOGE_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGE(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_ERROR, tag, __VA_ARGS__))
# define UCDLNA_LOGE_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_ERROR, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an fatal log message
 */
#if UCDLNA_LOG_PRIO_FATAL < UCDLNA_LOG_MIN_PRIORITY
# define UCDLNA_LOGF(tag, ...) ((void)0)
# define UCDLNA_LOGF_IF(cond, tag, ...) ((void)0)
#else
# define UCDLNA_LOGF(tag, ...) ((void)__UCDLNA_LOG(LOG_PRIO_FATAL, tag, __VA_ARGS__))
# define UCDLNA_LOGF_IF(cond, tag, ...) \
    ( (__UCDLNA_LOG_CONDITION(cond)) \
    ? ((void)__UCDLNA_LOG(LOG_PRIO_FATAL, tag, __VA_ARGS__)) \
    : (void)0 )
#endif

#define TRACE UCDLNA_LOGV(LOG_TAG, "fun: %s, line: %d", __FUNCTION__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* _UCDLNA_LOG_H_ */
