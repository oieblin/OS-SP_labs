#ifndef RING_H
#define RING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <string.h>
#include <stdint.h>

#define LEN_MESSAGE 260
#define OFFSET 4

typedef struct node_ring {  // узел
    int32_t shmid_curr;
    int32_t shmid_next;
    int32_t shmid_prev;
    uint8_t message[LEN_MESSAGE];
    bool flag_is_busy;
} node_ring;

typedef struct ring_shared_buffer {  // разделяемой памяти буфер
    int32_t shmid;                   // идентификатор сегмента, где находится буфер
    size_t consumed;                 // извлечённые добавленные
    size_t produced;                 // добавленные
    int32_t shmid_begin;             // первый узел 
    int32_t shmid_tail;              // полседний
} ring_shared_buffer;

node_ring* constructor_node();
ring_shared_buffer* constructor_buffer();
void append(ring_shared_buffer**);
void add_message(ring_shared_buffer*, const uint8_t*);
uint8_t* extract_message(ring_shared_buffer*);
void clear_shared_memory(ring_shared_buffer*);

#endif 
