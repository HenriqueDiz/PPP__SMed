#include <stdio.h>
#include <string.h>
#include "../lib/estruturas.h"
#define TAM 11

/////////////////////////////// Funções Auxiliares ///////////////////////////////

// Função para verificar se o array é constituído apenas por números
int verifica_numeros(const char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9')
            return 0; // Não é Número
    }
    return 1; // É um Número
}

// Função para verificar se a string contém apenas letras
int verifica_letras(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return 0; // Não é uma letra
        }
    }
    return 1; // É uma letra
}

// Função para Limpar o Buffer
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
            printf("Input inválido. Por favor, insira um número dentro do tamanho do Buffer ( %d dígitos no Máximo ) : ",TAM - 2);
            limpar_buffer();
        }
    }
}

// Função para input de datas
int verificar_data(char *data, int*dia, int*mes, int*ano) {
    char* input;
    const char delim[2]= "/"; //data separada por /
    input = strtok(data, delim);
    if(input == NULL) return 0; //se nada vier no input
    if (verifica_numeros(input) > 0 && verifica_numeros(input) <= 31)//dias até 31
        *dia = verifica_numeros(input);
    else {
        return 0;
    }
    input = strtok(NULL, delim);
    if(input == NULL) return 0; 
    if (verifica_numeros(input) > 0 && verifica_numeros(input) <= 12) { //meses ate 12
        *mes = verifica_numeros(input);
    } else {
        return 0;
    }
    input = strtok(NULL, delim);
    if(input == NULL) return 0;
    if (verifica_numeros(input) > 0 && verifica_numeros(input) <= 9999) { //ano até ao ano 9999
        *ano = verifica_numeros(input);
    } else {
        return 0;
    }
    return 1;
}

// Função para o input de strings
char* input_strings() {
    char input[TAM];
    while (1) {
        printf("Insira uma string: ");
        fgets(input, TAM, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strcspn(input, "\n")] = '\0'; // Removemos a primeira ocorrência de '\n'
            if (verifica_letras(input)) {
                return strdup(input);
            } else {
                printf("Input inválido. Por favor, insira uma string válida contendo apenas letras.\n");
            }
        } else {
            printf("Input inválido. Por favor, insira uma string dentro do tamanho do Buffer (%d caracteres no máximo).\n", TAM);
            limpar_buffer();
        }
    }
}