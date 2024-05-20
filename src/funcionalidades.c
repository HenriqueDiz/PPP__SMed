#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/estruturas.h"
#include "../lib/declaracoes.h"

/////////////////////////////// Funcionalidades da aplicação ///////////////////////////////

// Case 1 : Adicionar novo paciente
void novo_paciente(PACIENTES informacao) {
    info novo;
    printf(RED"\n\n\n>>>>> [Registar Paciente Novo na Database HUC-S_MED] <<<<<"RESET"\n");
    printf(RED"                Nome do paciente: ");
    char* str1 = input_strings(0);
    strcpy(novo.nome,str1);
    free(str1);
    printf(RED"--------------------------------------------------------------"RESET"\n");
    printf(RED"       Data de nascimento do paciente [DD/MM/AAAA]: ");
    novo.data_nascimento = input_data();
    printf(RED"--------------------------------------------------------------"RESET"\n");
    printf(RED"Cartão de Cidadão do Paciente [Formato 12345678-1-AB1]: ");
    char* str2 = input_strings(2);
    strcpy(novo.cartao_de_cidadao,str2);
    free(str2);
    printf(RED"--------------------------------------------------------------"RESET"\n");
    printf(RED"              NºTelefone do Paciente ? ");
    novo.telefone = input_numeros(1);
    printf(RED"--------------------------------------------------------------"RESET"\n");
    printf(RED"               Qual o email do paciente ? ");
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
    printf(RED"--------------------------------------------------------------"RESET"\n");
    printf(RED">>>>> [Paciente Registado com Sucesso na Database HUC-S_MED!] <<<<<"RESET"\n");
}

// Case 2 : Eliminar Doente Existente
void elimina_pacientes(PACIENTES lista) {
    printf(YELLOW"\n\n\n>>>>> [Remover Paciente da Database HUC-S_MED] <<<<<"RESET"\n");
    printf(YELLOW"Insira o ID do Paciente que pretende eliminar: ");
    int id = input_numeros(0);
    PACIENTES ant = lista, atual = lista->prox;
    while (atual!= NULL && atual->pessoa.id!= id) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual!= NULL) {
        lista->pessoa.id--; // Tiramos ao número Total de Pacientes
        if (atual->pessoa.pessoa_registo != NULL)
            destroi_registo(atual->pessoa.pessoa_registo);
        ant->prox = atual->prox;
        free(atual);
        printf(YELLOW"--------------------------------------------------------------"RESET"\n");
        printf(YELLOW"Paciente com ID Nº%d Eliminado com Sucesso!"RESET"\n", id);
        printf(YELLOW"--------------------------------------------------------------"RESET"\n");
    } else {
        printf("...Paciente com ID Nº%d não encontrado...\n", id);
    }
    save_pacientes(lista); // Guardamos a informação no ficheiro dos pacientes
    save_registros(lista); // Guardamos a informação no ficheiro dos registos
}

// Case 3 : Consultar Doentes (Ordem Alfabética)
void imprime(PACIENTES lista){
    printf(BLUE"\n\n\n>>>>> [Lista de Pacientes na Database HUC-S_MED (A-Z)] <<<<<"RESET"\n");
    size_t size = lista->pessoa.id; // Vamos buscar ao Header o tamanho da Lista
    PACIENTES aux = lista->prox; // Ignoramos o Header
    printf(BLUE"--------------------------------------------------------------"RESET"\n");
    for (size_t i = 0; i < size; i++){
        printf(BLUE"                 Paciente ID Nº%d: %s"RESET"\n",aux->pessoa.id, aux->pessoa.nome); // Mostramos o Nome
        printf(BLUE"--------------------------------------------------------------"RESET"\n");
        aux = aux->prox;
    }
}

// Case 4 : Consultar Doentes - Tensão
void listar_tensoes_acima(PACIENTES lista) {
    printf(GREEN"\n\n\n>>>>> [Lista de Tensão dos Pacientes HUC-S_MED] <<<<<"RESET"\n");
    printf(GREEN"Valor limite da Tensão Maxima: ");
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
    printf(GREEN"--------------------------------------------------------------"RESET"\n");
    while (current!= NULL) {
        printf(GREEN"          Tensão Máxima: %d do paciente com ID Nº%d"RESET"\n", current->reg.tensao_max, current->reg.tensao_min);
        printf(GREEN"--------------------------------------------------------------"RESET"\n");
        current = current->prox;
    }
    // Libertamos a memória
    while (temp!= NULL) {
        REGISTOS next = temp->prox;
        free(temp);
        temp = next;
    }
    if (!encontrados) printf("\n...0 Pacientes com Tensão Maxima > %d encontrados...\n", limite);
    else printf(GREEN">>>>> [Lista de Tensão dos Pacientes da Database HUC-S_MED] <<<<<"RESET"\n");
}

// Case 5 : Novo Registo Clínico 
void novo_registo(PACIENTES lista) {
    printf(MAGENTA"\n\n\n>>>>> [Inserir Novo Registo Clinico] <<<<<"RESET"\n");
    printf(MAGENTA"ID do paciente: ");
    int id = input_numeros(0);
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        registo novo;
        printf(MAGENTA"\n\n--------------------------------------------------------------"RESET"\n");
        printf(MAGENTA"      Data do registo [DD/MM/AAAA]: ");
        novo.data_registo = input_data();
        do {
            printf(MAGENTA"--------------------------------------------------------------"RESET"\n");
            printf(MAGENTA"             Tensão Maxima: ");
            novo.tensao_max = input_numeros(0);
            printf(MAGENTA"--------------------------------------------------------------"RESET"\n");
            printf(MAGENTA"             Tensão Minima: ");
            novo.tensao_min = input_numeros(0);
            printf(MAGENTA"--------------------------------------------------------------"RESET"\n");
            if (novo.tensao_min > novo.tensao_max) { //nao permitir que tensao min seja maior que a max
                printf("... A Tensão Minima não pode ser maior que a Tensão Maxima! Introduza Novamente...\n");
            }
        } while (novo.tensao_min > novo.tensao_max);
        printf(MAGENTA"               Peso: ");
        novo.peso = input_numeros(0);
        printf(MAGENTA"--------------------------------------------------------------"RESET"\n");
        printf(MAGENTA"              Altura: ");
        novo.altura = input_numeros(0);
        printf(MAGENTA"--------------------------------------------------------------"RESET"\n");
        insere_registo(novo, lista, id); // Inserimos o novo registo no nódulo do paciente
        save_registros(lista); // Guardamos a informação no ficheiro dos registos
        printf(MAGENTA">>>>> [Paciente Registado com Sucesso na Database HUC-S_MED!] <<<<<"RESET"\n");
    } else
        printf("\n...Paciente com ID Nº%d não encontrado...\n", id);
}

// Case 6 : Listar toda a informação de um paciente
void listar_informacao_paciente(PACIENTES lista) {
    printf(CYAN"\n\n\n>>>>> [Informação Clinica Paciente] <<<<<"RESET"\n");
    printf(CYAN"ID do paciente: ");
    int id = input_numeros(0);
    PACIENTES paciente = find_id(lista,id);
    if (paciente != NULL) {
        printf(CYAN"\n--------------------------------------------------------------"RESET"\n");
        printf(CYAN"            Informação do Paciente com ID Nº%d     "RESET"\n\n", paciente->pessoa.id);
        printf(CYAN"Nome: %s"RESET"\n", paciente->pessoa.nome);
        printf(CYAN"Data de Nascimento: %d/%d/%d"RESET"\n", paciente->pessoa.data_nascimento.dia, paciente->pessoa.data_nascimento.mes, paciente->pessoa.data_nascimento.ano);
        printf(CYAN"Cartão de Cidadão: %s"RESET"\n", paciente->pessoa.cartao_de_cidadao);
        printf(CYAN"Telefone: %d"RESET"\n", paciente->pessoa.telefone);
        printf(CYAN"Email: %s"RESET"\n", paciente->pessoa.email);
        printf(CYAN"--------------------------------------------------------------"RESET"\n");
        REGISTOS registro = paciente->pessoa.pessoa_registo;
        if (registro == NULL)
            printf("\n...Paciente com ID Nº%d não tem Registos Clínicos... \n", id);
        else{
            registro = registro->prox; // Saltamos o Header
            while (registro != NULL) {
                printf(CYAN"                       Registo Médico    "RESET"\n\n");
                printf(CYAN"Data do Registo: %d/%d/%d"RESET"\n", registro->reg.data_registo.dia, registro->reg.data_registo.mes, registro->reg.data_registo.ano);
                printf(CYAN"Tensão Máxima: %d"RESET"\n", registro->reg.tensao_max);
                printf(CYAN"Tensão Mínima: %d"RESET"\n", registro->reg.tensao_min);
                printf(CYAN"Peso: %d"RESET"\n", registro->reg.peso);
                printf(CYAN"Altura: %d"RESET"\n", registro->reg.altura);
                printf(CYAN"--------------------------------------------------------------"RESET"\n");
                registro = registro->prox;
            }
        }
    } 
    else
        printf("\n ...Paciente com ID %d não encontrado...\n", id);
}


// Case 7 : Número de Pacientes
void numero_total(PACIENTES info){
    int size = info->pessoa.id; // Número Total de pacientes armazenado no Header
    printf(BRIGHT_GREEN"\n\n>>>>> [Número Total de Pacientes] <<<<<"RESET"\n");
    printf(BRIGHT_GREEN"Número Total de Pacientes: %d\n", size);
    PACIENTES paciente = info->prox; // Saltamos o Header
    for (int i = 0; i < size; i++){
        if (paciente->pessoa.pessoa_registo != NULL)
            printf(BRIGHT_GREEN"Paciente com ID Nº%d tem %d registos\n", paciente->pessoa.id, paciente->pessoa.pessoa_registo->reg.peso);
        else
            printf(BRIGHT_GREEN"Paciente com ID Nº%d não tem registos\n", paciente->pessoa.id);
        paciente = paciente->prox;   
    }        
    printf(BRIGHT_GREEN">>>>> [Número Total de Pacientes] <<<<<\n\n");
}

/////////////////////////////// Função Running ///////////////////////////////

void running(PACIENTES informacao) {
    while (1) {
        printf("\n\n\n////// [Hospital da Universidade de Coimbra]  //////\n");
        printf(RED"///   1. Adicionar novo Doente                   ///"RESET"\n");
        printf(YELLOW"///   2. Eliminar Doente Existente               ///"RESET"\n");
        printf(BLUE"///   3. Consultar Doentes (Ordem Alfabética)    ///"RESET"\n");
        printf(GREEN"///   4. Consultar Doentes - Tensão              ///"RESET"\n");
        printf(MAGENTA"///   5. Novo Registo Clínico                    ///"RESET"\n");
        printf(CYAN"///   6. Listar toda a informação de um paciente ///"RESET"\n");
        printf(BRIGHT_GREEN"///   7. Número Total de Pacientes e de Registos ///"RESET"\n");
        printf(BRIGHT_RED"///   8. Sair                                    ///"RESET"\n");
        printf("////// [Hospital da Universidade de Coimbra]  //////\n");
        printf("\nQual a operação pretendida?\n> ");
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
                printf(BRIGHT_RED"\n >>> Programa Terminado <<<"RESET"\n\n");
                return;
            default:
                printf("\n...Input Não Reconhecido...\n");
                break;
        }
    }
}
