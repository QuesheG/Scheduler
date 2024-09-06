#include "list_table.h"

int is_empty(Queue *q){ //1 if empty, 0 otherwise
    if(!q->head) return 1;
    return 0;
}

int enqueue(Queue *q, Node *process, /*BCP * bcp (?)*/ ) { //1 if okay, 0 if error
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

    if (is_empty(q)) {
        q->head = process;
        return 1;
    }
    
    Node * p = q->head;
    while(p){
        if (bcp->credits[process->val] >= bcp->credits[p->val]) { //algo assim
            p->bef->next = process;
            process->next == p;
            process->bef == p->bef;
            p->bef = process;
            return 1;
        }
        
        p = p->next;
    }
    //caso seja o ultimo da fila
    p->next = process;
    process->bef = p;
    return 1;
}

int dequeue(Queue *q) {
    if (!q->head) {
        Node * aux = q->head.next;
        int val = q->head.val;

        if (!q->head->next) {
           q->head->next->bef = NULL; // isso ta certo ?
        }
        
        q->head = aux;
        return val;
    }
}

