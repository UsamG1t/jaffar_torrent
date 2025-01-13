#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

void* thread_func(void* args) {
    printf("Hello from thread %lu\n", pthread_self());
    return NULL;
}

int main(void) {

    pthread_t tid[5];
    void* attr;

    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], NULL, thread_func, NULL/*args*/);
        printf("pid %d create thread %lu\n", getpid(), tid[i]);
        pthread_join(tid[i], &attr);
        printf("pid %d join thread %lu\n\n\n", getpid(), tid[i]);
    }

    return 0;
}

// ---------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

struct Summa {
    int count;
    float sum;
};

pthread_t tid[5] = {};
void* attr;

void* thread_func(void* args) {
    for (int j = 1; j < 5; j++) {
        if (pthread_equal(pthread_self(), tid[j])) {
            pthread_join(tid[j-1], NULL);
            break;
        }
    }

    struct Summa* tmp = (struct Summa*) args;
    tmp->sum += 0.5 * (pthread_self() % 7);
    tmp->count++;
    printf("%lu insert smth in sum with pid%d\n\n", pthread_self(), getpid());
    return NULL;
}

int main(void) {
    struct Summa a = {.count = 0, .sum = 0.};

    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], NULL, thread_func, (void*) &a);
    }

    pthread_join(tid[4], &attr);
    printf("pid %d join %d threads with %f\n\n\n", getpid(), a.count, a.sum);

    return 0;
}

// ---------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_t tid[5] = {};
void* attr;

void* thread_func(void* args) {
    printf("%lu  hello\n\n", pthread_self());
    return NULL;
}

int main(void) {
    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], NULL, thread_func, NULL);
    }

    return 0;
}

// ------------------MUTEX--------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_sum(void* args) {
    int* sum = (int*) args;

    pthread_mutex_trylock(&mutex);
    printf("%lu - %d\n", pthread_self(), *sum);
    *sum += pthread_self() % 100;
    printf("~%lu - %d\n", pthread_self(), *sum);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void) {
    int sum = 0;
    pthread_t tid[10];

    for (int i = 0; i < 10; i++) {
        pthread_create(&tid[i], NULL, thread_sum, (void*) &sum);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

