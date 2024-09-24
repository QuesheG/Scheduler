#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <stdbool.h>
#include <stdio.h>
// #include "list_table.h"
typedef struct q Queue;

typedef struct registers {
    int X;
    int Y;
    int PC;
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
    char ** content;
    State state;
    Registers regs;
    int credits;
    int io_timer;
} BCP;

typedef struct Scheduler {
    BCP ** table; //Array de ponteiros de BCP (tabela de processos)
    Queue * ready_queue; //Referenciamos a Queue da list_table.h
    Queue * blocked_queue;
    int io_time;
    int quantum;
} Scheduler;

BCP * createBCP(State state, int credits);
Scheduler * create_scheduler(int quantum); // Alteração para aceitar o quantum como argumento
Comm line_processer(BCP * bcp);
int get_process(Scheduler * scheduler);
BCP * load_program(FILE * bcp, int credits);
int read_priority(int proc_num);
int get_process(Scheduler * scheduler);
int next_process(Scheduler * scheduler);
bool load_all(Scheduler * scheduler, FILE * log);

#endif