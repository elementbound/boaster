#include "include/test/test.h"
#include <stdio.h>

const char* __current_test;
int __current_pass;
int __tests_finished = 0;
int __tests_passed = 0;

void test_start(const char* name) {
    printf("[TEST][START] %s\n", name);
    __current_test = name;
    __current_pass = 1;
}

void test_finish() {
    __tests_finished++;

    if (__current_pass) {
        __tests_passed++;

        printf("[TEST][PASS] %s\n", __current_test);
    } else {
        printf("[TEST][FAIL] %s\n", __current_test);
    }

    printf("----\n");
}

int test_summarize() {
    if (__tests_finished == __tests_passed) {
        printf("[SUITE][PASS] %d / %d passed\n", \
            __tests_passed, __tests_finished);

        return 0;
    } else {
        printf("[SUITE][FAIL] %d / %d passed\n", \
            __tests_passed, __tests_finished);

        return 1;
    }
}
