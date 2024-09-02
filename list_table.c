#include "list_table.h"

int is_empty(Queue *q){ //1 if empty, 0 otherwise
    if(q->head) return 1;
    return 0;
}

int enqueue(Queue *q, Node *val) { //1 if okay, 0 if error
    if(!q) return 0;
    if(q->head) {
        q->end->next = val;
        val->bef = q->end->next;
        q->end = val;
        return 1;
    }
    else {
        q->head = val;
        q->end = val;
        return 1;
    }
    return 0;
}

int dequeue(Queue *q) {
    //termianr
}