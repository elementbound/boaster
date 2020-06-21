#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "include/boaster/executor.h"

#define THREAD_COUNT 2
#define TASK_COUNT THREAD_COUNT * 3

typedef struct {
    int sleep_us;
    int id;
} args_t;

void sleepsome(void *arg) {
    args_t *args = (args_t*) arg;

    usleep(args->sleep_us);
    printf("[%d] Slept %d us\n", args->id, args->sleep_us);
}

int main() {
    args_t task_args[TASK_COUNT];
    boaster_executor_t *executor = boaster_executor_create(THREAD_COUNT);

    printf("Running with %d threads and %d tasks\n", THREAD_COUNT, TASK_COUNT);

    srand(time(NULL));
    for (int i = 0; i < TASK_COUNT; ++i) {
        task_args[i].sleep_us = 500000 + (rand() % 500000);
        task_args[i].id = i;
        printf("Pushing task[%d] with %d us sleep\n", i, task_args[i].sleep_us);

        boaster_executor_push_task(executor, sleepsome, &task_args[i]);
    }

    boaster_executor_join(executor);

    return 0;
}
