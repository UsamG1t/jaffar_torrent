#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
// Функция, выполняемая дополнительной нитью  
void *worker_thread(void *arg) {  
    int steps = *(int *) arg;  
    for (int i = 0; i < steps; ++i) {  
        printf("Нить %ld: Шаг %d\n", pthread_self(), i);  
        sleep(1); // Симуляция работы  
    }  
    printf("Нить %ld: Завершена.\n", pthread_self());  
    return NULL;
}  
  
int main(int argc, char** argv) {  
    pthread_t thread;
    int steps = strtol(argv[1], NULL, 10);
    int timer = strtol(argv[2], NULL, 10);
    int result;

    // Создание нити
    result = pthread_create(&thread, NULL, worker_thread, &steps);
    
    if (result != 0) {
        fprintf(stderr, "Ошибка создания нити\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < timer; ++i) {  
        printf("Нить %ld: Ожидание %d\n", pthread_self(), i);  
        sleep(1); // Симуляция ожидания  
    }

    pthread_cancel(thread);

    // Ожидание завершения нити
    pthread_join(thread, NULL);
    printf("Нить завершена\n");
    return 0;
}
