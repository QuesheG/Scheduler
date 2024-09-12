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

bool enqueue(Queue *q, int val, Scheduler * scheduler) { //true if okay
    /*
    if(!q) return 0;
    if(q->head) {
        q->end->next = process;
        process->bef = q->end->next;
        q->end = process;
        return 1;
    }
    else {
        q->head = process;
        q->end = process;
        return 1;
    }
    return 0;
    */
    //Do jeito que ficou e so uma fila normal, nao sei qual o proposito geral dessa funcao quando voce pensou
    //mas acho que e melhor fazer um que dado a fila e um processo a ser colocado, ja colocasse diretamente
    //na posicao certa
    //Ai tambem nao precisa ordenar todos eles antes de ir "carregando na memoria"
    if(!q) return false;
    if(!scheduler) return false;

    Node * new_node = create_node(val);
    if (is_empty(q)) {
        q->head = new_node;
        return true;
    }
    
    Node * p = q->head;
    Node * last_node_visited = NULL; 
    while(p){
        if (scheduler->table[val] >= scheduler->table[p->val]) {
            new_node->next = p;
            if(last_node_visited = NULL){
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

int dequeue(Queue *q) { 
    if(!q) return -1;
    if (!q->head) {
        return -1;
    }
}

