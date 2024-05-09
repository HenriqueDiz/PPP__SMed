#include <stdio.h>
#include "../lib/estruturas.h"

/////////////////////////////// Funções Auxiliares ///////////////////////////////

int verifica_numeros(const char* input) {  //implementar depois no loop, very raw
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9')
            return 0; // nao é numero
    }
    return 1; // é um numero
}

/* FEITA PELO STOR 
void limpar_buffer(char* array) { //very raw tbm
    int c;
    if (array[strlen(array)-1] != '\n')
        while ((c = getchar()) != '\n' && c != EOF);
    else{
        array[strlen(array)-1] != '\0';
    }
}*/

void limpar_buffer() { //very raw tbm
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

PACIENTES find_id(PACIENTES lista,int id){
    PACIENTES paciente = lista->prox; // ignorar header
    while (paciente != NULL && paciente->pessoa.id != id) {
        paciente = paciente->prox;
    }
    return paciente;
}