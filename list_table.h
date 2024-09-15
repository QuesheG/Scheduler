#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
} Queue;

Queue * create_queue(); //create and return a queue structure
Node * create_node(int val); //create and return a node structure
bool is_empty(Queue *q); //true if empty
bool enqueue_ready(Scheduler * scheduler, Node * new_node); //true if okay, false if error
bool enqueue_blocked(Scheduler * scheduler, Node * new_node); //true if okay, false if error
Node* dequeue(Queue *q); //return the first node in queue and removes it from queue. return -1 if queue dont exist or is empty
void update_blocked_queue(Scheduler * scheduler); //Updates the iotimer and re enqueues the processes