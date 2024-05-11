#include "ring.h"

void push(node_t** head, node_t** tail) 
{
    if (*head != NULL) 
    {
        node_t *temp = (node_t*)malloc(sizeof(node_t));
        temp->message = (mes_t*)malloc(sizeof(mes_t));
        init_mes(temp->message);
        temp->next = *head;
        temp->prev = *tail;
        (*tail)->next = temp;
        (*head)->prev = temp;
        *tail = temp;
    } 
    else 
    {
        *head = (node_t*)malloc(sizeof(node_t));
        (*head)->message = (mes_t*)malloc(sizeof(mes_t));
        init_mes((*head)->message);
        (*head)->prev = *head;
        (*head)->next = *head;
        *tail = *head;
    }
}

void pop(node_t** head, node_t** tail) 
{
    if (*head != NULL) 
    {
        if (*head != *tail) 
        {
            node_t *temp = *head;
            (*tail)->next = (*head)->next;
            *head = (*head)->next;
            (*head)->prev = *tail;
            free(temp);
        } else 
        {
            free(*head);
            *head = NULL;
            *tail = NULL;
        }
    }
}

void init_mes(mes_t* message) // инициализация рандомного сообщения
{
    const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    message->type = 0;
    message->hash = 0;
    message->size = rand() % 257;
    message->data = (uint8_t*)malloc(message->size * sizeof(uint8_t));
    
    for (size_t i = 0; i < message->size; i++) 
    {
        message->data[i] = letters[rand() % 53];
        message->hash += message->data[i];
    }
    message->hash = ~message->hash;
}

void print_mes(mes_t* mes) 
{
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->type, mes->hash, mes->size);
    for(size_t i = 0; i<mes->size; i++)
        printf("%c", mes->data[i]);
    printf("\n");
}