#include <string.h>
#include "schedule.h"
#include "list_table.h"

BCP * createBCP(State state, int credits){
    BCP * bcp = (BCP *)malloc(sizeof(BCP));
    bcp->content = (char **)malloc(sizeof(char *) * 22);
    bcp->state = state;
    bcp->regs.X = 0;
    bcp->regs.Y = 0;
    bcp->regs.PC = 1;
    bcp->credits = credits;
    bcp->io_timer = 2;
    return bcp;
}

Scheduler * create_scheduler(int quantum){
    Scheduler *s = (Scheduler *)malloc(sizeof(Scheduler));

    s->table = (BCP **)malloc(sizeof(BCP*) * 11);
    s->blocked_queue = create_queue();
    s->ready_queue = create_queue();
    s->quantum = quantum;
    s->io_time = 2;

    return s;
}

//parser para cada linha do arquivo texto recebido
Comm line_processer(BCP * bcp) {
    /************************
    * FORMATO DOS COMANDOS: *
    * COM       -> COM      *
    * X|Y=INT   -> ATRIB    *
    * E/S       -> IO       *
    * SAIDA     -> END      *
    * COMANDOS PODEM SER    *
    * IDENTIFICADOS COM     *
    * APENAS 1 CARACTERE    *
    ************************/

    int line = bcp->regs.PC;
    switch (bcp->content[line][0])
    {
        case 'C':
            return COM;
        case 'S':
            //Close process
            return END;
        case 'E':
            //Change queue
            return IO;
        case 'X':
            bcp->regs.X = atoi(&(bcp->content[line][2]));
            return ATRIB;
        case 'Y':
            bcp->regs.Y = atoi(&(bcp->content[line][2]));
            return ATRIB;
        
        default:
            break;
    }

}

BCP * load_program(FILE * file, int credits){
    //nota: as linhas "úteis" serão apenas de 1~n, com a linha 0 sendo usada para referenciar o nome do processo
    char * c = (char *)malloc(sizeof(char) * 25);
    BCP * bcp = createBCP(READY, credits);
    int i = 0;
    int pc = 0;
  
    while(fread(c + i, sizeof(char), 1, file)) {
        if(*(c + i) == '\n') {
            *(c + i) = '\0';
            bcp->content[pc] = (char *)malloc(sizeof(char) * (i + 1));
            strcpy(bcp->content[pc], c); 
            i = 0;
            pc += 1;
            continue;
        }
        i++;
    }
    if(i > 0) {
        *(c + i) = '\0';
        bcp->content[pc] = (char *)malloc(sizeof(char) * (i + 1));
        strcpy(bcp->content[pc], c);
    }
    free(c);
    return bcp;
}

int read_priority(int proc_num) {
    FILE *arq_prio = fopen("programas/prioridades.txt", "r");
    if (!arq_prio) {
        printf("Error opening priority file\n");
        return -1;
    }

    int priority = -1;
    int line_counter = 0;  // Inicia a contagem de linhas em 0
    char linha[100];  // Buffer para armazenar a linha

    // Percorre o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arq_prio)) {
        // Verifica se a linha atual é a que corresponde ao número do processo
        if (line_counter == proc_num - 1) {
            if (sscanf(linha, "%d", &priority) != 1) {
                printf("Error read priority\n");
                priority = -1;
            }
            break;  // Sai do loop depois de encontrar e ler a linha correta
        }
        line_counter++;
    }

    fclose(arq_prio);
    return priority;
}

int get_process(Scheduler * scheduler){
    return scheduler->ready_queue->head->val;
}

//Return -1 if there is no process to run, 0 if the context isnt changed, 1 if context will be changed
int next_process(Scheduler * scheduler){
    if (!scheduler->ready_queue->head){
        return -1;
    }

    if (!scheduler->ready_queue->head->next){ 
        return 0;
    }

    int actual_process = scheduler->ready_queue->head->val;
    int next_process = scheduler->ready_queue->head->next->val;

    
    //if the actual credits are lower than the next credits, reenqueue the actual process
    if(scheduler->table[actual_process]->credits < scheduler->table[next_process]->credits){
        return 1;
    }

    return 0;
}

bool load_all(Scheduler * scheduler, FILE * log) {
    for (int i = 1; i <= 10; i++) {
        char filename[32];
        sprintf(filename, "programas/%02d.txt", i);  
        FILE * file = fopen(filename, "r");

        if(!file) return false;
        
        int credit = read_priority(i);
        
        //Load programs in memory
        scheduler->table[i] = load_program(file, credit);

        Node * process_node = create_node(i);
        enqueue_ready(scheduler, process_node);

        fprintf(log, "Carregando %s\n", scheduler->table[i]->content[0]);
    }
    return true;
}

void reload_credits(Scheduler * scheduler){
    if (scheduler->ready_queue->head) {
        Node * p = scheduler->ready_queue->head;
        Node * last_node = NULL;

        while (p){
            scheduler->table[p->val]->credits = read_priority(p->val);
            p = p->next;
        }

        p = scheduler->ready_queue->head;

        while (p && p->next) {
            //If the process' credits < next process' credits change positions
            if(scheduler->table[p->val]->credits < scheduler->table[p->next->val]->credits){
                if (last_node == NULL) {
                    Node* aux = p->next->next;
                    scheduler->ready_queue->head = p->next;
                    scheduler->ready_queue->head->next = p;
                    p->next = aux;
                }else{
                    last_node->next = p->next;
                    p->next = p->next->next;
                    last_node->next->next = p;
                }
                last_node = NULL;
                p = scheduler->ready_queue->head;
                continue;
            }
            last_node = p;
            p = p->next;
        }        
    }
    if (scheduler->blocked_queue->head) {
        Node * p = scheduler->blocked_queue->head;
        while (p) {
            scheduler->table[p->val]->credits = read_priority(p->val);
            p = p->next;
        }
    }
}