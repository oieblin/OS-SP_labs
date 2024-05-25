#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>
#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
// структура для передачи аргументов в поток
typedef struct {
    index_record * buf;
    int blockSize;
    int threadNum;
} threadArgs;
// структура для хранения параметров создания данных
typedef struct {
    int blockSize;
    int threads;
    char* fname;
} creatingData;
// Глобальные переменные для размера, блоков, потоков, барьера и мьютекса
extern int size;
extern int blocks;
extern int threads;
extern pthread_barrier_t barrier;
extern pthread_mutex_t mutex;
extern index_record* cur;

int compare(const void* a, const void* b);
void* sorting(void* threadA);
void *openMem(void* data);
