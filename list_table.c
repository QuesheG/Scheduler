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
    Queue * q = scheduler->ready_queue;
    if(!q) return false;
    if(!scheduler) return false;

    
    if (is_empty(q)) {
        q->head = new_node;
        return true;
    }
    
    Node * p = q->head;
    Node * last_node_visited = NULL; 
    
    while(p){
        int new = scheduler->table[new_node->val]->credits;
        int current = scheduler->table[p->val]->credits;

        //if new_process' credits >= current_process add in position
        if (new >= current) {
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
    last_node_visited->next = new_node;
    return true;
}

Node * dequeue(Queue *q) {
    if(!q) return NULL;
    if(!q->head) return NULL;

    Node * process = q->head;
    q->head = q->head->next;
    process->next = NULL;

    return process;
}

bool enqueue_blocked(Scheduler * scheduler, Node * new_node){
    Queue * q = scheduler->blocked_queue;
    Node * p = q->head;
    
    //Set the io timer in process
    scheduler->table[new_node->val]->io_timer = 2;

    while(p){ 
        if(!p->next){
            //adds the new_node in the end
            p->next = new_node;
            return true;
        }
        p = p->next;
    }
    q->head = new_node;
    return false;
}

void update_blocked_queue(Scheduler * scheduler, bool update_last){
    Queue * q = scheduler->blocked_queue;
    if(!q) return;
    if(!q->head) return;

    Node * p = q->head;

    while (p) {
        if(!update_last && !p->next){ //skips the last process
            break;
        }

        scheduler->table[p->val]->io_timer--; //decrease the io_timer
        if(scheduler->table[p->val]->io_timer == 0){
            printf("Retorna processo %d para fila de prontos\n", p->val);
            scheduler->table[p->val]->state = READY;
            
            Node * process = dequeue(q);
            enqueue_ready(scheduler, process);
            
            printf("\n");
            p = q->head;
            continue;
        }

        printf("Processo %d esta com %d tempo\n", p->val, scheduler->table[p->val]->io_timer);
        p = p->next;
    }
    
}