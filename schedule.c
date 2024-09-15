#include "schedule.h"

BCP * createBCP(State state, int credits){
    BCP * bcp = (BCP *)malloc(sizeof(BCP));
    bcp->content = (BCP *)malloc(sizeof(char *) * 22);
    bcp->state = state;
    bcp->regs.X = 0;
    bcp->regs.Y = 0;
    bcp->regs.PC = 0;
    bcp->credits = credits;
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
Comm line_processer(BCP * bcp, int line) {
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
    switch (bcp->content[line][0])
    {
        //TODO: códigos para comandos
        case 'C':
            //código para COMM, acho q só diminuir o quantum e pá
            return COM;
        case 'S':
            //fechar processo
            return END;
        case 'E':
            //Mudar processo para fila de entrada e saída (não sei se tem mais além disso)
            return END;
        case 'X':
            //Atribuição para variável X
            bcp->regs.X = atoi(bcp->content[line][2]);
            return ATRIB;
        case 'Y':
            //Atribuição para variável Y
            bcp->regs.Y = atoi(bcp->content[line][2]);
            return ATRIB;
        
        default:
            break;
    }

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
    //nota: as linhas "úteis" serão apenas de 1~n, com a linha 0 sendo usada para referenciar o nome do processo
    char * c = (char *)malloc(sizeof(char) * 25);
    BCP * bcp = createBCP(READY, 0);
    int i = 0;
    int pc = 0;
    /*     
    estou (quesheg) reescrevendo a string c a cada iteração para aproveitar o espaço
    de c mantendo o ponteiro para a origem e usando o i para calcular o próximo char
    */
    while(fread(c + i, sizeof(char), 1, fil)) {
        if(*(c + i) == '\n') {
            *(c + i) = '\0';
            bcp->content[pc] = (char *)malloc(sizeof(char) * (i + 1));
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