#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/estruturas.h"
#include "../lib/declaracoes.h"

/////////////////////////////// Funcionalidades da aplicação ///////////////////////////////

// Case 1 : Novo Doente
void novo_paciente(PACIENTES informacao) {
    info novo;
    printf(GREEN"\n\n>>>>> [Registar Paciente Novo na Database HUC-S_MED] <<<<<\n");
    printf("                Nome do paciente: ");
    char* str1 = input_strings(0);
    strcpy(novo.nome,str1);
    free(str1);
    printf("--------------------------------------------------------------\n");
    printf("       Data de nascimento do paciente [DD/MM/AAAA]: ");
    novo.data_nascimento = input_data();
    printf("--------------------------------------------------------------\n");
    printf("Cartão de Cidadão do Paciente [Formato 12345678-1-AB1]: ");
    char* str2 = input_strings(2);
    strcpy(novo.cartao_de_cidadao,str2);
    free(str2);
    printf("--------------------------------------------------------------\n");
    printf("              NºTelefone do Paciente ? ");
    novo.telefone = input_numeros(1);
    printf("--------------------------------------------------------------\n");
    printf("               Qual o email do paciente ? ");
    char* str3 = input_strings(1);
    strcpy(novo.email,str3);
    free(str3);
    informacao->pessoa.id++;  // Aumentamos o número de pacientes no header
    novo.id = 1;
    while(find_id(informacao,novo.id) != NULL) // Caso o id já exista, continuamos a incrementar até conseguirmos um ID único
        novo.id++;
    novo.pessoa_registo =  NULL;
    insere_pacientes(informacao, novo); // Inserimos o novo paciente na estrutura
    save_pacientes(informacao); // Guardamos a informação no ficheiro dos pacientes
    printf("--------------------------------------------------------------\n");
    printf(">>>>> [Paciente Registado com Sucesso na Database HUC-S_MED!] <<<<<\n");
}

// Case 2 : Eliminar Doente Existente
void elimina_pacientes(PACIENTES lista) {
    printf(RED"\n\n>>>>> [Remover Paciente da Database HUC-S_MED] <<<<<\n");
    printf("Insira o ID do Paciente que pretende eliminar: ");
    int id = input_numeros(0);
    PACIENTES ant = lista, atual = lista->prox;
    while (atual!= NULL && atual->pessoa.id!= id) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual!= NULL) {
        lista->pessoa.id--; // Tiramos ao número Total de Pacientes
        if (atual->pessoa.pessoa_registo != NULL)
            destroi_registos(atual->pessoa.pessoa_registo);
        ant->prox = atual->prox;
        free(atual);
        printf("--------------------------------------------------------------\n");
        printf("Paciente com ID Nº%d Eliminado com Sucesso!\n", id);
        printf("--------------------------------------------------------------\n");
    } else {
        printf("...Paciente com ID Nº%d não encontrado...\n", id);
    }
    save_pacientes(lista); // Guardamos a informação no ficheiro dos pacientes
    save_registos(lista);  // Guardamos a informação no ficheiro dos registos
}

// Case 3 : Consultar Doentes - Ordem Alfabética
void imprime(PACIENTES lista){
    printf(MAGENTA"\n\n>>>>> [Lista de Pacientes na Database HUC-S_MED (A-Z)] <<<<<\n");
    size_t size = lista->pessoa.id; // Vamos buscar ao Header o tamanho da Lista
    PACIENTES aux = lista->prox; // Ignoramos o Header
    printf("--------------------------------------------------------------\n");
    for (size_t i = 0; i < size; i++){
        printf("                 Paciente ID Nº%d: %s\n",aux->pessoa.id, aux->pessoa.nome); // Mostramos o Nome
        printf("--------------------------------------------------------------\n");
        aux = aux->prox;
    }
}

// Case 4 : Consultar Doentes - Tensão Máxima
void listar_tensoes_acima(PACIENTES lista) {
    printf(MAGENTA"\n\n>>>>> [Lista de Tensão dos Pacientes HUC-S_MED] <<<<<\n");
    printf("Valor limite da Tensão Maxima: ");
    int limite = input_numeros(0), encontrados = 0;
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
    printf("--------------------------------------------------------------\n");
    while (current!= NULL) {
        printf("          Tensão Máxima: %d do paciente com ID Nº%d\n", current->reg.tensao_max, current->reg.tensao_min);
        printf("--------------------------------------------------------------\n");
        current = current->prox;
    }
    // Libertamos a memória
    while (temp!= NULL) {
        REGISTOS next = temp->prox;
        free(temp);
        temp = next;
    }
    if (!encontrados) printf("\n...0 Pacientes com Tensão Maxima > %d encontrados...\n", limite);
    else printf(">>>>> [Lista de Tensão dos Pacientes da Database HUC-S_MED] <<<<<\n");
}

// Case 5 : Novo Registo Clínico 
void novo_registo(PACIENTES lista) {
    printf(GREEN"\n\n>>>>> [Inserir Novo Registo Clinico] <<<<<\n");
    printf("ID do paciente: ");
    int id = input_numeros(0);
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        registo novo;
        printf("\n\n--------------------------------------------------------------\n");
        printf("      Data do registo [DD/MM/AAAA]: ");
        novo.data_registo = input_data();
        do {
            printf("--------------------------------------------------------------\n");
            printf("             Tensão Maxima: ");
            novo.tensao_max = input_numeros(0);
            printf("--------------------------------------------------------------\n");
            printf("             Tensão Minima: ");
            novo.tensao_min = input_numeros(0);
            printf("--------------------------------------------------------------\n");
            if (novo.tensao_min > novo.tensao_max) { //nao permitir que tensao min seja maior que a max
                printf("... A Tensão Minima não pode ser maior que a Tensão Maxima! Introduza Novamente...\n");
            }
        } while (novo.tensao_min > novo.tensao_max);
        printf("               Peso: ");
        novo.peso = input_numeros(0);
        printf("--------------------------------------------------------------\n");
        printf("              Altura: ");
        novo.altura = input_numeros(0);
        printf("--------------------------------------------------------------\n");
        insere_registos(novo, lista, id); // Inserimos o novo registo no nódulo do paciente
        save_registos(lista); // Guardamos a informação no ficheiro dos registos
        printf(">>>>> [Paciente Registado com Sucesso na Database HUC-S_MED!] <<<<<\n");
    } else
        printf("\n...Paciente com ID Nº%d não encontrado...\n", id);
}

// Case 6 : Listar toda a informação de um paciente
void listar_informacao_paciente(PACIENTES lista) {
    printf(MAGENTA"\n\n>>>>> [Informação Clinica Paciente] <<<<<\n");
    printf("ID do paciente: ");
    int id = input_numeros(0);
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        printf("\n--------------------------------------------------------------\n");
        printf("            Informação do Paciente com ID Nº%d     \n\n", paciente->pessoa.id);
        printf("Nome: %s\n", paciente->pessoa.nome);
        printf("Data de Nascimento: %d/%d/%d\n", paciente->pessoa.data_nascimento.dia, paciente->pessoa.data_nascimento.mes, paciente->pessoa.data_nascimento.ano);
        printf("Cartão de Cidadão: %s\n", paciente->pessoa.cartao_de_cidadao);
        printf("Telefone: %d\n", paciente->pessoa.telefone);
        printf("Email: %s\n", paciente->pessoa.email);
        printf("--------------------------------------------------------------\n");
        REGISTOS registro = paciente->pessoa.pessoa_registo;
        if (registro == NULL)
            printf("\n...Paciente com ID Nº%d não tem Registos Clínicos... \n", id);
        else{
            registro = registro->prox; // Saltamos o Header
            while (registro != NULL) {
                printf("                       Registo Médico    \n\n");
                printf("Data do Registo: %d/%d/%d\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
                printf("Tensão Máxima: %d\n", registro->reg.tensao_max);
                printf("Tensão Mínima: %d\n", registro->reg.tensao_min);
                printf("Peso: %d\n", registro->reg.peso);
                printf("Altura: %d\n", registro->reg.altura);
                printf("--------------------------------------------------------------\n");
                registro = registro->prox;
            }
        }
    } 
    else
        printf("\n ...Paciente com ID %d não encontrado...\n", id);
}


// Case 7 : Número Total de Pacientes e de Registos
void numero_total(PACIENTES info){
    int size = info->pessoa.id; // Número Total de pacientes armazenado no Header
    printf(YELLOW"\n\n>>>>> [Número Total de Pacientes] <<<<<\n");
    printf("Número Total de Pacientes: %d\n", size);
    PACIENTES paciente = info->prox; // Saltamos o Header
    for (int i = 0; i < size; i++){
        if (paciente->pessoa.pessoa_registo != NULL)
            printf("Paciente com ID Nº%d tem %d registos\n", paciente->pessoa.id, paciente->pessoa.pessoa_registo->reg.peso);
        else
            printf("Paciente com ID Nº%d não tem registos\n", paciente->pessoa.id);
        paciente = paciente->prox;   
    }        
    printf(">>>>> [Número Total de Pacientes] <<<<<\n\n");
}

/////////////////////////////// Função Running ///////////////////////////////

void running(PACIENTES informacao) {
    while (1) {
        printf(CYAN"\n\n////// [Hospital da Universidade de Coimbra]  //////\n");
        printf(GREEN"///   1. Novo Paciente                           ///\n");
        printf(RED"///   2. Eliminar Paciente Existente             ///\n");
        printf(MAGENTA"///   3. Consultar Pacientes - Ordem Alfabética  ///\n");
        printf(MAGENTA"///   4. Consultar Pacientes - Tensão            ///\n");
        printf(GREEN"///   5. Novo Registo Clínico                    ///\n");
        printf(MAGENTA"///   6. Consultar Pacientes - Listar Paciente   ///\n");
        printf(YELLOW"///   7. Número Total de Pacientes               ///\n");
        printf(RED"///   8. Sair                                    ///\n");
        printf(CYAN"////// [Hospital da Universidade de Coimbra]  //////\n");
        printf(CYAN"\nQual a operação pretendida?\n> ");
        int choice = input_numeros(0);
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
            case 7:   // Número Total de Pacientes e de Registos
                numero_total(informacao);
                break;    
            case 8:   // Sair do programa
                printf(RED"\n >>> Programa Terminado <<<\n\n"RESET);
                return;
            default:
                printf("\n...Input Não Reconhecido...\n");
                break;
        }
    }
}