#include <stdio.h>
#include <stdbool.h>

typedef struct node {
    int val;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
} Queue;

Queue * create_queue(); //create and return a queue structure
bool is_empty(Queue *q); //true if empty
bool enqueue(Queue *q, Node *val); //true if okay, false if error
int dequeue(Queue *q); //val of element