#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Функция, выполняемая нитью
void* thread_function(void* arg) {
    int num = * (int*) arg;
    printf("Нить получила число: %d\n", num);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int number = 42;
    int result;

    // Создание нити
    result = pthread_create(&thread, NULL, thread_function, &number);
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения нити
    pthread_join(thread, NULL);
    printf("Нить завершена\n");
    return 0;
}