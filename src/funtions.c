#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturas.h"

registo lerRegistos(char *nomeFicheiro) {
    //ler ficheiro
    FILE *ficheiro = fopen(nomeFicheiro, "r");

    //se o ficheiro nao abrir/nao existir
    if (!ficheiro) {
        printf("Erro!! Ficheiro n/ existe! %s\n", nomeFicheiro);
        return NULL;
    }

    //se o ficheiro abrir/existir, criar as listas
    registo *lista = NULL;
    registo *atual = NULL;


    RegistoDoente novoRegisto;

    while (fscanf(ficheiro, "%d\n", &novoRegisto.id_doente) == 1) {
        fgets(novoRegisto.data_registo, sizeof(novoRegisto.data_registo), ficheiro);
        novoRegisto.data_registo[strcspn(novoRegisto.data_registo, "\n")] = '\0'; // Remove o '\n' do final da string
        fscanf(ficheiro, "%d\n%d\n%d\n%d\n", &novoRegisto.tensao_max, &novoRegisto.tensao_min, &novoRegisto.peso, &novoRegisto.altura);

        registo *novoNode = (registo *)malloc(sizeof(registo));
        novoNode->registo = novoRegisto;
        novoNode->next = NULL;

        if (lista == NULL) {
            lista = novoNode;
        } else {
            atual->next = novoNode;
        }
        atual = novoNode;
    }

    fclose(ficheiro);
    return lista;
}

void guardar_txt_registos(registo lista, char *nomeFicheiro) {
    FILE *ficheiro = fopen(nomeFicheiro, "w");
    if (!ficheiro) {
        printf("Erro ao abrir o ficheiro %s\n", nomeFicheiro);
        return;
    }

    registo *atual = lista;
    while (atual != NULL) {
        fprintf(ficheiro, "%d\n", atual->registo.id_doente);
        fprintf(ficheiro, "%s\n", atual->registo.data_registo);
        fprintf(ficheiro, "%d\n", atual->registo.tensao_max);
        fprintf(ficheiro, "%d\n", atual->registo.tensao_min);
        fprintf(ficheiro, "%d\n", atual->registo.peso);
        fprintf(ficheiro, "%d\n", atual->registo.altura);
        atual = atual->next;
    }

    fclose(ficheiro);
}
void adicionarRegisto(registo lista, RegistoDoente novoRegisto) {
    registo *novoNode = (registo *)malloc(sizeof(registo));
    novoNode->registo = novoRegisto;
    novoNode->next = *lista;
    *lista = novoNode;
}

registo procurarUltimoRegistoPorId(registo lista, int id) {
    registo ultimoRegisto = NULL;
    while (lista != NULL) {
        if (lista->registo.id_doente == id) {
            ultimoRegisto = lista;
        }
        lista = lista->next;
    }
    return ultimoRegisto;
}
void free_lista_registos(registo lista) {
    registo atual = lista;
    while (atual != NULL) {
        registo *temp = atual;
        atual = atual->next;
        free(temp);
    }
}


PACIENTES cria(){
    PACIENTES aux;
    /* FAZER AQUI UMA FUNCAO QUE VERIFICA SE JÁ EXISTIAM DADOS EM FICHEIROS */
    info novo = {{1,1,2000},"11111111-1-AB1","header@header.pt","Header", 0, 0};  // Define a informações do Header e iremos usar o 'id' para armazenar o número de pacientes
    aux = (PACIENTES) malloc (sizeof(bloco));
    if (aux != NULL) {
        aux->pessoa = novo;
        aux->prox= NULL;
    }
    return aux;
}

void procura(PACIENTES lista, char* chave_nome, PACIENTES* ant, PACIENTES* actual){
    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) < 0) { /* Ordenamos os pacientes na Lista por ordem Alfabética*/
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) != 0)
        *actual = NULL;   /* Paciente não encontrado, ou seja, é um novo paciente */
}

void insere(PACIENTES lista, info novo) {
    PACIENTES no, ant, inutil;
    no = (PACIENTES) malloc (sizeof(bloco));
    if (no != NULL) {
        no->pessoa = novo;
        procura(lista, novo.nome, &ant, &inutil);
        no->prox= ant->prox;
        ant->prox= no;
    }
    /* FALTA FUNCAO PARA ESCREVER PARA O FICHEIRO */
}

void register_new_patient(PACIENTES informacao) {
    info novo;
    printf("\nQual o nome do paciente ? ");
    fgets(novo.nome, 40, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Qual a data de nascimento do paciente ? (Formato : dia / mes / ano) ");
    char data_nascimento_str[20];
    fgets(data_nascimento_str, 20, stdin);
    sscanf(data_nascimento_str, "%d / %d / %d", &novo.data_nascimento.dia, &novo.data_nascimento.mes, &novo.data_nascimento.ano);
    printf("Qual o cartão de cidadão do paciente ? ");
    fgets(novo.cartao_de_cidadao, 16, stdin);
    novo.cartao_de_cidadao[strcspn(novo.cartao_de_cidadao, "\n")] = '\0';
    printf("Qual o telefone do paciente ? ");
    char telefone_str[20];
    fgets(telefone_str, 20, stdin);
    telefone_str[strcspn(telefone_str, "\n")] = '\0';
    sscanf(telefone_str, "%d", &novo.telefone);
    printf("Qual o email do paciente ? ");
    fgets(novo.email, 40, stdin);
    novo.email[strcspn(novo.email, "\n")] = '\0';
    insere(informacao, novo);
}

void imprime(PACIENTES lista){
    PACIENTES aux = lista->prox; /* Salta o header */
    while (aux) {
        printf("O nome do paciente é : %s\n", aux->pessoa.nome);
        printf("O email do paciente é : %s\n", aux->pessoa.email);
        printf("A data de nascimento do paciente é : %d/%d/%d\n", aux->pessoa.data_nascimento.dia,aux->pessoa.data_nascimento.mes,aux->pessoa.data_nascimento.ano);
        printf("O cartão de cidadão do paciente é : %s\n", aux->pessoa.cartao_de_cidadao);
        printf("O telefone do paciente é : %d\n", aux->pessoa.telefone);
        printf("O ID do paciente é : %d\n", aux->pessoa.id);
        printf("\n");
        aux = aux->prox;
    }
}

// FALTAM DUAS FUNÇÕES : funcao de verificar numbers e outra para limpar o buffer de entrada com ciclo no getchar()

void running(PACIENTES informacao) {
    int choice;
    while (1) {
        printf("////// [Hospital da Universidade de Coimbra] //////// \n");
        printf("/     1. Adicionar novo Doente                      \\\n");
        printf("/     2. Eliminar Doente Existente                  \\\n");
        printf("/     3. Consultar Doentes (Ordem Alfabética)       \\\n");
        printf("/     4. Consultar Doentes - Tensão                 \\\n");
        printf("/     5. Novo Registo Clínico                       \\\n");
        printf("/     6. Listar toda a informação de um pacient     \\\n");
        printf("/     7. Sair                                       \\\n");
        printf("////// [Hospital da Universidade de Coimbra] //////// \n");        
        char input[100];
        printf("\nQual a operação pretendida? ");
        fgets(input, 100, stdin);
        if (sscanf(input, "%d", &choice)!= 1 || choice < 1 || choice > 7 || input[1] != '\n')  // Isto de SAIR (Só é preciso 1 ciclo)
            printf("\nInput inválido. Por favor, introduza um número entre 1 e 7.\n");
        else{ 
            switch (choice) {
                case 1:
                    register_new_patient(informacao); // Introduzir dados de um novo paciente
                    break;
                case 2:
                    // Eliminar um paciente existente
                    break;
                case 3:
                    // Listar todos os pacientes
                    break;
                case 4:
                    // Listar os pacientes com tensões máximas acima de um determinado valor
                    break;
                case 5:
                    // Listar toda a informação de um paciente
                    break;
                case 6:
                    // Novo registo de um paciente
                    break;
                case 7:
                    printf("\n!!! Programa Terminado !!!\n\n");
                    return;
                default:
                    printf("\n!!! NOPE !!!\n\n");
            }
        }
    }
}