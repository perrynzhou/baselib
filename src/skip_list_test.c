/*
 * Copyright (C) 2021, perrynzhou <perrynzhou@gmail.com>
*/
#include <stdio.h>
#include <stdlib.h>
#if defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <sys/time.h>

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

int clock_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#else
#include <time.h>
#endif

#include "skiplist.h"

#define N 1*1024*1024
// #define SKIPLIST_DEBUG

int
main(void)
{
    int i;
    struct timespec start, end;

    slice *key = malloc(N * sizeof(slice));
    if (key == NULL) {
        exit(-1);
    }

    skiplist   *list = skiplist_new(slice_cmp);
    if (list == NULL) {
        exit(-1);
    }

    fprintf(stdout,"Test start n=%d!\n",N);
    printf("Add %d nodes...\n", N);

    /* Insert test */
    srandom(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        int value = (int)random();
        char buf[256] = {'\0'};
        snprintf((char *)&buf,256,"test-%d",i);
        key[i] = slice_create((char *)&buf);
        skiplist_insert(list, key[i], value);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
    #ifdef SKIPLIST_DEBUG
    skiplist_dump(list);
    #endif

    /* Search test */
    printf("Now search each node...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        struct skipnode *node = skiplist_search(list, key[i]);
        if (node != NULL) {
             #ifdef SKIPLIST_DEBUG
            printf("key:%s value:%d\n", (char *)node->key, node->value);
            #endif
        } else {
            printf("Not found:%s\n", (char *)key[i]);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);

    /* Delete test */
    printf("Now remove all nodes...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++) {
        skiplist_remove(list, key[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time span: %ldms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
    #ifdef SKIPLIST_DEBUG
    skiplist_dump(list);
    #endif

    printf("End of Test.\n");
    skiplist_delete(list);
    for(int i=0;i<N;i++) {
        slice_destroy(key[i]);
    }
    free(key);

    return 0;
}
