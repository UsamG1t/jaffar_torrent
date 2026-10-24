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
  
int main(int argc, char** argv) {  
    pthread_t thread;
    int num = strtol(argv[1], NULL, 10);
    int timer = strtol(argv[2], NULL, 10);
    int result;  
  
    // Создание нити
    result = pthread_create(&thread, NULL, thread_function, &num);
    
    if (result != 0) {
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
    sleep(timer);  
    printf("Главный поток завершен\n");  
    return 0;  
}