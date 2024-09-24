#include <stdio.h>
#include <stdlib.h>
#include "list_table.h"
#include "schedule.h"

//ctrl+F programas

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
    //checa quantum
    FILE *q = fopen("programas/quantum.txt", "r");
    if(!q) {
        printf("Could not read quantum value\n");
        printf("Quantum should be specified in quantum.txt\n");
        printf("Exiting...\n");
        return 0;
    }

    //Cria escalonador
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

    //Carregar programas em memoria
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
        for (int i = 0; i < scheduler->quantum; i++) {
            command = line_processer(bcp);
            
            // fprintf(log, "Executando %s\n", bcp->content[bcp->regs.PC]);
            if(command == END){
                fprintf(log, "%s terminado. X=%d. Y=%d\n", bcp->content[0], bcp->regs.X, bcp->regs.Y);
                free(dequeue(scheduler->ready_queue)); //Removes from queue
                free(scheduler->table[proc]); //Removes from table
                update_blocked_queue(scheduler, true);
                break;
            }else if(command == IO){
                fprintf(log, "E/S iniciada em %s\n", bcp->content[0]);
                fprintf(log, "Interrompendo %s após %d instrucoes\n", bcp->content[0], i+1);
                //FIXME: ^ log files precisam incluir também número médio de trocas

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
                fprintf(log, "Interrompendo %s após %d instrucoes\n", bcp->content[0], scheduler->quantum); //FIXME: número de intruções errado
                update_blocked_queue(scheduler, true); 
            } else{
                update_blocked_queue(scheduler, false);
            }
        }

        //Escolhe proximo processo
        int res = next_process(scheduler);
        // fprintf(log, "Proximo processo: %d\n\n", res); FIXME: checar necessidade
        switch (res) {
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
        //TODO: reload_credits();
    }
    // fwrite("MEDIA DE TROCAS: %D\n", 0, 0, log); TODO: media de trocas e instruções
    // fwrite("MEDIA DE INSTRUÇÕES: %D\n", 0, 0, log);
    fprintf(log, "QUANTUM: %d\n", scheduler->quantum);
    
    fclose(log);
    free(scheduler);

    return 0;
}

