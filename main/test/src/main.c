#include "greatest.h"
#include "test_args.h"

#include <logger.h>

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    log_initialize(kConsole);

    log_trace("Hello trace");
    log_debug("Hello debug");
    log_info("Hello info");
    log_warn("Hello warn");
    log_error("Hello error");

    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Individual tests can be run directly in main, outside of suites. */
    /* RUN_TEST(x_should_equal_1); */

    /* Tests can also be gathered into test suites. */
    
    RUN_SUITE(args_suite);

    GREATEST_MAIN_END();        /* display results */

}