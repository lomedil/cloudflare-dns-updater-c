#ifndef __LOGGER__H__
#define __LOGGER__H__

#include <types.h>
#include <stdarg.h>

typedef enum {
    kUndefined,
    kConsole
}  log_appender;

typedef enum {
    kTrace,
    kDebug,
    kInfo,
    kWarning,
    kError
} log_level;

void log_initialize( log_appender level );

void log_message( log_level level, const char *psz_message);

void log_fmt_message(log_level level, const char *fmt, ...);

#define log_trace($msg)     log_message(kTrace, $msg) 
#define log_debug($msg)     log_message(kDebug, $msg) 
#define log_info($msg)      log_message(kInfo, $msg) 
#define log_warn($msg)      log_message(kWarning, $msg) 
#define log_error($msg)     log_message(kError, $msg) 

#define log_fmt_trace($msg, ...)     log_fmt_message(kTrace, $msg, __VA_ARGS__) 
#define log_fmt_debug($msg, ...)     log_fmt_message(kDebug, $msg, __VA_ARGS__) 
#define log_fmt_info($msg, ...)      log_fmt_message(kInfo, $msg, __VA_ARGS__) 
#define log_fmt_warn($msg, ...)      log_fmt_message(kWarning, $msg, __VA_ARGS__) 
#define log_fmt_error($msg, ...)     log_fmt_message(kError, $msg, __VA_ARGS__) 


#endif // __LOGGER__H__