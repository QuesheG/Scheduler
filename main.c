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
    for (int i = 0; i < 1; i++) {
        //ler prios;
        char filename[50];  // Cria um buffer para armazenar o nome do arquivo
        sprintf(filename, "teste%d.txt", i);  // Gera o nome do arquivo
        FILE * file = fopen(filename, "r");

        int credit = read_priority(i);
        
        //carregar programas para memoria
        scheduler->table[i] = load_program(file, i); //FIXME: atualmente essa linha ta dando uma string e pá, tem q mudar pra passar o arquivo e ir alocando o bcp na tabela

        //load_process(i, scheduler); // (quesheg) acho q poderíamos fazer esse primeiro, pra aí já ir alocando os arquivos na ordem q precisa e criar a fila ordenadamente
        //log_function(); Dps da pra fazer isso no final //TODO: logs -_-

        Node * process_node = create_node(credit);
        enqueue_ready(scheduler, process_node);

        printf("Carregando %s\n", scheduler->table[i][0]); //TODO: APAGAR DEPOIS
    }

    bool run = true;

    while (run) { //main while
        //Read the first process in queue
        int proc = get_process(scheduler);
        BCP * bcp = scheduler->table[proc];
        bcp->state = EXEC;
        bcp->credits--;

        update_blocked_queue(scheduler);

        printf("Executando %s", bcp[0]); //TODO: APAGAR DEPOIS
        for (int i = 0; i < scheduler->quantum; i++) {
            Comm command = line_processer(bcp);

            if(command == END){
                printf("%s terminado. X=%d Y=%d\n", bcp[0], bcp->regs.X, bcp->regs.Y); //TODO: APAGAR DEPOIS
                free(dequeue(scheduler->ready_queue)); //Removes from queue
                free(scheduler->table[proc]); //Removes from table
                break;
            }else if(command == IO){
                printf("E/S iniciada em %s\n", bcp[0]); //TODO: APAGAR DEPOIS
                printf("Interrompendo %s apos %d instrucoes\n", bcp[0], i+1); //TODO: APAGAR DEPOIS
                enqueue_blocked(scheduler, dequeue(scheduler->ready_queue)); //Removes from ready queue 
                bcp->state = BLOCK;
                bcp->regs.PC++;
                break;
            }

            bcp->regs.PC++;
        }

        if(bcp->state != BLOCK){
            bcp->state = READY;
            printf("Interrompendo %s apos 2 instrucoes\n", bcp[0]); //TODO: APAGAR DEPOIS
        }   

        //Escolhe proximo processo
        switch (next_process(scheduler)) {
            case -1: //No processes to run
                if(!scheduler->blocked_queue->head){
                    run = false;
                    //reload_all_processes();
                }else{
                    do {
                        update_blocked_queue(scheduler);
                    } while (next_process(scheduler) != -1);
                }               
                break;

            case 1: //Change the process
                enqueue_ready(scheduler, dequeue(scheduler->ready_queue));
                break;
                
            default: //Keep running the same process
                break;
        }

        printf("FIM");
    }

    return 0;
}

