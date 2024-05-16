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
    strcpy(novo.nome,input_strings(0));
    printf("Qual a data de nascimento do paciente ? (Formato : dia / mes / ano) ");
    novo.data_nascimento = input_data();
    printf("Qual o cartão de cidadão do paciente ? (Formato: 12345678-9-AB1) ");
    strcpy(novo.cartao_de_cidadao,input_strings(2));
    printf("Qual o telefone do paciente ? ");
    novo.telefone = input_numeros();
    printf("Qual o email do paciente ? ");
    strcpy(novo.email,input_strings(1));
    informacao->pessoa.id++;  // Aumentamos o número de pacientes no header
    novo.id = 1;
    while(find_id(informacao,novo.id) != NULL) // Caso o id já exista, continuamos a incrementar até conseguirmos um ID único
        novo.id++;
    novo.pessoa_registo =  NULL;
    insere_pacientes(informacao, novo); // Inserimos o novo paciente na estrutura
    save_pacientes(informacao); // Guardamos a informação no ficheiro dos pacientes
    printf("\nNovo paciente adicionado!\n");
}

// Case 2 : Eliminar Doente Existente
void elimina_pacientes(PACIENTES lista) {
    printf("\nID do paciente p/ eliminar? ");
    int id = input_numeros();
    PACIENTES ant = lista, atual = lista->prox;
    while (atual != NULL && atual->pessoa.id != id) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        ant->prox = atual->prox;
        free(atual);
        printf("\nPaciente com ID %d eliminado!", id);
    } else
        printf("\nPaciente com ID %d não encontrado\n", id);
    save_pacientes(lista); // Guardamos a informação no ficheiro dos pacientes
}

// Case 3 : Consultar Doentes (Ordem Alfabética)
void imprime(PACIENTES lista){
    size_t size = lista->pessoa.id; // Vamos buscar ao Header o tamanho da Lista
    PACIENTES aux = lista->prox; // Ignoramos o Header
    for (size_t i = 0; i < size; i++){
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
    printf("\nQual o valor limite da tensão máxima? ");
    int limite = input_numeros(), encontrados = 0;
    REGISTOS temp = NULL; // Criamos uma lista temporária
    size_t size = lista->pessoa.id; // Vamos buscar ao Header o tamanho da Lista
    PACIENTES paciente = lista->prox; // Saltamos o Header
    // Vamos percorrer todas as listas de registos
    for (size_t i = 0; i < size; i++){
        REGISTOS registro = paciente->pessoa.pessoa_registo;
        while (registro!= NULL) {
            if (registro->reg.tensao_max > limite) {
                encontrados = 1;
                REGISTOS new_node = (REGISTOS) malloc(sizeof(bloco_registo));
                new_node->reg = registro->reg;
                new_node->reg.tensao_min = paciente->pessoa.id; // Usamos a tensao mínima para armazenar o ID na lista auxiliar
                new_node->prox = temp;
                temp = new_node;
            }
            registro = registro->prox;
        }
        paciente = paciente->prox;
    }
    // Ordenamos todas as Tensões independentemente do ID do paciente
    REGISTOS current = temp;
    REGISTOS next = NULL;
    while (current!= NULL) {
        next = current->prox;
        while (next!= NULL) {
            if (current->reg.tensao_max < next->reg.tensao_max) {
                registo temp = current->reg;
                current->reg = next->reg;
                next->reg = temp;
            }
            next = next->prox;
        }
        current = current->prox;
    }
    // Imprimimos a Lista
    current = temp;
    while (current!= NULL) {
        printf("\nTensão Máxima: %d do paciente com ID: %d", current->reg.tensao_max, current->reg.tensao_min);
        current = current->prox;
    }
    // Libertamos a memória
    while (temp!= NULL) {
        REGISTOS next = temp->prox;
        free(temp);
        temp = next;
    }
    if (!encontrados) printf("\nNenhum paciente com tensão máxima acima de %d encontrado.\n", limite);
    else printf("\n");
}

// Case 5 : Novo Registo Clínico 
void novo_registo(PACIENTES lista) {
    printf("\nQual o ID do paciente para adicionar um novo registo? ");
    int id = input_numeros();
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        registo novo;
        printf("Qual a data do registo (Formato : dia / mes / ano) ? ");
        novo.data_registo = input_data();
        printf("Qual a tensão máxima ? ");
        novo.tensao_max = input_numeros();
        printf("Qual a tensão mínima ? ");
        novo.tensao_min = input_numeros();
        printf("Qual o peso ? ");
        novo.peso = input_numeros();
        printf("Qual a altura ? ");
        novo.altura = input_numeros();
        insere_registo(novo, lista, id); // Inserimos o novo registo no nódulo do paciente
        save_registros(lista); // Guardamos a informação no ficheiro dos registos
        printf("\nNovo registo adicionado!\n");
    } else {
        printf("\nPaciente com ID %d não encontrado.\n", id);
    }
}

// Case 6 : Listar toda a informação de um paciente
void listar_informacao_paciente(PACIENTES lista) {
    printf("\nQual o ID do paciente? ");
    int id = input_numeros();
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
        if (registro == NULL)
            printf("\nPaciente com ID %d não tem Registos Clínicos \n", id);
        else{
            registro = registro->prox; // Saltamos o Header
            while (registro != NULL) {
                printf("\nData do Registo: %d/%d/%d\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
                printf("Tensão Máxima: %d\n", registro->reg.tensao_max);
                printf("Tensão Mínima: %d\n", registro->reg.tensao_min);
                printf("Peso: %d\n", registro->reg.peso);
                printf("Altura: %d\n", registro->reg.altura);
                registro = registro->prox;
            }
        }
    } 
    else {
        printf("\nPaciente com ID %d não encontrado.\n", id);
    }
}

/////////////////////////////// Função Running ///////////////////////////////

void running(PACIENTES informacao) {
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
        printf("\nQual a operação pretendida?\n> ");
        int choice = input_numeros();
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
            case 7:   // Sair do programa
                printf("\n!!! Programa Terminado !!!\n\n");
                return;
            default:
                printf("\n!!! Caso não possível !!!\n");
                break;
        }
    }
}