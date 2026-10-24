#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t* mutex;

void* thread_function(void* args) {
    // pthread_mutex_lock(mutex);
    pthread_mutex_lock(&mutex);

    int* sum = args;
    printf("Hello from thread %lu with sum = %d\n", pthread_self(), *sum);
    *sum += 1;
    printf("Threadsum == %d\n", *sum);

    pthread_mutex_unlock(&mutex);
    // pthread_mutex_unlock(mutex);
    pthread_exit(NULL);
}

int main(void) {

    // pthread_mutex_init(mutex, NULL);
    int sum = 0;
    pthread_t tid[5];
    void* ans_code;

    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], NULL, thread_function, (void*)&sum);
        printf("pid %d create thread %lu\n", getpid(), tid[i]);
        // pthread_join(tid, &ans_code);
    }
    
    pthread_join(tid[4], &ans_code);
    printf("pid %d join threads with sum %d\n", getpid(), sum);
    // pthread_mutex_destroy(mutex);
    return 0;
}