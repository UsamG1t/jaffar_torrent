#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Функция, выполняемая внутри нити
void* thread_function_in(int ret_exit) {
    switch (ret_exit) {
        case 1:
            //return_in-method
            printf("return_in-method\n");
            return NULL;
        case 2:
            //exit_in-method
            printf("exit_in-method\n");
            pthread_exit(NULL);
    }
}

// Функция, выполняемая нитью
void* thread_function(void* arg) {
    int num = * (int*) arg;

    switch(num) {
        case 1:
            // return-return-method
            printf("Return-return-method before\n");
            thread_function_in(1);
            printf("Return-return-method after\n");
            return NULL;
        case 2:
            // exit-return-method
            printf("Exit-return-method before\n");
            thread_function_in(1);
            printf("Exit-return-method after\n");
            pthread_exit(NULL);
        case 3:
            // return-exit-method
            printf("Return-exit-method before\n");
            thread_function_in(2);
            printf("Return-exit-method after\n");
            return NULL;
        case 4:
            // exit-exit-method
            printf("Exit-exit-method before\n");
            thread_function_in(2);
            printf("Exit-exit-method after\n");
            pthread_exit(NULL);
        case 5:
            // exit-process-method
            printf("Exit-process-method\n");
            exit(0);
    };

    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int number = strtol(argv[1], NULL, 10);
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