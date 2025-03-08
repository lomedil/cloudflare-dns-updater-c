#include "greatest.h"
#include <buffer.h>

TEST buffer_initialize_test()
{
    buffer_t buffer = buffer_init(16);

    ASSERT_EQ(buffer.size, 16);
    ASSERT_NEQ(buffer.data, 0);

    buffer_release(&buffer);

    PASS();
}

TEST buffer_release_test()
{
    buffer_t buffer = buffer_init(16);

    buffer_release(&buffer);

    ASSERT_EQ(buffer.size, 0);
    ASSERT_EQ(buffer.data, 0);

    PASS();
}

TEST buffer_aligned4_intialization_test()
{
    buffer_t buffer = buffer_init(21);

    ASSERT_EQ(buffer.size, 24);
    ASSERT_NEQ(buffer.data, 0);

    buffer_release(&buffer);

    PASS();
}

TEST buffer_ensure_space_test()
{
    buffer_t buffer = buffer_init(16);

    for (int i = 0; i < 16; ++i)
    {
        buffer.data[i] = i;
    }

    char *p_orig_ptr = buffer.data;
    int diff_size = buffer_ensure_space(&buffer, 32);

    ASSERT_EQ(diff_size, 16);
    ASSERT_EQ(buffer.size, 32);
    ASSERT_NEQ(p_orig_ptr, buffer.data);

    for (int i = 0; i < 16; ++i)
    {
        ASSERT_EQ(i, buffer.data[i]);
    }

    buffer_release(&buffer);

    PASS();
}

TEST buffer_no_change_space_test()
{
    buffer_t buffer = buffer_init(16);

    char *p_orig_ptr = buffer.data;
    int diff_size = buffer_ensure_space(&buffer, 8);

    ASSERT_EQ(diff_size, 0);
    ASSERT_EQ(p_orig_ptr, buffer.data);

    buffer_release(&buffer);

    PASS();
}

TEST buffer_take_ownership_test()
{
    buffer_t buffer_orig = buffer_init(16);
    for (int i = 0; i < buffer_orig.size; ++i)
        buffer_orig.data[i] = i;

    buffer_t buffer_owner = buffer_take(&buffer_orig);

    ASSERT_EQ(buffer_orig.size, 0);
    ASSERT_EQ(buffer_orig.data, 0);

    // It should not crash on release
    buffer_release(&buffer_orig);

    ASSERT_EQ(buffer_owner.size, 16);
    ASSERT_EQ(buffer_owner.data[0], 0);
    //...
    ASSERT_EQ(buffer_owner.data[15], 15);

    PASS();
}

TEST buffer_clone_test()
{
    buffer_t buffer_orig = buffer_init(16);
    for (int i = 0; i < buffer_orig.size; ++i)
        buffer_orig.data[i] = i;

    buffer_t buffer_owner = buffer_clone(&buffer_orig);

    ASSERT_EQ(buffer_orig.size, 16);
    ASSERT_EQ(buffer_orig.data[0], 0);
    ASSERT_EQ(buffer_orig.data[15], 15);

    ASSERT_EQ(buffer_owner.size, 16);
    ASSERT_EQ(buffer_owner.data[0], 0);
    ASSERT_EQ(buffer_owner.data[15], 15);

    ASSERT_NEQ(buffer_owner.data, buffer_orig.data);

    PASS();
}

SUITE(buffer_suite)
{
    RUN_TEST(buffer_initialize_test);
    RUN_TEST(buffer_aligned4_intialization_test);
    RUN_TEST(buffer_release_test);
    RUN_TEST(buffer_ensure_space_test);
    RUN_TEST(buffer_no_change_space_test);
    RUN_TEST(buffer_take_ownership_test);
    RUN_TEST(buffer_clone_test);
}