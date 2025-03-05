#include <logger.h>
#include <assert.h>
#include <stdio.h>

#include "buffer.h"

typedef void (*appender_func)(char *msg);

void console_writer(char *msg)
{
    printf("%s", msg);
}

struct logger_state_t
{
    log_appender appender;
    appender_func writer;
    buffer_t buffer;
} *p_logger = 0;

void log_initialize(log_appender appender)
{
    if (p_logger)
    {
        assert("Logger already initialized");
    }

    p_logger = (struct logger_state_t *)malloc(sizeof(struct logger_state_t));
    if (!p_logger)
    {
        assert("Logger cannot be instantiated");
    }

    p_logger->appender = appender;
    p_logger->buffer = buffer_init(256);

    switch (appender)
    {
    case kConsole:
        p_logger->writer = &console_writer;
        break;
    default:
        assert("Appender not supported");
    }
}

const char *level_to_str(log_level level)
{
    switch (level)
    {
    case kTrace:
        return "TRACE";
    case kDebug:
        return "DEBUG";
    case kError:
        return "ERROR";
    case kWarning:
        return "WARN";
    case kInfo:
    default:
        return "INFO";
    }
}

void log_message(log_level level, const char *psz_message)
{
    int required_size = 0;

    required_size = strlen(psz_message);
    required_size = ((required_size >> 1) << 1) + 4; // aligned to 4

    buffer_t *p_buffer = &p_logger->buffer;

    buffer_ensure_space(p_buffer, required_size);
    buffer_clear(p_buffer);

    sprintf(p_buffer->data, "[%s]\t %s\n", level_to_str(level), psz_message);

    p_logger->writer(p_buffer->data);
}