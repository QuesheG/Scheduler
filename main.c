#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

int compare_res(char *a, char *b, size_t siz) {
    int res = 0;
    for (size_t i = 0; i < siz; i++) {
        if (a[i] != b[i]) break;
        else {
            res = 1;
        }
    }
    return res;
}

int read_val(FILE *file) {
    char c = 'A';
    char res[16];
    int i = 0;
    while(fread(&c, sizeof(char), 1, file)) {
        if(c == '\n') break;
        if(c < 48 || c > 57) break;
        res[i] = c;
        i+=1;
    }
    res[i] = '\0';
    return atol(res);
}

int main(void) {
    //checa quantum
    FILE *q = fopen("quantum.txt", "r");
    if(!q) {
        printf("Could not read quantum value\n");
        printf("Quantum should be specified in quantum.txt\n");
        printf("Exiting...\n");
        return 0;
    }

    //Cria escalonador
    Scheduler * scheduler = create_scheduler(read_val(q));

    if(scheduler->quantum < 1) {
        printf("Quantum should be greater than 0\n");
        printf("Exiting...\n");
        return 0;
    }

    //q1 -> prontos
    //q2 -> blocked
    //p* -> rodando

    //Carregar programas em memoria
    //Talvez a gente tenha que transformar isso em uma funcao
    for (int i = 1; i <= 10; i++) {
        //ler prios;
        char filename[50];  // Cria um buffer para armazenar o nome do arquivo
        sprintf(filename, "%d.txt", i);  // Gera o nome do arquivo
        FILE * file = fopen(filename, "r");
        
        int credit = read_priority(i);
        
        //carregar programas para memoria
        scheduler->table[i] = load_program(file, i, credit); //FIXME: atualmente essa linha ta dando uma string e pá, tem q mudar pra passar o arquivo e ir alocando o bcp na tabela

        //log_function(); Dps da pra fazer isso no final //TODO: logs -_-

        Node * process_node = create_node(i);
        enqueue_ready(scheduler, process_node);

        printf("Carregando %s\n", scheduler->table[i]->content[0]); //TODO: APAGAR DEPOIS
    }

    bool run = true;

    while (run) { //main while
        Node * t = scheduler->ready_queue->head;
        Node * b = scheduler->blocked_queue->head;
        printf("Ordem na filinha de prontinhos: \n");
        while (t) {
            printf("%d(%d) ", t->val, scheduler->table[t->val]->credits);
            t = t->next;
        }
        printf("\nOrdem na filinha de bloqueados: \n");
        while (b) {
            printf("%d(%d) ", b->val, scheduler->table[b->val]->io_timer);
            b = b->next;
        }
        
        printf("\n\n");

        //Read the first process in queue
        int proc = get_process(scheduler);
        BCP * bcp = scheduler->table[proc];
        bcp->state = EXEC;
        bcp->credits--;

        printf("Processo: %d\n", proc);
        Comm command;
        for (int i = 0; i < scheduler->quantum; i++) {
            command = line_processer(bcp);
            
            printf("Executando %s\n", bcp->content[bcp->regs.PC]); //TODO: APAGAR DEPOIS
            if(command == END){
                printf("%s terminado. X=%d Y=%d\n", bcp->content[0], bcp->regs.X, bcp->regs.Y); //TODO: APAGAR DEPOIS
                free(dequeue(scheduler->ready_queue)); //Removes from queue
                free(scheduler->table[proc]); //Removes from table
                update_blocked_queue(scheduler, true);
                break;
            }else if(command == IO){
                printf("E/S iniciada em %s\n", bcp->content[0]); //TODO: APAGAR DEPOIS
                printf("Interrompendo %s apos %d instrucoes\n", bcp->content[0], i+1); //TODO: APAGAR DEPOIS

                bcp->state = BLOCK;
                bcp->regs.PC++;
                enqueue_blocked(scheduler, dequeue(scheduler->ready_queue)); //Removes from ready queue 
                break;
            }

            bcp->regs.PC++;
        }

        if(command != END){
            enqueue_ready(scheduler, dequeue(scheduler->ready_queue)); //Reinsert in the
            if(bcp->state != BLOCK){
                bcp->state = READY;
                printf("Interrompendo %s apos quantum instrucoes\n", bcp->content[0]); //TODO: APAGAR DEPOIS
                update_blocked_queue(scheduler, true); 
            } else{
                update_blocked_queue(scheduler, false);
            }
        }

        //Escolhe proximo processo
        int res = next_process(scheduler);
        printf("Proximo processo: %d\n\n", res);
        switch (res) {
            case -1: //No processes to run
                if(scheduler->blocked_queue->head){
                    do {
                        update_blocked_queue(scheduler, true);
                    } while (next_process(scheduler) == -1);
                }               
                break;

            case 1: //Change the process
                enqueue_ready(scheduler, dequeue(scheduler->ready_queue));
                break;
                
            default: //Keep running the same process
                break;
        }
        //TODO: reload_credits();
    }
    return 0;
}

