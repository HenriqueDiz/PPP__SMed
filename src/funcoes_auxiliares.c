#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../lib/estruturas.h"
#define TAM_INT 12
#define TAM_STR 52

/////////////////////////////// Funções Auxiliares ///////////////////////////////

// Função para verificar se o array é constituído apenas por números
int verifica_numeros(const char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9')
            return 0; // Não é Número
    }
    return 1; // É um Número
}

// Função para verificar se é uma string
int verifica_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ')
            return 0; // Não é só letras e espaços
    }
    return 1; // É só letras e espaços
}

// Função para verificar se email ('@' e '.' obrigatórios)
int verifica_email(const char *str) {
    int tem_arroba = 0, tem_ponto = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '@')
            tem_arroba = 1;
        else if (str[i] == '.') 
            tem_ponto = 1;
    }
    return (tem_arroba && tem_ponto);
}

// Função para verificar se CC tem formato 12345678-9-AB1
int verifica_cartao_cidadao(const char *str) {
    int n1, n2, n3;
    char c1, c2;
    if (strlen(str) == 14 && sscanf(str, "%8d-%1d-%1c%1c%1d", &n1, &n2, &c1, &c2, &n3) == 5) { //verificar 8 nums - 1 num - 2 letras + 1 num (14 Caracteres)
        if (isupper(c1) && isupper(c2)) // Verificar uppercase
            return 1; // Válido
    }
    return 0; // É válido
}

// Função para Limpar o Buffer
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função encontrar o nódulo do paciente pelo ID
PACIENTES find_id(PACIENTES lista,int id){
    PACIENTES paciente = lista->prox; // Ignoramos o Header
    while (paciente != NULL && paciente->pessoa.id != id)
        paciente = paciente->prox;
    return paciente;
}

/////////////////////////////// Funções para os Inputs ///////////////////////////////

int input_numeros(int flag) {  // Portanto, flag = 1 -> Número de Telefone
    int num;
    char input[TAM_INT];
    while (1) {
        fgets(input, TAM_INT, stdin);
        if (input[strlen(input) - 1] == '\n') { // Verifica se o input tem o caractere '\n' e o remove
            input[strcspn(input, "\n")] = '\0';  // Removemos a primeira ocorrência de '\n'
            if (strlen(input) == 0) { // Add this condition to check for blank input
                printf("Por favor, insira um valor válido: ");
                continue;
            }
            if (verifica_numeros(input)) {
                if (flag == 1 && strlen(input) != 9)
                    printf("Número de telefone é composto por 9 dígitos! Insira novamente: ");
                else {
                    sscanf(input, "%d", &num);
                    return num;
                }
            } else
                printf("Número inválido. Por favor, insira dígitos válidos: ");
        } else {
            printf("Número inválido. Por favor, insira um número dentro do tamanho do Buffer (%d dígitos no Máximo): ", TAM_INT - 2);
            limpar_buffer();
        }
    }
}

// Função para o input de Datas
data input_data() {
    data temp;
    char input[TAM_STR];
    while (1) {
        fgets(input, TAM_STR, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strcspn(input, "\n")] = '\0';   // Removemos a primeira occorência de '\n'
            if (sscanf(input, "%d/%d/%d", &temp.dia, &temp.mes, &temp.ano) == 3) { // Verificar se estão todas completas
                if (temp.dia >= 1 && temp.dia <= 31 && temp.mes >= 1 && temp.mes <= 12 && temp.ano >= 1000 && temp.ano <= 9999)
                    return temp; // Data Válida
                else
                    printf("Data inválida. Por favor, insira números válidos para a Data : ");
            } 
            else printf("Data inválida. Por favor, preencha os 3 campos da Data : ");
        } else{
            printf("Data inválida. Por favor, insira uma Data válida : ");
            limpar_buffer();            
        }
    }
}

// Função para o input de Strings (Nome, Email, CC)
char* input_strings(int flag) {
    char input[TAM_STR];
    while (1) {
        fgets(input, TAM_STR, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strcspn(input, "\n")] = '\0';   // Removemos a primeira occorência de '\n'
            switch(flag) {
                case 0:   // Nome
                    if (verifica_string(input)) return strdup(input);
                    else printf("Nome inválido. Por favor, insira um Nome válido : ");
                    break;
                case 1:   // Email
                    if (verifica_email(input)) return strdup(input);
                    else printf("Email inválido. Por favor, insira um Email válido : ");
                    break;
                case 2:   // CC
                    if (verifica_cartao_cidadao(input)) return strdup(input);
                    else printf("CC inválido. Por favor, insira um CC válido (Formato: 12345678-9-AB1) : ");
                    break;
            }
        }
        else{
            printf("Input inválido. Por favor, insira um Input dentro do tamanho do Buffer ( %d Caracteres no Máximo ) : ", TAM_STR - 2);
            limpar_buffer();            
        }
    }
}
