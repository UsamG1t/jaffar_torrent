#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

void* thread_function(void* args) {
    printf("Hello from thread\n");
    int* sum = args;
    *sum += 1;
    printf("Threadsum == %d\n", *sum);
}

int main(void) {

    int sum = 0;
    pthread_t tid;
    void* ans_code;

    for (int i = 0; i < 5; i++) {
        pthread_create(&tid, NULL, thread_function, (void*)&sum);
        printf("pid %d create thread %lu\n", getpid(), tid);
        // pthread_cancel(tid);
        // printf("pid %d trya cancel thread %lu\n", getpid(), tid);
        pthread_join(tid, &ans_code);
        printf("pid %d join thread %lu with sum %d\n", getpid(), tid, sum);
    }

    return 0;
}