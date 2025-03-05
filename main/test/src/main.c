#include "greatest.h"
#include "test_args.h"

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {

    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Individual tests can be run directly in main, outside of suites. */
    /* RUN_TEST(x_should_equal_1); */

    /* Tests can also be gathered into test suites. */
    
    RUN_SUITE(args_suite);

    GREATEST_MAIN_END();        /* display results */

}