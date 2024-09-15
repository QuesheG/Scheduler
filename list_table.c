#include "list_table.h"
#include "schedule.h"

bool is_empty(Queue *q){ //true if empty
    if(!q->head) return true;
    return false;
}

Queue * create_queue(){
    Queue * q = (Queue *)malloc(sizeof(Queue));
    q->head = NULL;
    return q;
}

Node * create_node(int val){
    Node * node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->val = val;
    return node;
}

bool enqueue_ready(Scheduler * scheduler, Node * new_node) { //true if okay
    if(!q) return false;
    if(!scheduler) return false;

    Queue * q = scheduler->ready_queue;
    if (is_empty(q)) {
        q->head = new_node;
        return true;
    }
    
    Node * p = q->head;
    Node * last_node_visited = NULL; 
    while(p){
        //if new_process' credits >= current_process add in position
        if (scheduler->table[new_node->val]->credits >= scheduler->table[p->val]->credits) {
            new_node->next = p;
            if(last_node_visited == NULL){
                q->head = new_node;
            }else{
                last_node_visited->next = new_node;
            }
            return true;
        }

        last_node_visited = p;
        p = p->next;
    }
    //caso seja o ultimo da fila
    p->next = new_node;
    return true;
}

Node * dequeue_ready(Queue *q) { 
    if(!q->head) return NULL;
    if(!q) return NULL;

    Node * process = q->head;
    q->head = q->head->next;
    process->next = NULL;

    return process;
}

bool enqueue_blocked(Scheduler * scheduler, int val){
    Node * new_node = create_node(val);
    Node * p = q->head;
    
    //Set the io timer in process
    scheduler->table[val]->io_timer = 2;

    while(p){ 
        if(!p->next){
            //adds the new_node in the end
            p->next = new_node;
            return true;
        }
        p = p->next;
    }
    return false;
}

int dequeue_blocked(Queue *q) { 
    if(!q->head) return -1;
    if(!q) return -1;

    int val = q->head->val;
    
    Node * aux = q->head;
    q->head = q->head->next;

    free(aux);
    return val;
}

