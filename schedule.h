#include "list_table.h"

typedef struct reg {
    int X;
    int Y;
    int PC; //int que guarda a posição 
} Registers;

typedef enum comm {
    IO,
    COM,
    ATRIB,
    END
} Comm;

typedef enum state {
    BLOCK,
    READY,
    EXEC
} State;

typedef struct proc {
    int x;
    int y;
    int io_timer;
    char text[25];
} Process;

typedef struct bcp {
    char ** content; //máximo de umass 23 linha acho
    State state;
    Registers regs; //Transferi o PC para cá
    int credits;
    int io_timer; //Transferindo iotimer para ca, nao sei se sera necessario aquela struct Process
} BCP;

typedef struct Scheduler{
    BCP ** table; //Array de ponteiros de BCP (tabela de processos)
    Queue * ready_queue;
    Queue * blocked_queue;
    int quantum;
} Scheduler;

BCP * createBCP(char ** lines, State state, Registers regs, int credits);
Scheduler * create_scheduler();
Comm line_reader(FILE *file);
BCP * load_program(FILE * bcp, int proc_number);