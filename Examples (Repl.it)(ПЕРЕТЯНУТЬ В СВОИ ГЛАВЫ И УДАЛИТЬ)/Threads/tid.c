#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int ForAllValue = 0;

// Функция, выполняемая нитью
void* thread_function(void* arg) {
    printf("Нить %ld: %d\n", pthread_self(), ++ForAllValue);
    return &ForAllValue;
}

int main() {
    pthread_t threads[5];
    int result;

    for (int i = 0; i < 5; i++) {
        result = pthread_create(&threads[i], NULL, thread_function, NULL);
        if (result != 0) {
            fprintf(stderr, "Ошибка создания нити\n");
            exit(EXIT_FAILURE);
        }
        printf("Thread %ld created successfully\n", threads[i]);
    }

    int sum = 0;
    for (int i = 0; i < 5; i++) {
        int value = 0;
        void* ptr = &value;
        pthread_join(threads[i], &ptr);
        sum += * (int*) ptr;
        printf("Thread %ld joined successfully\n", threads[i]);
    }
    
    printf("%d\n", sum);
    return 0;
}