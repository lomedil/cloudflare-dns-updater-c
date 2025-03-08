#include <logger.h>

void test_logs() {
    log_trace("Hello trace");
    log_debug("Hello debug");
    log_info("Hello info");
    log_warn("Hello warn");
    log_error("Hello error");

    log_fmt_trace("Hello trace: %s - %d ", "super", 5);
    log_fmt_debug("Hello debug: %s - %d", "super", 5);
    log_fmt_info("Hello info: %s - %d", "super", 5);
    log_fmt_warn("Hello warn: %s - %d", "super", 5);
    log_fmt_error("Hello error: %s - %d", "super", 5);
}