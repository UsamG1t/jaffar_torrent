#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
// Функция, выполняемая дополнительной нитью  
void *worker_thread(void *arg) {  
    int id = *(int *) arg;  
    for (int i = 1; i <= 5; ++i) {  
        printf("Нить %d: Шаг %d\n", id, i);  
        sleep(1); // Симуляция работы  
    }  
    printf("Нить %d: Завершена.\n", id);  
    return NULL;  
}  
  
int main(void) {  
    pthread_t thread;  
    int thread_id = 1;  
  
    int mode;  
    if (!scanf("%d", &mode)) {  
        fprintf(stderr, "Неизвестный режим работы\n");  
        exit(1);  
    }  
  
    // Создание дополнительной нити, на примере работы которой мы увидем различия  
    if (pthread_create(&thread, NULL, worker_thread, &thread_id) != 0) {  
        perror("pthread_create");  
        exit(EXIT_FAILURE);  
    }  
  
    if (mode == 0) {  
        printf("Главный поток: Завершение с помощью return.\n");  
        return 0; // Завершение main через return  
    } else{  
        printf("Главный поток: Завершение с помощью pthread_exit.\n");  
        pthread_exit(NULL); // Завершение main через pthread_exit  
    }
}
