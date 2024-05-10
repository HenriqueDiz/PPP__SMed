#include <stdio.h>
#include <string.h>
#include "../lib/estruturas.h"
#define TAM 10

/////////////////////////////// Funções Auxiliares ///////////////////////////////

// Função para verificar se o array é constituído apenas por números
int verifica_numeros(const char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9')
            return 0; // Não é Número
    }
    return 1; // É um Número
}

// Função para Limpar o Buffer
int limpar_buffer() {
    int c, flag = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        flag = 1;
    return flag;
}

// Função encontrar o nódulo do paciente pelo ID
PACIENTES find_id(PACIENTES lista,int id){
    PACIENTES paciente = lista->prox; // Ignoramos o Header
    while (paciente != NULL && paciente->pessoa.id != id) {
        paciente = paciente->prox;
    }
    return paciente;
}

// Função para o input de números
int input_numeros() {
    int num;
    char input[TAM];
    while (1) {
        fgets(input, TAM, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strcspn(input, "\n")] = '\0';   // Removemos a primeira occorência de '\n'
            if (verifica_numeros(input)) {
                sscanf(input, "%d", &num);
                return num;
            } else {
                printf("Input inválido. Por favor, insira um número válido: ");
            }
        } else {
            printf("Input inválido. Por favor, insira um número dentro do tamanho do Buffer ( %d dígitos no Máximo ) : ",TAM);
            limpar_buffer();
        }
    }
}

// Função para o input de strings ???