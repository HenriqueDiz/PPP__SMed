#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturas.h"

/////////////////////////////// Funções Auxiliares ///////////////////////////////

int verifica_numeros(const char *input) {  //implementar depois no loop, very raw
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

// TODO: DO THEY WORK FINE ???? : IF NOT USE FUNCTIONS FROM SLIDE MAYBE 

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
    PACIENTES paciente = lista_pacientes->prox; // Salta o Header
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

void procura(PACIENTES lista, char* chave_nome, PACIENTES* ant, PACIENTES* actual){
    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) < 0) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) != 0)
        *actual = NULL;   /* elemento não encontrado*/
}

void add_paciente(PACIENTES lista, info novo) {
    PACIENTES no, ant, inutil;
    no = (PACIENTES) malloc (sizeof(bloco));
    if (no != NULL) {
        no->pessoa = novo;
        procura(lista, novo.nome, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

void elimina(PACIENTES lista, info chave){
    PACIENTES ant, actual;
    procura (lista, chave.nome, &ant, &actual);
    if (actual != NULL) {
        ant->prox = actual->prox;
        free (actual);
    }
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
        fscanf(ficheiro, "%d", &dados.id);
        fgets(dados.nome, 40, ficheiro);
        dados.nome[strcspn(dados.nome, "\n")] = '\0';
        fscanf(ficheiro, "%d/%d/%d", &dados.data_nascimento.dia, &dados.data_nascimento.mes, &dados.data_nascimento.ano);
        fscanf(ficheiro, "%s", dados.cartao_de_cidadao);
        fscanf(ficheiro, "%d", &dados.telefone);
        fscanf(ficheiro, "%s", dados.email);
        lista_pacientes->pessoa.id++; // Estamos a adicionar ao id do header para sabermos quantos doentes estão armazenados
        add_paciente(lista_pacientes, dados);
    }
    if (fclose(ficheiro) == EOF){
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
    novo.id = informacao->pessoa.id + 1;
    add_paciente(informacao, novo);
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

void eliminar_paciente(PACIENTES *lista_pacientes) { //nao está a encontrar o id
    int id;
    printf("\nQual o ID do paciente a ser eliminado? ");
    char input_id[20]; //meter isto como char para depois comparar como o prof aconselhou
    fgets(input_id, 20, stdin);
    if (verifica_numeros(input_id)) {
        sscanf(input_id, "%d", &id);
        PACIENTES anterior = *lista_pacientes;
        PACIENTES atual = (*lista_pacientes)->prox;
        while (atual != NULL && atual->pessoa.id != id) {
            anterior = atual;
            atual = atual->prox;
        }
        if (atual != NULL) {
            anterior->prox = atual->prox;
            free(atual);
            printf("Paciente eliminado!\n");
        } else {
            printf("Paciente com ID %d não encontrado.\n", id);
        }
    } else {
        printf("ID inválido. Por favor, insira um número válido.\n"); //estamos a cair sempre aqui, ele n encontra o id
        limpar_buffer();
    }
}

void listar_tensoes_acima(PACIENTES lista) { //nao esta a encontrar as tensoes, parece nao ter qq track das tensoes
    int valor_limite;
    printf("\nQual o valor limite da tensão máxima? ");
    char input_limite[20]; //fazer isto como char para depoiis comparar como o prof aconselhou
    fgets(input_limite, 20, stdin);
    if (verifica_numeros(input_limite)) {
        sscanf(input_limite, "%d", &valor_limite);
        PACIENTES paciente = lista->prox; // ignorar header
        int encontrados = 0;
        while (paciente != NULL) {
            REGISTOS registro = paciente->pessoa.pessoa_registo;
            while (registro != NULL) {
                if (registro->reg.tensao_max > valor_limite) {
                    encontrados = 1;
                    printf("\nPaciente ID: %d\n", paciente->pessoa.id);
                    printf("Tensão Máxima: %d\n", registro->reg.tensao_max);
                    printf("Tensão Mínima: %d\n", registro->reg.tensao_min);
                    printf("Peso: %d\n", registro->reg.peso);
                    printf("Altura: %d\n", registro->reg.altura);
                }
                registro = registro->prox;
            }
            paciente = paciente->prox;
        }
        if (!encontrados) {
            printf("Nenhum paciente com tensão máxima acima de %d encontrado.\n", valor_limite);
        }
    } else {
        printf("Valor limite inválido. Por favor, insira um número válido.\n");  //estamos tbm aqui neste
        limpar_buffer();
    }
}

void listar_informacao_paciente(PACIENTES lista) { //nao esta a encontrar o id
    int id_paciente;
    printf("\nQual o ID do paciente? ");
    char input_id[20]; //fazer isto como char para depois comparar como o prof aconselhou
    fgets(input_id, 20, stdin);
    if (verifica_numeros(input_id)) {
        sscanf(input_id, "%d", &id_paciente);
        PACIENTES paciente = lista->prox; // ignorar header
        while (paciente != NULL && paciente->pessoa.id != id_paciente) {
            paciente = paciente->prox;
        }
        if (paciente != NULL) {
            printf("\nInformação do Paciente ID: %d\n", paciente->pessoa.id);
            printf("Nome: %s\n", paciente->pessoa.nome);
            printf("Data de Nascimento: %d/%d/%d\n", paciente->pessoa.data_nascimento.dia, paciente->pessoa.data_nascimento.mes, paciente->pessoa.data_nascimento.ano);
            printf("Cartão de Cidadão: %s\n", paciente->pessoa.cartao_de_cidadao);
            printf("Telefone: %d\n", paciente->pessoa.telefone);
            printf("Email: %s\n", paciente->pessoa.email);
            printf("Registros Médicos:\n");
            REGISTOS registro = paciente->pessoa.pessoa_registo;
            while (registro != NULL) {
                printf("Data do Registo: %d/%d/%d\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
                printf("Tensão Máxima: %d\n", registro->reg.tensao_max);
                printf("Tensão Mínima: %d\n", registro->reg.tensao_min);
                printf("Peso: %d\n", registro->reg.peso);
                printf("Altura: %d\n", registro->reg.altura);
                registro = registro->prox;
            }
        } else {
            printf("Paciente com ID %d não encontrado.\n", id_paciente);
        }
    } else {
        printf("ID inválido. Por favor, insira um número válido.\n");
        limpar_buffer();
    }
}

/////////////////////////////// Função Running ///////////////////////////////

void running(PACIENTES informacao) {
    int choice;
    while (1) {
        printf("\n////// [Hospital da Universidade de Coimbra]  //////\n");
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