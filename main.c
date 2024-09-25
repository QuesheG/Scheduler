#include <stdio.h>
#include <stdlib.h>
#include "list_table.h"
#include "schedule.h"

int read_quantum(FILE *file) {
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
    //Checks quantum
    FILE *q = fopen("programas/quantum.txt", "r");
    if(!q) {
        printf("Could not read quantum value\n");
        printf("Quantum should be specified in quantum.txt\n");
        printf("Exiting...\n");
        return 0;
    }

    Scheduler * scheduler = create_scheduler(read_quantum(q));

    fclose(q);

    if(scheduler->quantum < 1) {
        printf("Quantum should be greater than 0\n");
        printf("Exiting...\n");
        return 0;
    }

    char logfilename[32];
    sprintf(logfilename, "log%02d.txt", scheduler->quantum);
    FILE *log = fopen(logfilename, "w");
    
    int total_instructions = 0;
    int total_exchanges = 0;
    int total_process= 0;

    //Load programs in memory
    load_all(scheduler, log);

    bool run = true;

    while (run) { //main while
        //Read the first process in queue
        int proc = get_process(scheduler);
        BCP * bcp = scheduler->table[proc];
        bcp->state = EXEC;
        bcp->credits--;

        fprintf(log, "Executando %s\n", bcp->content[0]);
        Comm command;

        int instructions_executed = 0; 

        for (int i = 0; i < scheduler->quantum; i++) {
            command = line_processer(bcp);

            instructions_executed++;

            if(command == END){
                fprintf(log, "%s terminado. X=%d. Y=%d\n", bcp->content[0], bcp->regs.X, bcp->regs.Y);
                free(dequeue(scheduler->ready_queue)); //Removes from queue
                free(scheduler->table[proc]); //Removes from table
                update_blocked_queue(scheduler, true);

                total_process++; //increase total number of completed processe

                break;
            }else if(command == IO){
                fprintf(log, "E/S iniciada em %s\n", bcp->content[0]);
                fprintf(log, "Interrompendo %s após %d instrucoes\n", bcp->content[0], i+1);
              

                bcp->state = BLOCK;
                bcp->regs.PC++;
                enqueue_blocked(scheduler, dequeue(scheduler->ready_queue)); //Removes from ready queue 
                break;
            }

            bcp->regs.PC++;
        }

        if(command != END){
            enqueue_ready(scheduler, dequeue(scheduler->ready_queue)); //Reinsert in the ready queue
            if(bcp->state != BLOCK){
                bcp->state = READY;
                fprintf(log, "Interrompendo %s após %d instrucoes\n", bcp->content[0], scheduler->quantum);
                update_blocked_queue(scheduler, true); 
            } else{
                update_blocked_queue(scheduler, false);
            }
        }
        total_instructions += instructions_executed;  //increase total number of instructions executed
        total_exchanges++; //increase total number of exchanges
        
        //Check reload credits
        if (bcp->credits <= 0) {
            Node * rproc = scheduler->ready_queue->head;
            Node * bproc = scheduler->blocked_queue->head;
            bool check = false;

            while (rproc) {
                if (scheduler->table[rproc->val]->credits > 0) {
                    check = true;
                    break;
                }
                rproc = rproc->next;
            }

            while (bproc) {
                if(scheduler->table[bproc->val]->credits > 0) {
                    check = true;
                    break;
                }
                bproc = bproc->next;
            }

            if (check == false) {
                reload_credits(scheduler);
            }
        }
        
        switch (next_process(scheduler)) {
            case -1: //No processes to run
                if(scheduler->blocked_queue->head){
                    do {
                        update_blocked_queue(scheduler, true);
                    } while (next_process(scheduler) == -1);
                }
                else {
                    run = false;
                }
                break;

            case 1: //Change the process
                enqueue_ready(scheduler, dequeue(scheduler->ready_queue));
                break;

            default: //Keep running the same process
                break;
        }
    }

    
    double media_trocas = (double) total_exchanges / total_process;
    double media_instrucoes = (double) total_instructions / total_exchanges;

    fprintf(log, "MEDIA DE TROCAS: %.2f\n", media_trocas); 
    fprintf(log, "MEDIA DE INSTRUÇÕES: %.2f\n", media_instrucoes);
    fprintf(log, "QUANTUM: %d\n", scheduler->quantum);
    
    fclose(log);
    free(scheduler);

    return 0;
}

