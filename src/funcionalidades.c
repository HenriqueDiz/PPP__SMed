#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/estruturas.h"
#include "../lib/declaracoes.h"

/////////////////////////////// Funcionalidades da aplicação ///////////////////////////////

// Case 1 : Adicionar novo paciente
void novo_paciente(PACIENTES informacao) {
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
    novo.id = ++informacao->pessoa.id;  // aumentamos o número de pacientes no header
    while(find_id(informacao,novo.id) != NULL) // caso o id já exista, continuamos a incrementar até conseguirmos um id 
        novo.id++;
    novo.pessoa_registo =  NULL;
    insere_pacientes(informacao, novo);
    save_pacientes(informacao); // Guardamos a informação no ficheiro dos pacientes
}

// Case 2 : Eliminar Doente Existente
void elimina_pacientes(PACIENTES lista) {
    int id;
    printf("ID do paciente p/ eliminar? ");
    scanf("%d", &id);
    PACIENTES ant = lista, atual = lista->prox;
    while (atual != NULL && atual->pessoa.id != id) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        ant->prox = atual->prox;
        free(atual);
        printf("Paciente %d eliminado!", id);
    } else {
        printf("Paciente com ID %d não encontrado\n", id);
    }
    limpar_buffer();
    save_pacientes(lista); // Guardamos a informação no ficheiro dos pacientes
}

// Case 3 : Consultar Doentes (Ordem Alfabética)
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

// Case 4 : Consultar Doentes - Tensão
void listar_tensoes_acima(PACIENTES lista) {
    int valor_limite;
    printf("\nQual o valor limite da tensão máxima? ");
    char input_limite[20];
    fgets(input_limite, 20, stdin);
    input_limite[strcspn(input_limite, "\n")] = '\0';     // remover o char da nova linha
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
        printf("Valor limite inválido. Por favor, insira um número válido.\n");
    }
    limpar_buffer();
}

// Case 5 : Novo Registo Clínico 
void novo_registo(PACIENTES lista) {
    int id;
    printf("\nQual o ID do paciente para adicionar um novo registo? ");
    scanf("%d", &id);
    limpar_buffer(); // Clear the input buffer
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        bloco_registo novo_registro;
        printf("Qual a data do registo (Formato : dia / mes / ano)? ");
        char data_registo_str[20];
        fgets(data_registo_str, 20, stdin);
        sscanf(data_registo_str, "%d / %d / %d", &novo_registro.reg.data_registo.dia, &novo_registro.reg.data_registo.mes, &novo_registro.reg.data_registo.ano);
        printf("Qual a tensão máxima? ");
        char tensao_max_str[20];
        fgets(tensao_max_str, 20, stdin);
        sscanf(tensao_max_str, "%d", &novo_registro.reg.tensao_max);
        printf("Qual a tensão mínima? ");
        char tensao_min_str[20];
        fgets(tensao_min_str, 20, stdin);
        sscanf(tensao_min_str, "%d", &novo_registro.reg.tensao_min);
        printf("Qual o peso? ");
        char peso_str[20];
        fgets(peso_str, 20, stdin);
        sscanf(peso_str, "%d", &novo_registro.reg.peso);
        printf("Qual a altura? ");
        char altura_str[20];
        fgets(altura_str, 20, stdin);
        sscanf(altura_str, "%d", &novo_registro.reg.altura);

        insere_registo(novo_registro, lista, id);
        save_registros(lista); // Guardamos a informação no ficheiro dos registos
        printf("\nNovo registo adicionado com sucesso!\n");
    } else {
        printf("ID inválido. Por favor, insira um ID válido.\n");
    }
}

// Case 6 : Listar toda a informação de um paciente
void listar_informacao_paciente(PACIENTES lista) { //nao esta a encontrar o id
    int id = 0;
    printf("\nQual o ID do paciente? ");
    scanf("%d", &id); // MUDEI SO PARA TESTAR O CODIGO ( precisava de ver se a parte dos ficheiros dos registos estava a ser bem feita)
    if (id) {
        PACIENTES paciente = find_id(lista,id);
        if (paciente != NULL) {
            printf("\nInformação do Paciente ID: %d\n\n", paciente->pessoa.id);
            printf("Nome: %s\n", paciente->pessoa.nome);
            printf("Data de Nascimento: %d/%d/%d\n", paciente->pessoa.data_nascimento.dia, paciente->pessoa.data_nascimento.mes, paciente->pessoa.data_nascimento.ano);
            printf("Cartão de Cidadão: %s\n", paciente->pessoa.cartao_de_cidadao);
            printf("Telefone: %d\n", paciente->pessoa.telefone);
            printf("Email: %s\n", paciente->pessoa.email);
            printf("\nRegistos Médicos:\n");
            REGISTOS registro = paciente->pessoa.pessoa_registo;
            while (registro != NULL) {
                printf("\nData do Registo: %d/%d/%d\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
                printf("Tensão Máxima: %d\n", registro->reg.tensao_max);
                printf("Tensão Mínima: %d\n", registro->reg.tensao_min);
                printf("Peso: %d\n", registro->reg.peso);
                printf("Altura: %d\n", registro->reg.altura);
                registro = registro->prox;
            }
        } else {
            printf("\nPaciente com ID %d não encontrado.\n", id);
        }
    } else {
        printf("ID inválido. Por favor, insira um número válido.\n");
    }
    limpar_buffer();
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
                case 1:   // Introduzir dados de um novo paciente
                    novo_paciente(informacao);
                    break;
                case 2:   // Eliminar um paciente existente
                    elimina_pacientes(informacao);
                    break;
                case 3:   // Listar todos os pacientes
                    imprime(informacao);
                    break;
                case 4:   // Listar os pacientes com tensões máximas acima de um determinado valor
                    listar_tensoes_acima(informacao);
                    break;
                case 5:   // Adicionar um novo Registo Clínico
                    novo_registo(informacao);
                    break;
                case 6:   // Listar toda a informação de um paciente
                    listar_informacao_paciente(informacao);
                    break;
                case 7:
                    printf("\n!!! Programa Terminado !!!\n\n");
                    return;
            }
        }
    }
}
