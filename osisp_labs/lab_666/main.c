#include "mainFunctions.h"                                                                                                                                 //     ./generator 4096 data.bin
                                                                                                                                                           //     ./reader data.bin
                                                                                                                                                           //     ./prog 4096 16 8 data.bin
                                                                                                                                                           //     ./reader data.bin

int size;
int blocks;
int threads;

pthread_barrier_t barrier;                     // Барьер для синхронизации потоков, мьютекс и текущая запись.
pthread_mutex_t mutex;                         // Мьютекс для обеспечения безопасности при доступе к ресурсам
index_record* cur;                             // Текущая запись

int main(int argc, char* argv[]) {
    // 1 - size (4096, 8192, 12288, 16384, ...
    // 2 - blocks,  2^n and > threads
    // 3 - threads, 8...8000
    // 4 - fname
    if(argc>1) {
        printf("Scanning params.\n");
        size = atoi(argv[1]);
        blocks = atoi(argv[2]);
        threads = atoi(argv[3]);               // Сканирование параметров.
        if(size%4096!=0) {
            printf("Wrong size, it should be multple 4096.\n");
            return 0;
        }
        if(threads<8 || threads > 8000) {
            printf("Wrong count of threads, form 8 to 8000.\n");
            return 0;
        }
        if(blocks%2!=0 || blocks < threads) {
            printf("Wrong count of block, it should be multiple 2 and more than threads count.\n");
            return 0;
        }

        if(pthread_barrier_init(&barrier, NULL, threads)) {
            printf("Error while creating barrier.\n");
            return 0;
        }

        if(pthread_mutex_init(&mutex, NULL)!=0) {
            printf("Error while creating mutex.\n");
            return 0;
        }

        //Создаём структурку 
        creatingData* cd = (creatingData*)malloc(sizeof(creatingData));

        cd->blockSize = size/blocks;        // Получение дополнительных параметров.
        cd->threads = threads;
        cd->fname = argv[4];

        pthread_t pthreadId;                // Создание потока номер 0.
        if(pthread_create(&pthreadId, NULL, openMem, cd) != 0) {
            printf("Error while creating 0 thread.\n");
            exit(0);
        }

        if(pthread_join(pthreadId, NULL)!=0) { // Ожидание завершения потока.
            printf("Error executing process.\n");
            exit(0);
        }

        pthread_barrier_destroy(&barrier);
        pthread_mutex_destroy(&mutex);
        printf("Work done.\n");
    } else
        printf("Not enough parameters.\n");

    return 0;
}








/*
Эта программа выполняет сортировку массива записей index_record 
по полю time_mark с использованием нескольких потоков:
--  Программа принимает параметры командной строки: размер файла (кратный 4096), 
                                                    количество блоков (четное число, больше количества потоков), 
                                                    количество потоков (от 8 до 8000) и имя файла.
-- Открывает указанный файл в двоичном режиме для чтения и записи.
-- Отображает содержимое файла в память с помощью mmap().
-- Создает барьер синхронизации и мьютекс для координации работы потоков.
-- Создает и запускает указанное количество потоков (кроме основного потока 0).
-- Каждый поток (включая основной) выполняет следующие действия:
                            Получает свой блок данных из отображенной в память области
                            Сортирует свой блок данных с помощью qsort()
                            Ожидает завершения всех потоков на барьере синхронизации
            После завершения всех потоков:
                             Отменяет отображение файла в память с помощью munmap()
                             Закрывает файл
-- Выводит значение time_mark каждой записи в отсортированном массиве.

Программа позволяет ускорить сортировку массива записей за счет 
распараллеливания работы на несколько потоков, 
используя механизмы синхронизации для координации их работы.
*/