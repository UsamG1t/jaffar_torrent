#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


// Функция, выполняемая нитью
void* thread_function(void* arg) {
    sleep(5);
    printf("Я проснувся...\n");
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int number = strtol(argv[1], NULL, 10);
    int result;

    // Создание нити
    result = pthread_create(&thread, NULL, thread_function, NULL);
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения нити
    // pthread_join(thread, NULL);

    switch(number) {
        case 1:
            //return-method
            printf("Return main\n");
            return 0;
        case 2:
            //exit-method
            printf("Exit main\n");
            pthread_exit(NULL);
    };
}