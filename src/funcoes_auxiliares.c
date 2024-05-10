#include <stdio.h>
#include <string.h>
#include "../lib/estruturas.h"

/////////////////////////////// Funções Auxiliares ///////////////////////////////

int verifica_numeros(const char* input) {
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
        array[strlen(array)-1] == '\0';
    }
}*/

int limpar_buffer() {
    int c, flag = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        flag = 1;
    return flag;
}

PACIENTES find_id(PACIENTES lista,int id){
    PACIENTES paciente = lista->prox; // ignorar header
    while (paciente != NULL && paciente->pessoa.id != id) {
        paciente = paciente->prox;
    }
    return paciente;
}

// Função para o input de números
int input_numeros(int tam){
    char input[tam];
    int num;
    fgets(input, tam, stdin);
    input[strcspn(input, "\n")] = '\0';     // remover o char da nova linha
    if (limpar_buffer() == 1)
        return -1;
    if (verifica_numeros(input)){
        sscanf(input, "%d", &num);
        return num;
    }
    else{   
        return -1;
    }
}