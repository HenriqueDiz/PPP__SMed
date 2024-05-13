#include <stdio.h>
#include <string.h>
#include "../lib/estruturas.h"
#include "ctype.h"  //para a cena do "isalpha"
#define TAM 11
#define TAM_STR 50

/////////////////////////////// Funções Auxiliares ///////////////////////////////

// Função para verificar se o array é constituído apenas por números
int verifica_numeros(const char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9')
            return 0; // Não é Número
    }
    return 1; // É um Número
}

// Função para verificar se é apenas por letras
int verifica_letras(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return 0; // não é uma letra nem espaço
        }
    }
    return 1; // É uma letra ou espaço (ambos funceminam)
}

// Função para verificar se email (@ e . obrigadtorios)
int verifica_email(const char *str) {
    int tem_arroba = 0;
    int tem_ponto = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '@') {
            tem_arroba = 1;
        } else if (str[i] == '.') {
            tem_ponto = 1;
        }
    }

    return (tem_arroba && tem_ponto);
}

// Função para verificar se tem formato dd/mm/aaaa
int verifica_data(const char *str) {
    int dia, mes, ano;
    if (sscanf(str, "%d/%d/%d", &dia, &mes, &ano) == 3) { //verificar se estão todas completas
        if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && ano >= 1000 && ano <= 9999) {
            return 1; // data válida
        }
    }
    return 0; // data inválida
}

// Função para verificar se cc tem formato 12345678-1-23
int verifica_cartao_cidadao(const char *str) {
    int n1, n2, n3;
    if (sscanf(str, "%8d-%1d-%2d", &n1, &n2, &n3) == 3) {
        return 1; //  cc válido
    }
    return 0; // cc inválido
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

// Função principal para input de strings -- recebe a flag
char* input_strings(int flag) {
    char input[TAM_STR];
    while (1) {
        fgets(input, TAM_STR, stdin);
        input[strcspn(input, "\n")] = '\0'; // remover quebra de linha

        switch(flag) {
            case 0: // nome
                if (verifica_letras(input)) {
                    return strdup(input);
                } else {
                    printf("Insira um nome válido!\n");
                }
                break;
            case 1: // email
                if (verifica_email(input)) {
                    return strdup(input);
                } else {
                    printf("Insira um email válido.\n");
                }
                break;
            case 2: // data
                if (verifica_data(input)) {
                    return strdup(input);
                } else {
                    printf("Insira uma data válida (dd/mm/aaaa).\n");
                }
                break;
            case 3: // cc
                if (verifica_cartao_cidadao(input)) {
                    return strdup(input);
                } else {
                    printf("Insira um número de cartão de cidadão válido (12345678-1-23).\n");
                }
                break;
            default:
                printf("Flag Incorreta.\n"); //debug
                break;
        }
    }
}
