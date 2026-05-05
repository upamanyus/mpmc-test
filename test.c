#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mpmc.h"

#define N 8
#define OPS_PER_THREAD 1000

static queue_t q;

static void *worker(void *arg) {
    int id = (int)(long)arg;
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        int *val = malloc(sizeof(int));
        *val = id * OPS_PER_THREAD + i;

        if (!queue_push(&q, val)) {
            fprintf(stderr, "thread %d: queue_push failed at i=%d\n", id, i);
            free(val);
            return NULL;
        }

        void *out = queue_pop(&q);
        // if (out == NULL) {
        // fprintf(stderr, "thread %d: queue_pop returned NULL at i=%d\n", id, i);
        // return NULL;
        // }
        if (out != NULL) {
            free(out);
        }
    }
    return (void *)1;
}

int main(void) {
    queue_init(&q);

    pthread_t threads[N];
    for (int i = 0; i < N; i++)
        pthread_create(&threads[i], NULL, worker, (void *)(long)i);

    int ok = 1;
    for (int i = 0; i < N; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        if (ret == NULL) {
            fprintf(stderr, "thread %d failed\n", i);
            ok = 0;
        }
    }

    if (ok)
        printf("all %d threads completed successfully\n", N);
    return ok ? 0 : 1;
}
