#include <stdio.h>
#include <stdlib.h>
#include "list_table.h"
#include "schedule.h"

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

int main(void) {
    //checa quantum
    FILE *q = fopen("quantum.txt", "r");
    if(!q) {
        printf("Could not read quantum value\n");
        printf("Quantum should be specified in quantum.txt\n");
        printf("Exiting...\n");
        return 0;
    }

    //Cria escalonador
    Scheduler * scheduler = create_scheduler(read_val(q));

    if(scheduler->quantum < 1) {
        printf("Quantum should be greater than 0\n");
        printf("Exiting...\n");
        return 0;
    }

    //Carregar programas em memoria
    for (int i = 0; i < 10; i++) {
        //ler prios;
        int credit = read_priority(i);
        
        //carregar programas para memoria
        load_program(scheduler->table[i], i); //FIXME: atualmente essa linha ta dando uma string e pá, tem q mudar pra passar o arquivo e ir alocando o bcp na tabela
        


        //load_process(i, scheduler); // (quesheg) acho q poderíamos fazer esse primeiro, pra aí já ir alocando os arquivos na ordem q precisa e criar a fila ordenadamente
        //log_function(); Dps da pra fazer isso no final //TODO: logs -_-

        Node * process_node = create_node(credit);
        enqueue_ready(scheduler, process_node);
    }

    bool run = true;

    while (run) { //main while
        //Ler primeiro da fila de ready
        //Ler do bcp desse processo o comando a ser executado

        //Executar comando (💀) - Subtrair creditos tambem


        //Atualiza a fila de bloqueados e os reinsere
        update_blocked_queue();

        //Escolhe proximo processo
        switch (next_process()) {
        case -1:

            break;

        case 0:

            break;
        
        case 1:

            break;
        }
        
        //Pequenos ajustes de casos especificos descritos no ep
    }
    


    
    return 0;
}

