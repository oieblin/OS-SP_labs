#include "routines.h"
#include "ring.h"

#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

extern ring_t* ring;

_Thread_local int consumer_continue = 1;

int threads_counter = 0;

void cons_stop_handler(int signo)  // для остановки потока
{
    if (signo != SIGUSR1)
        return;

    consumer_continue = 0;
}

#define SLEEP_TIME 2

void* consumer_routine()  // поток-потребитель: извлекает сообщения из кольцевого буфера
{
    signal(SIGUSR1, cons_stop_handler);// остановить, если поток получит сигнал SIGUR1
    // инициализация и открытие семафоров: открывается доступ к ресурсам, защищёнными этими семафорами
    sem_t* consumer = sem_open("/consumer", 0);
    sem_t* mutex    = sem_open("/mutex", 0);

    while (consumer_continue) 
    {
        // остановка семафоров
        sem_wait(consumer);                                   
        sem_wait(mutex);

        if (ring->cur > 0) // если есть место в буфере, извлечь сообщение
        {  
            mes_t* temp = ring->head->message;

            pop(&ring->head, &ring->tail);

            ring->deleted++;
            ring->cur--;

            printf("--Ejected %ld message:\n", ring->deleted);
            print_mes(temp);

            free(temp->data);
            free(temp);
        } 
        // разблокировать семафоры
        sem_post(mutex);                           
        sem_post(consumer);

        sleep(SLEEP_TIME); // прервать поток на SLEEP_TIME  секунд
    }
    // закрыть семафоры
    sem_close(consumer);                                    
    sem_close(mutex);
    // инфа о завершении потока-потребителя
    printf("\nConsumer %d has finished\n", (threads_counter++ + 1));

    return NULL;
}

void* producer_routine() // поток-производитель: добавляет сообщения в кольцевой буфер
{
    signal(SIGUSR1, cons_stop_handler); // остановить, если поток получит сигнал SIGUR1
    // инициализация и открытие семафоров: открывается доступ к ресурсам, защищёнными этими семафорами
    sem_t* producer = sem_open("/producer", 0);
    sem_t* mutex    = sem_open("/mutex", 0);

    while (consumer_continue) 
    {
        sem_wait(producer);                                   
        sem_wait(mutex);

        if (ring->cur < ring->size) 
        {  
            push(&ring->head, &ring->tail);

            ring->added++;
            ring->cur++;

            printf("--Append %ld message:\n", ring->added);
            print_mes(ring->tail->message);
        }

        sem_post(mutex);                                       
        sem_post(producer);

        sleep(SLEEP_TIME);
    }

    sem_close(producer);                                    
    sem_close(mutex);

    printf("\nProducer %d has finished\n", (threads_counter++ + 1));

    return NULL;
}