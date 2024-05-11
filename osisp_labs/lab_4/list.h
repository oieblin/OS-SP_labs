#ifndef LIST_H
#define LIST_H

#include <sys/types.h>

typedef struct node_list {
    char index_worker;                                         // индекс рабочего процесса
    pid_t pid;                                                 // идентификатор
    struct node_list* next;                                    // указатель на следующий узел
}node_list;

node_list* constructor_list(pid_t, char);                      // создание
void push_list(node_list**, pid_t, char);                      // добавление нового в конец списка
void display_list(const node_list*);                           // инфа о всех узлах списка
pid_t pop_list(node_list**);                                   // удаление последнего узла
void clear(node_list**);                                       // очистить список
pid_t erase_list(node_list**, size_t tum);                     // удалить по номеру

#endif //LIST_H

