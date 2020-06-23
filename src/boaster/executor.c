#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include "include/boaster/buffer.h"
#include "include/boaster/executor.h"

typedef struct {
    boaster_runner_t runner;
    void *arg;
    int success;
} boaster_task_t;

struct boaster_executor_t {
    pthread_t* threads;
    pthread_mutex_t mutex;

    int num_threads;
    boaster_buffer_t *task_buffer;
    int task_at;
    int task_count;
    int is_running;
    int is_joining;
};

void boaster_executor_lock(boaster_executor_t *executor) {
    pthread_mutex_lock(&executor->mutex);
}

void boaster_executor_unlock(boaster_executor_t *executor) {
    pthread_mutex_unlock(&executor->mutex);
}

boaster_task_t boaster_executor_pull_task(boaster_executor_t *executor) {
    boaster_executor_lock(executor);

    while (executor->task_at == executor->task_count) {
        boaster_executor_unlock(executor);

        boaster_task_t task;
        task.success = 0;

        return task;
    }

    boaster_task_t *tasks = executor->task_buffer->data;
    boaster_task_t task = tasks[executor->task_at];
    task.success = 1;

    ++executor->task_at;

    boaster_executor_unlock(executor);

    return task;
}

void *boaster_runner(void *arg) {
    boaster_executor_t *executor = (boaster_executor_t*) arg;

    while (1) {
        boaster_task_t task = boaster_executor_pull_task(executor);

        if (task.success) {
            task.runner(task.arg);
        } else if (executor->is_joining) {
            break;
        }
    }

    pthread_exit(NULL);
}

boaster_executor_t* boaster_executor_create(int num_threads) {
    boaster_executor_t *result =
        (boaster_executor_t*) malloc(sizeof(boaster_executor_t));

    result->num_threads = num_threads;
    result->threads =
        (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
    result->task_buffer = boaster_buffer_create();

    result->task_at = 0;
    result->task_count = 0;
    result->is_running = 1;
    result->is_joining = 0;

    // Init mutex
    pthread_mutex_init(&result->mutex, NULL);

    // Init threads
    for (int i = 0; i < num_threads; ++i) {
        int rc =
            pthread_create(&result->threads[i], NULL, boaster_runner, result);

        if (rc) {
            // Couldn't create thread; fail
            boaster_executor_free(result);
            return NULL;
        }
    }

    return result;
}

void boaster_executor_push_task(
    boaster_executor_t *executor,
    boaster_runner_t func,
    void *arg
) {
    boaster_executor_lock(executor);

    boaster_task_t task = {
        .runner = func,
        .arg = arg
    };

    boaster_buffer_push_data(executor->task_buffer, boaster_task_t, task);
    ++(executor->task_count);

    boaster_executor_unlock(executor);
}

void boaster_executor_join(boaster_executor_t *executor) {
    executor->is_joining = 1;

    for (int i = 0; i < executor->num_threads; ++i) {
        void *status;
        pthread_join(executor->threads[i], &status);
    }

    executor->is_running = 0;
}

void boaster_executor_terminate(boaster_executor_t *executor) {
    for (int i = 0; i < executor->num_threads; ++i) {
        pthread_cancel(executor->threads[i]);
    }

    executor->is_running = 0;
}

void boaster_executor_free(boaster_executor_t *executor) {
    assert(executor->is_running == 0);

    free(executor->threads);
    boaster_buffer_destroy(executor->task_buffer);

    free(executor);
}
