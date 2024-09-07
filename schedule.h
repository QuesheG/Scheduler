typedef struct reg {
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
    char text[25];
} Process;