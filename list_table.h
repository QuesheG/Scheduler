#include <stdio.h>
#include "schedule.h"

typedef struct bcp {
    FILE * reference;
    State state;
    Registers regs; //Transferi o PC para cá
    int creditos;
} BCP;

typedef struct node {
    int val;
    struct node *bef;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
    Node *end;
} Queue;

typedef struct Scheduler{
    struct BCP * table; //Array de ponteiros de BCP (tabela de processos)
    struct Queue * ready_queue;
    struct Queue * blocked_queue;
} Scheduler;

char is_empty(Queue *q); //1 if empty, 0 otherwise
char enqueue(Queue *q, Node *val); //1 if okay, 0 if error
int dequeue(Queue *q); //val of element