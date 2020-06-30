#ifndef TESTBOAT_H_
#define TESTBOAT_H_

#include <stdio.h>

extern const char* __current_test;
extern int __current_pass;
extern int __tests_finished;
extern int __tests_passed;

void test_start(const char* name);
void test_finish();
int test_summarize();

#define test_assert(condition, message) {\
    if (!(condition)) { \
        printf("[TEST][FAIL] %s\n", __current_test); \
        printf("    Assertion failed: " #condition "\n"); \
        printf("    Message: %s\n", message); \
        __current_pass = 0; \
    } \
}

#define test(name, test) { \
    test_start(name);\
    test \
    test_finish(); \
}


#endif  // TESTBOAT_H_
