#include "greatest.h"
#include <args.h>

TEST args_read_long_param()
{
    char *args[] = {
        "program",
        "check",
        "--user",
        "noname"};

    args_cli cli = args_init(4, args);

    args_psz user = args_parameter(cli, "u", "user");

    ASSERT_NEQ(user, NULL);
    ASSERT_STR_EQ(user, "noname");

    PASS();
}

TEST args_read_short_param()
{
    char *args[] = {
        "program",
        "check",
        "-u",
        "noname"};

    args_cli cli = args_init(4, args);

    args_psz user = args_parameter(cli, "u", "user");

    ASSERT_NEQ(user, NULL);
    ASSERT_STR_EQ(user, "noname");

    PASS();
}

TEST args_read_not_first_param()
{
    char *args[] = {
        "program",
        "check",
        "--host",
        "moggle.com",
        "--user",
        "noname"};

    args_cli cli = args_init(6, args);

    args_psz user = args_parameter(cli, "u", "user");

    ASSERT_NEQ(user, NULL);
    ASSERT_STR_EQ(user, "noname");

    PASS();
}

TEST args_ret_null_param_not_found()
{
    char *args[] = {
        "program",
        "check",
        "--host",
        "moogle.com"};

    args_cli cli = args_init(4, args);

    args_psz user = args_parameter(cli, "u", "user");

    ASSERT_EQ(user, NULL);

    PASS();
}

TEST args_ret_null_no_more_params()
{
    char *args[] = {
        "program",
        "check",
        "--host",
    };

    args_cli cli = args_init(3, args);

    args_psz user = args_parameter(cli, "u", "user");

    ASSERT_EQ(user, NULL);

    PASS();
}

SUITE(args_suite)
{
    RUN_TEST(args_read_long_param);
    RUN_TEST(args_read_short_param);
    RUN_TEST(args_read_not_first_param);
    RUN_TEST(args_ret_null_param_not_found);
    RUN_TEST(args_ret_null_no_more_params);
}