#include <stdio.h>
#include "schedule.h"

typedef struct table {
    FILE * PC;
    State estado;
    Registradores Regs;
    
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

char is_empty(Queue *q); //1 if empty, 0 otherwise
char enqueue(Queue *q, Node *val); //1 if okay, 0 if error
int dequeue(Queue *q); //val of element