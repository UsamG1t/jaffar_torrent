#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <unistd.h>  
  
time_t tmstart;  
//разделяемая переменная - число одновременно работающих читателей  
int rc = 0;  
//определение и инициализация мьютексов, состояние "открыт"  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t db = PTHREAD_MUTEX_INITIALIZER;  
  
//эмуляция работы нити – пауза в течение nsec секунд  
void do_some_work(int nsec) {  
    sleep(nsec);  
}  
  
void *reader(void *arg) {  
    int flag = 0;  
    pthread_mutex_lock(&mutex);  
    rc++;  
    if (rc == 1) { //флаг – данная нить захватила мьютекс  
        flag = 1;  
        pthread_mutex_lock(&db);  
    }  
    pthread_mutex_unlock(&mutex);  
    do_some_work(5);  
  
    pthread_mutex_lock(&mutex);  
    rc--;  
  
    while (flag) {  
        //цикл ожидания работает только в нити (первом читателе), которая захватила мьютекс db  
        if (rc == 0) {  
            //освобождение мьютекса db захватившей его нитью   
pthread_mutex_unlock(&db);  
            flag = 0;  
        } else { // ожидание окончания текущей очереди читателей для корректного (в этой же нити) освобождения db  
  
            //проверка состояния очереди читателей раз в секундe, неблокирующее ожиданиеpthread_mutex_unlock(&mutex);  
            sleep(1);  
            pthread_mutex_lock(&mutex);  
        }  
    }  
    pthread_mutex_unlock(&mutex);  
  
    printf("---Читатель #%lu завершился на %ld секунде\n", 
			    pthread_self(), time(0) - tmstart);  
    return NULL;  
}  
  
  
//реализация нити писателя  
void *writer(void *arg) {  
    pthread_mutex_lock(&db);  
    do_some_work(12);  
    pthread_mutex_unlock(&db);  
    printf("---Писатель #%lu завершился на %ld секунде\n", 
			    pthread_self(), time(0) - tmstart);  
    return NULL;  
}  
  
int main(void) {  
    int nr, nw, bFlag;  
    pthread_t th_id;  
    printf(">>Введите число читателей и писателей:\n");  
    scanf("%u %u", &nr, &nw);  
    tmstart = time(0); //начальная точка отсчёта в секундах  
    bFlag = nr <= 0; //значение 0-порождает читателя, 1-писателя  
    //цикл порождения читателей и писателей чередованием    
    while (nr > 0 || nw > 0) {  
        pthread_create(&th_id, NULL, bFlag ? writer : reader, NULL);  
        printf("++%s #%lu запущен на %ld секунде\n", 
		        bFlag ? "Писатель" : "Читатель", th_id, time(0) - tmstart);  
        //выбираем тип нити для запуска на следующей итерации чередованием (если не исчерпаны)  
        bFlag = bFlag ^ ((nr--, nr == 0) || (nw--, nw == 0));  
    }  
    for (;;) sleep(5); /*бесконечный цикл ожидания*/  
    return 0;  
}
