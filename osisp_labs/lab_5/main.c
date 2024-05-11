//Программа позволяет создавать потоки-производители (producer) и потоки-потребители 
//(consumer), которые могут добавлять и извлекать элементы из кольцевого буфера. 
//Кольцевой буфер реализован с помощью структуры ring_t, которая содержит информацию о
// размере буфера, количестве добавленных и извлеченных элементов, а также указатели на
// начало и конец буфера.
#include "ring.h"
#include "routines.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>

#define DEFAULT_RING_SIZE 10
#define MAX_THREADS_COUNT 50

ring_t* ring;

int main()
{
    sem_unlink("/producer");  // синхронизация потоков-производителей
    sem_unlink("/consumer");  // синхронизация потоков-потребителей
    sem_unlink("/mutex");     // синхронизация доступа к общим данным
    // инициализация семафоров
    sem_t* producer = sem_open("/producer", O_CREAT, 0644, 1); // O_CREAT - создание семафора, если он ещё не существует
    sem_t* consumer = sem_open("/consumer", O_CREAT, 0644, 1); // 0644: 0 - тип объекта(семафор), 6 - права владельца(чтение/запись), 4 - права группы(чтение), 4 - право остальных(чтение)
    sem_t* mutex    = sem_open("/mutex", O_CREAT, 0644, 1);    // 1 - начальное значение семафора(устанавливает количество потоков, которые могут получить доступ к данным ресурсам, соотвественно 1)

    pthread_t threads[MAX_THREADS_COUNT];                           // массив для хранения идентификаторов потоков
    size_t producer_count = 0;
    size_t consumer_count = 0;
    size_t threads_count  = 0;
    char input[2];

    ring       = (ring_t*)calloc(1, sizeof(ring_t)); // инициализация кольцевого буфера
    ring->size = DEFAULT_RING_SIZE;

    while (1)
    {
        printf("-------------------------------------------\n");
        printf("input one of chars: + - p c s q\n");
        printf("-------------------------------------------\n");
        scanf("%s", input);

        switch (input[0])
        {
        case 'p': // создание потока-производителя
        {
            pthread_t producer_thread_id;
            pthread_create(&producer_thread_id, NULL, producer_routine, NULL); // создать поток-производитель
            threads[threads_count++] = producer_thread_id; // сохранить идентификатор потока
            producer_count++;     // увеличить счётчик производителей
            break;
        }        
        case 'c': // создание потока-потребителя
        {
            pthread_t consumer_thread_id;
            pthread_create(&consumer_thread_id, NULL, consumer_routine, NULL); // созданть поток-потребитель
            threads[threads_count++] = consumer_thread_id; // сохранить идентификатор в массив
            consumer_count++; // увеличсить счётчик потребителей
            break;
        }
        case 's': // инфа о кольцевом буфере, кол-во потребителей и производителей
        {
            printf("\n=====================\n");
            printf("Added: %ld\nGetted: %ld\nProducers count: %ld\nConsumers count: %ld\nCurrent size: %ld\n", 
                                                                                ring->added, 
                                                                                ring->deleted, 
                                                                                producer_count, 
                                                                                consumer_count,
                                                                                ring->cur);
            printf("=====================\n\n");                                                          
            break;
        }
        case '+': // увеличть кольцевой буфер на 1 элемент
        {
            sem_wait(mutex);             // mutex блокируется
            ring->size++;                     // увелисить размер кольцевого буфера
            sem_post(mutex);              // mutex разблокируется
            break;
        }
        case '-': // уменьнить кольцнвой буфер на 1 элемент/ удалить элемент
        {
            sem_wait(mutex);
            if (ring->size > 0) 
            {
                ring->size--;
                if (ring->cur > ring->size)
                {
                    pop(&ring->head, &ring->tail);
                    ring->cur--;
                } 
            }
            else printf("\nRING IS EMPTY\n");
            sem_post(mutex);
            break;
        }
        case 'q': // завершить программу
        {
            while (threads_count != 0)
            {
                pthread_kill(threads[threads_count - 1], SIGUSR1);  // остановка всех потоков
                pthread_join(threads[threads_count - 1], NULL);   // ожидание завершения потоков
                threads_count--;
            }
            // закрытие семафоров
            sem_close(producer);
            sem_close(consumer);
            sem_close(mutex);
            // удаление семафоров 
            sem_unlink("/producer");
            sem_unlink("/consumer");
            sem_unlink("/mutex");
            
            return 0;
        }
        default:
            break;
        }
    }
    return 0;
}