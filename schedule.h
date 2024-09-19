#include "list_table.h"

// Forward declaration de structs que estão em list_table.h
struct Node;
struct Queue;

typedef struct registers {
    int X;
    int Y;
    int PC; // int que guarda a posição 
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
    char ** content; //máximo de umas 23 linhas
    State state;
    Registers regs; // Transferi o PC para cá
    int credits;
    int io_timer; // Transferi o iotimer para cá
} BCP;

typedef struct Scheduler {
    BCP ** table; // Array de ponteiros de BCP (tabela de processos)
    Queue * ready_queue; // Agora referenciamos a Queue da list_table.h
    Queue * blocked_queue;
    int io_time;
    int quantum;
} Scheduler;

BCP * createBCP(State state, int credits);
Scheduler * create_scheduler(int quantum); // Alteração para aceitar o quantum como argumento
Comm line_reader(FILE *file);
Comm line_processer(BCP * bcp);
int get_process(Scheduler * s);
BCP * load_program(FILE * bcp, int proc_number, int credits);
int read_priority(int proc_num);
int next_process(Scheduler * s);