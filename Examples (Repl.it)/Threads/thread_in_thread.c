#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


// Функция, выполняемая нитью
void* enclosed_thread_function(void* arg) {
    int number = * (int*) arg;


    switch(number) {
        case 0:
            //return-method
            printf("Return enclosed\n");
            return 0;
        case 1:
            //exit-method
            printf("Exit thread\n");
            pthread_exit(NULL);
        default:
            // wait-method
            sleep(2);
            printf("Enclosed: Я проснувся...\n");
            break;
    };

    return NULL;
}


// Функция, выполняемая нитью
void* thread_function(void* arg) {
    int number = * (int*) arg;

    // Создание нити
    pthread_t enclosed_thread;
    int result = pthread_create(&enclosed_thread, NULL, enclosed_thread_function, &number);
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    switch(number) {
        case 0:
        case 1:
            // Join_method
            pthread_join(enclosed_thread, NULL);
            printf("Thread join enclosed\n");
            break;
        case 2:
            //return-method
            printf("Return thread\n");
            return 0;
        case 3:
            //exit-method
            printf("Exit thread\n");
            pthread_exit(NULL);
        case 4:
        case 5:
            // wait-method
            sleep(4);
            printf("Thread: Я проснувся...\n");
            break;
    };
    
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int number = strtol(argv[1], NULL, 10); 
    /* main_thread - thread - enclosed_thread
    0 - Join - join - return
    1 - Join - join - exit
    2 - Join - return - wait
    3 - join - exit - wait
    4 - return - wait - wait
    5 - exit - wait - wait
    */
    int result;

    // Создание нити
    result = pthread_create(&thread, NULL, thread_function, &number);
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    // Ожидание завершения нити
    
    switch(number) {
        case 4:
            //return-method
            printf("Return main\n");
            return 0;
        case 5:
            //exit-method
            printf("Exit main\n");
            pthread_exit(NULL);
        default:
            // Join_method
            pthread_join(thread, NULL);
            printf("Main join thread\n");
            pthread_exit(NULL);
            break;
    };

    return 0;
}