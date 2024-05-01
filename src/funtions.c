#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturas.h"

/////////////////////////////// Ficheiro Registos ///////////////////////////////
REGISTOS novo_registro() {
    REGISTOS novo = (REGISTOS)malloc(sizeof(bloco_registo));

    if (novo == NULL) {
        printf("Erro ao alocar memória para o novo registro.\n");
        exit(1);
    }
    novo->prox = NULL;
    return novo;
}

void add_registro(REGISTOS* lista_registros, registo novo_registo) {
    REGISTOS novo = novo_registro();
    novo->reg = novo_registo;
    novo->prox = *lista_registros;
    *lista_registros = novo;
}

void load_registros(PACIENTES lista_pacientes) {
    FILE *ficheiro = fopen("docs/registos.txt", "r");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro registos.txt (Processo: Loading)\n");
        exit(1);
    }
    int id;
    registo novo_registo;
    while (fscanf(ficheiro, "%d", &id) == 1) {
        fscanf(ficheiro, "%d/%d/%d", &novo_registo.data_registo.dia, &novo_registo.data_registo.mes, &novo_registo.data_registo.ano);
        fscanf(ficheiro, "%d", &novo_registo.tensao_max);
        fscanf(ficheiro, "%d", &novo_registo.tensao_min);
        fscanf(ficheiro, "%d", &novo_registo.peso);
        fscanf(ficheiro, "%d", &novo_registo.altura);
        // Procurar pelo ID
        PACIENTES paciente = lista_pacientes;
        while (paciente != NULL && paciente->pessoa.id != id) {
            paciente = paciente->prox;
        }
        // Caso encontre, adicionamos novo registo
        if (paciente != NULL) {
            add_registro(&paciente->pessoa.pessoa_registo, novo_registo);
        }
    }
    if (fclose(ficheiro) == EOF){
        printf("Erro ao fechar o ficheiro registos.txt (Processo: Loading)\n");        
        exit(1);
    }
}

void save_registros(PACIENTES lista_pacientes) {
    FILE* ficheiro = fopen("docs/registos.txt", "w");
    if (ficheiro == NULL) {
        printf("Erro ao fechar o ficheiro registos.txt (Processo: Saving)\n");   
        exit(1);
    }
    PACIENTES paciente= lista_pacientes->prox; // Salta o Header
    while (paciente != NULL) {
        REGISTOS registro = paciente->pessoa.pessoa_registo;
        while (registro != NULL) {
            fprintf(ficheiro, "%d\n", paciente->pessoa.id);
            fprintf(ficheiro, "%d/%d/%d\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
            fprintf(ficheiro, "%d\n", registro->reg.tensao_max);
            fprintf(ficheiro, "%d\n", registro->reg.tensao_min);
            fprintf(ficheiro, "%d\n", registro->reg.peso);
            fprintf(ficheiro, "%d\n", registro->reg.altura);
            registro = registro->prox;
        }
        paciente = paciente->prox;
    }
    if (fclose(ficheiro) == EOF){
        printf("Erro ao fechar o ficheiro registos.txt (Processo: Saving)\n");        
        exit(1);
    }
}

/////////////////////////////// Ficheiro Pacientes ///////////////////////////////

PACIENTES cria_pacientes(){
    PACIENTES aux;
    info novo = {{1,1,2000},"11111111-1-AB1","header@header.pt","Header", 0, 0, NULL};  // Define a informações do Header e iremos usar o 'id' para armazenar o número de pacientes
    aux = (PACIENTES) malloc (sizeof(bloco));
    if (aux != NULL) {
        aux->pessoa = novo;
        aux->prox= NULL;
    }
    return aux;
}

PACIENTES novo_paciente() {
    PACIENTES novo = (PACIENTES)malloc(sizeof(bloco));
    if (novo == NULL) {
        printf("Erro de alloc.\n");
        exit(1);
    }
    novo->prox = NULL;
    novo->pessoa.pessoa_registo = NULL;
    return novo;
}

void add_paciente(PACIENTES* lista_pacientes, info dados) {
    PACIENTES novo = novo_paciente();
    novo->pessoa = dados;
    novo->prox = *lista_pacientes;
    *lista_pacientes = novo;
}

PACIENTES load_pacientes() {
    FILE *ficheiro = fopen("docs/doentes.txt", "r");

    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro doentes.txt (Processo: Loading)\n");        
        exit(1);
    }
    PACIENTES lista_pacientes = cria_pacientes();
    info dados;

    while (fscanf(ficheiro, "%d", &dados.id) == 1) {
        fgetc(ficheiro);
        fgets(dados.nome, sizeof(dados.nome), ficheiro);
        dados.nome[strcspn(dados.nome, "\n")] = '\0';
        fscanf(ficheiro, "%d/%d/%d", &dados.data_nascimento.dia, &dados.data_nascimento.mes, &dados.data_nascimento.ano);
        fscanf(ficheiro, "%s", dados.cartao_de_cidadao);
        fscanf(ficheiro, "%d", &dados.telefone);
        fscanf(ficheiro, "%s", dados.email);

        add_paciente(&lista_pacientes, dados);

        fgetc(ficheiro); // delete char nova linha após email
    }if (fclose(ficheiro) == EOF){
        printf("Erro ao fechar o ficheiro doentes.txt (Processo: Loading)\n");       
        exit(1);
    }
    return lista_pacientes;
}

void save_pacientes(PACIENTES lista_pacientes) {
    FILE* ficheiro = fopen("docs/doentes.txt", "w");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro doentes.txt (Processo: Saving)\n");       
        exit(1);
    }

    PACIENTES paciente = lista_pacientes->prox; // Saltamos o Header
    while (paciente != NULL) {
        fprintf(ficheiro, "%d\n", paciente->pessoa.id);
        fprintf(ficheiro, "%s\n", paciente->pessoa.nome);
        fprintf(ficheiro, "%d/%d/%d\n", paciente->pessoa.data_nascimento.dia, paciente->pessoa.data_nascimento.mes, paciente->pessoa.data_nascimento.ano);
        fprintf(ficheiro, "%s\n", paciente->pessoa.cartao_de_cidadao);
        fprintf(ficheiro, "%d\n", paciente->pessoa.telefone);
        fprintf(ficheiro, "%s\n", paciente->pessoa.email);
        paciente = paciente->prox;
    }
    if (fclose(ficheiro) == EOF){
        printf("Erro ao fechar o ficheiro doentes.txt (Processo: Saving)\n");       
        exit(1);
    }
}

/////////////////////////////// Funcionalidades da aplicação ///////////////////////////////

void registar(PACIENTES informacao) {
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
    add_paciente(&informacao, novo);
}

void imprime(PACIENTES lista){
    PACIENTES aux = lista->prox; // Salta o header
    while (aux) {
        printf("ID: %d\n", aux->pessoa.id);
        printf("Nome: %s\n", aux->pessoa.nome);
        printf("Data de Nascimento: %d/%d/%d\n", aux->pessoa.data_nascimento.dia, aux->pessoa.data_nascimento.mes, aux->pessoa.data_nascimento.ano);
        printf("Cartão de Cidadão: %s\n", aux->pessoa.cartao_de_cidadao);
        printf("Telefone: %d\n", aux->pessoa.telefone);
        printf("Email: %s\n\n", aux->pessoa.email);
        aux = aux->prox;
    }
}

// TODO - Missing 2 functions : funcao de verificar numbers e outra para limpar o buffer de entrada com ciclo no getchar()

/////////////////////////////// Função Running ///////////////////////////////

void running(PACIENTES informacao) {
    int choice;
    while (1) {
        printf("////// [Hospital da Universidade de Coimbra]  //////\n");
        printf("///   1. Adicionar novo Doente                   ///\n");
        printf("///   2. Eliminar Doente Existente               ///\n");
        printf("///   3. Consultar Doentes (Ordem Alfabética)    ///\n");
        printf("///   4. Consultar Doentes - Tensão              ///\n");
        printf("///   5. Novo Registo Clínico                    ///\n");
        printf("///   6. Listar toda a informação de um paciente ///\n");
        printf("///   7. Sair                                    ///\n");
        printf("////// [Hospital da Universidade de Coimbra]  //////\n");       
        char input[100];
        printf("\nQual a operação pretendida? ");
        fgets(input, 100, stdin);
        if (sscanf(input, "%d", &choice)!= 1 || choice < 1 || choice > 7 || input[1] != '\n')
            printf("\nInput inválido. Por favor, introduza um número entre 1 e 7.\n");
        else { 
            switch (choice) {
                case 1:
                    registar(informacao); // Introduzir dados de um novo paciente
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
                    imprime(informacao); // TODO: just to test if it works
                    break;
                case 7:
                    printf("\n!!! Programa Terminado !!!\n\n");
                    return;
            }
        }
    }
}