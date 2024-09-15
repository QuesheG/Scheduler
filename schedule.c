#include "schedule.h"

BCP * createBCP(State state, int credits){
    //lines to be added in file_reading function;
    BCP * bcp = (BCP *)malloc(sizeof(BCP));
    bcp->state = state;
    bcp->regs.X = 0;
    bcp->regs.Y = 0;
    bcp->regs.PC = 0;
    bcp->credits = credits; //FIXME: esse aqui tem q mudar pq nem sei oq significa pra falar a real (luiyu reply)-> credits sao os creditos de prioridade a serem gastados
    bcp->io_timer = 0;
    return bcp;
}

Scheduler * create_scheduler(int quantum){
    Scheduler *s = (Scheduler *)malloc(sizeof(Scheduler));

    s->table = (BCP **)malloc(sizeof(BCP*) * 10);
    s->blocked_queue = create_queue();
    s->ready_queue = create_queue();
    s->quantum = quantum;
    s->io_time = 2;

    return s;
}

//parser para cada linha do arquivo texto recebido
Comm line_reader(FILE *file) {  
    //Eu (luiyu) pergunto-me se nao é melhor ler um file (programa) por vez e ja ir armazendando os comandos nos arrays
    //Eu (quesheg) digo que n sei de porra nenhuma
    Comm found = END;
    char c[3] = {'A', 'A'};
    char atrib1[2] = {'X', '='};
    char atrib2[2] = {'Y', '='};
    char exit[3] = {'S', 'A'};
    char com[3] = {'C', 'O'};
    char io[3] = {'E', '/'};
    /************************
    * FORMATO DOS COMANDOS: *
    * COM       -> COM      *
    * X|Y=INT   -> ATRIB    *
    * E/S       -> IO       *
    * SAIDA     -> END      *
    * COMANDOS PODEM SER    *
    * IDENTIFICADOS COM     *
    * APENAS 2 CARACTERES   *
    ************************/
    fread(c, sizeof(char), 2, file);
    if(compare_res(c, com,  2)) return COM;
    if(compare_res(c, io,   2)) return IO;
    if(compare_res(c, atrib1, 2)) {
        int val = read_val(file);
        //TODO: armazenar valor recebido em X;
        return ATRIB;
    }
    else if(compare_res(c, atrib2, 2)) {
        int val = read_val(file);
        //TODO: armazenar valor recebido em Y;
        return ATRIB;
    }
    if(compare_res(c, exit, 2)) return END;

}

void cpystr(char * dest, char * origin, int size) {
    if(!dest || !origin) return;
    for(int i = 0; i < size; i++) {
        dest[i] = origin[i]; //FIXME: essa aq é papo de pior função já feita dsclp
    }
}

BCP * load_program(FILE * fil, int proc_number){
    //read from file
    //load commands in bcp->address[]
    //set registers on 0
    //TODO: carregue programa da memória secundária para a memória sendo apontada pelo BCP
    //FIXME: o ponteiro de arquivo provavelmente teria que ser passado já na segunda fila aqui,
    //       devido a isso, tenhamos cuidado ou achemos um meio de consertar isso por favor;
    char * c = (char *)malloc(sizeof(char) * 25);
    BCP * bcp = createBCP(READY, 0);
    int i = 0;
    int pc = 0;
    while(fread(c + i, sizeof(char), 1, fil)) {
        if(*(c + i) == '\n') {
            *(c + i) = '\0';
            bcp->content = (char *)malloc(sizeof(char) * (i + 1));
            cpystr(bcp->content[pc], c, i);
            i = 0;
            pc += 1;
        }
    }
    if(i > 0) {
        *(c + i) = '\0';
        bcp->content[pc] = (char *)malloc(sizeof(char) * (i + 1));
        cpystr(bcp->content[pc], c, i);
    }
    return bcp;
}

int read_priority(int proc_num) {
    FILE *arq_prio = fopen("prioridades.txt", "r");
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
        if (line_counter == proc_num -1) {
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

void next_process(Scheduler * scheduler){
    
}