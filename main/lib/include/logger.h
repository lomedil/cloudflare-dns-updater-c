#ifndef __LOGGER__H__
#define __LOGGER__H__

#include <types.h>

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

void log_initialize( log_appender );

void log_message( log_level level, const char* psz_message);

#define log_trace($msg)     log_message(kTrace, $msg) 
#define log_debug($msg)     log_message(kDebug, $msg) 
#define log_info($msg)      log_message(kInfo, $msg) 
#define log_warn($msg)      log_message(kWarning, $msg) 
#define log_error($msg)     log_message(kError, $msg) 


#endif // __LOGGER__H__