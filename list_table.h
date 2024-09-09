#include <stdio.h>
#include "schedule.h"

typedef struct bcp {
    char * address[22];
    State state;
    Registers regs; //Transferi o PC para cá
    int credits;
} BCP;

typedef struct node {
    int val;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
} Queue;

typedef struct Scheduler{
    struct BCP * table; //Array de ponteiros de BCP (tabela de processos)
    struct Queue * ready_queue;
    struct Queue * blocked_queue;
    int quantum;
} Scheduler;

Queue * create_queue(); //create and return a queue structure
char is_empty(Queue *q); //1 if empty, 0 otherwise
char enqueue(Queue *q, Node *val); //1 if okay, 0 if error
int dequeue(Queue *q); //val of element