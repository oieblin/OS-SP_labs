#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct message
{
    uint8_t    type;   // тип сообщения
    uint16_t   hash;   // хэш сообщения
    uint8_t    size;   // размер сообщения
    uint8_t*   data;   //само сообщение
} mes_t;

typedef struct node // узел кольцевого буфера
{
    mes_t*  message;            
    struct node* next;
    struct node* prev;
} node_t;

typedef struct ring
{
    node_t* head;
    node_t* tail;
    size_t  added;
    size_t  deleted;
    size_t  cur;
    size_t  size;
} ring_t;

void push(node_t**, node_t**);
void pop(node_t**, node_t**);
void init_mes(mes_t*);
void print_mes(mes_t*);