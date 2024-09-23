#ifndef LIST_TABLE_H
#define LIST_TABLE_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "schedule.h"

typedef struct node {
    int val;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
} Queue;

Queue * create_queue(); //cria e retorna uma estrutura de fila
Node * create_node(int val); //cria e retorna uma estrutura de nó
bool is_empty(Queue *q); //retorna true se estiver vazio
bool enqueue_ready(Scheduler * scheduler, Node * new_node); //true se OK, false se erro
bool enqueue_blocked(Scheduler * scheduler, Node * new_node); //true se OK, false se erro
Node* dequeue(Queue *q); //retorna o primeiro nó na fila e remove ele. Retorna -1 se fila não existir ou estiver vazia
void update_blocked_queue(Scheduler * scheduler, bool update_last); //atualiza o iotimer e re-enfila os processos

#endif