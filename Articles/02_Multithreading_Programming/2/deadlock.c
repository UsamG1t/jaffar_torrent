#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&mutex1);
    printf("Нить 1: Захватила mutex1\n");
    sleep(1);
    pthread_mutex_lock(&mutex2);
    printf("Нить 1: Захватила mutex2\n");

    // Освобождение мьютексов
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    pthread_exit(NULL);
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&mutex2);
    printf("Нить 2: Захватила mutex2\n");
    sleep(1);
    pthread_mutex_lock(&mutex1);
    printf("Нить 2: Захватила mutex1\n");

    // Освобождение мьютексов
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    int ret;

    // Инициализация мьютексов
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Создание нитей
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    // Ожидание завершения нитей
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Разрушение мьютексов
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
