#ifndef BOASTER_EXECUTOR_H_
#define BOASTER_EXECUTOR_H_

typedef void(*boaster_runner_t)(void *arg);
typedef struct boaster_executor_t boaster_executor_t;

boaster_executor_t* boaster_executor_create(int num_threads);

void boaster_executor_push_task(
    boaster_executor_t *executor,
    boaster_runner_t func,
    void *arg
);

void boaster_executor_join(boaster_executor_t *executor);
void boaster_executor_terminate(boaster_executor_t *executor);

void boaster_executor_free(boaster_executor_t *executor);

#endif  // BOASTER_EXECUTOR_H_
