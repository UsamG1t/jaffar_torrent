#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  

// Функция, выполняемая нитью  
void *thread_function(void *arg) {  
    int *num = (int *) arg;  
    printf("Отсоединённая нить получила число: %d\n", *num);  
    sleep(2); // Симуляция работы  
    printf("Отсоединённая нить завершена\n");  
    pthread_exit(NULL);  
}  

int main(void) {  
    pthread_t thread;  
    int number = 42;  

    // Создание нити  
    if (pthread_create(&thread, NULL, thread_function, &number) != 0) {  
        fprintf(stderr, "Ошибка создания нити\n");  
        exit(EXIT_FAILURE);  
    }  

    // Отсоединение нити  
    if (pthread_detach(thread) != 0) {  
        fprintf(stderr, "Ошибка отсоединения нити\n");  
        exit(EXIT_FAILURE);  
    }  

    printf("Главный поток продолжает работу без ожидания нити\n");  
    // Здесь можно выполнить другую работу  
    // sleep(3); // Даем отсоединённой нити время завершиться  
    printf("Главный поток завершен\n");  
    return 0;  
}