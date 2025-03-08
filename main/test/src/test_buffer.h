#include "greatest.h"
#include <buffer.h>

TEST buffer_initialize_test() {
    buffer_t buffer = buffer_init(16);

    ASSERT_EQ(buffer.size, 16);
    ASSERT_NEQ(buffer.data, 0);

    buffer_release(&buffer);

    PASS();
}

TEST buffer_release_test() {
    buffer_t buffer = buffer_init(16);

    buffer_release(&buffer);

    ASSERT_EQ(buffer.size, 0);
    ASSERT_EQ(buffer.data, 0);

    PASS();
}

TEST buffer_aligned4_intialization_test() {
    buffer_t buffer = buffer_init(21);

    ASSERT_EQ(buffer.size, 24);
    ASSERT_NEQ(buffer.data, 0);

    buffer_release(&buffer);

    PASS();
}

TEST buffer_ensure_space_test() {
    buffer_t buffer = buffer_init(16);
    
    for(int i = 0; i < 16; ++i){
        buffer.data[i] = i;
    }

    char *p_orig_ptr = buffer.data;
    int diff_size = buffer_ensure_space(&buffer, 32);

    ASSERT_EQ(diff_size, 16);
    ASSERT_EQ(buffer.size, 32);
    ASSERT_NEQ(p_orig_ptr, buffer.data);

    for(int i = 0; i < 16; ++i){
        ASSERT_EQ(i, buffer.data[i]);
    }

    buffer_release(&buffer);

    PASS();
}

TEST buffer_no_change_space_test() {
    buffer_t buffer = buffer_init(16);

    char *p_orig_ptr = buffer.data;
    int diff_size = buffer_ensure_space(&buffer, 8);

    ASSERT_EQ(diff_size, 0);
    ASSERT_EQ(p_orig_ptr, buffer.data);

    buffer_release(&buffer);

    PASS();
}

SUITE(buffer_suite){
    RUN_TEST(buffer_initialize_test);
    RUN_TEST(buffer_aligned4_intialization_test);
    RUN_TEST(buffer_release_test);
    RUN_TEST(buffer_ensure_space_test);
    RUN_TEST(buffer_no_change_space_test);
}