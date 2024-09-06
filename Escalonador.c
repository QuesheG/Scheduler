#include "schedule.h"
#include "list_table.h"
#include <stdio.h>
#include <stdlib.h>

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

//parser para cada linha do arquivo texto recebido
Comm line_reader(FILE *file) {  //Eu (luiyu) pergunto-me se nao é melhor ler um file (programa) por vez e ja ir armazendando os comandos nos arrays
    Comm found = END;
    char c[3] = {'A', 'A', 'A'};
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
    ************************/
    fread(c, sizeof(char), 2, file);
    if(compare_res(c, com,  3)) return COM;
    if(compare_res(c, exit, 3)) return END;
    if(compare_res(c, io,   3)) return IO;

    if(compare_res(c, atrib1, 2)) {
        int val = read_val(file);
        //TODO: armazenar valor recebido em X;
    }
    else if(compare_res(c, atrib2, 2)) {
        int val = read_val(file);
        //TODO: armazenar valor recebido em Y;
    }
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
    int quantum = read_val(q);
    if(q<1) {
        printf("Quantum should be greater than 0\n");
        printf("Exiting...\n");
        return 0;
    }

    //ler prios


    /*Depois de ler prios, ir colocando na fila de prontos de forma ja ordenada
    while () {
        add_to_ready_queue(); 
    }
    */

    
    return 0;
}