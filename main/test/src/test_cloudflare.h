#include "greatest.h"
#include <cloudflare.h>

TEST cf_parse_success()
{

    const char *json = "{ \"success\": true, \"errors\": [] }";

    cf_response response = {0};
    int err = cf_parse_response_from_psz(json, &response);

    ASSERT_EQ(err, 0);
    ASSERT_EQ(response.success, 1);

    // End of errors are marked with first full empty "row"
    ASSERT_EQ(response.errors[0].code, 0);
    ASSERT_EQ(response.errors[0].message, 0);

    PASS();
}

TEST cf_parse_errors()
{
    const char *json =
        "{"
        "  \"success\": false,"
        "  \"errors\": ["
        "    { \"code\": 10000, \"message\": \"Authentication error\" },"
        "    { \"code\": 10001, \"message\": \"Invalid request\" }"
        "  ]"
        "}";

    cf_response response = {0};
    int err = cf_parse_response_from_psz(json, &response);

    ASSERT_EQ(err, 0);
    ASSERT_EQ(response.success, 0);

    ASSERT_EQ(10000, response.errors[0].code);
    ASSERT_STR_EQ("Authentication error", response.errors[0].message);

    ASSERT_EQ(10001, response.errors[1].code);
    ASSERT_STR_EQ("Invalid request", response.errors[1].message);

    // End of errors are marked with first full empty "row"
    ASSERT_EQ(response.errors[2].code, 0);
    ASSERT_EQ(response.errors[2].message, 0);

    PASS();
}

SUITE(cloudflare_suite)
{
    RUN_TEST(cf_parse_success);
    RUN_TEST(cf_parse_errors);
}
