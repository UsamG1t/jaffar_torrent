#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


// Функция, выполняемая нитью
void* thread_function(void* arg) {
    printf("Я - Самоубивца!!!\n");

    pthread_cancel(pthread_self());

    sleep(4);

    printf("Не получилось...");
    
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int result;

    // Создание нити
    result = pthread_create(&thread, NULL, thread_function, NULL);
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения нити
    pthread_join(thread, NULL);
    printf("Завершаемся\n");
    
    return 0;
}