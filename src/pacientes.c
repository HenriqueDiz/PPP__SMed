#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/declaracoes.h"
#include "../lib/estruturas.h"

/////////////////////////////// Ficheiro Pacientes ///////////////////////////////

PACIENTES cria_pacientes(){
    PACIENTES aux;
    info novo = {{1,1,2000},"11111111-1-AB1","header@header.pt","Header", 0, 0, NULL};  // Definimos a informações do Header e iremos usar o 'id' para armazenar o número de pacientes
    aux = (PACIENTES) malloc (sizeof(bloco));
    if (aux != NULL) {
        aux->pessoa = novo;
        aux->prox= NULL;
    }
    return aux;
}

void procura_paciente(PACIENTES lista, char* chave_nome, PACIENTES* ant, PACIENTES* actual){
    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) < 0) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && strcasecmp((*actual)->pessoa.nome, chave_nome) != 0)
        *actual = NULL;   /* Paciente não encontrado*/
}

void insere_pacientes(PACIENTES lista, info novo) {
    PACIENTES no, ant, inutil;
    no = (PACIENTES) malloc (sizeof(bloco));
    if (no != NULL) {
        no->pessoa = novo;
        procura_paciente(lista, novo.nome, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

PACIENTES destroi_pacientes(PACIENTES lista){
    PACIENTES temp;
    size_t size = lista->pessoa.id; // Vamos buscar o tamanho ao Header
    for (size_t i = 0; i < size; i++){
        temp = lista;
        if (temp->pessoa.pessoa_registo != NULL) destroi_registo(temp->pessoa.pessoa_registo);
        lista = lista->prox;
        free (temp);
    }
    free(lista);
    return NULL;
}

PACIENTES load_pacientes() {
    FILE *ficheiro = fopen("docs/doentes.txt", "r");
    if (ficheiro == NULL) {
        printf("\nFicheiro doentes.txt não encontrado, sendo criado novo (Processo: Loading)\n");        
        ficheiro = fopen("docs/doentes.txt", "w");
    }
    PACIENTES lista_pacientes = cria_pacientes();
    info dados;
    while (fscanf(ficheiro, "%d", &dados.id) == 1) {
        fscanf(ficheiro, "%d", &dados.id);
        fgets(dados.nome, 40, ficheiro);
        dados.nome[strcspn(dados.nome, "\n")] = '\0';
        dados.nome[strcspn(dados.nome, "\r")] = '\0'; // Para Mac e Linux 
        fscanf(ficheiro, "%d/%d/%d", &dados.data_nascimento.dia, &dados.data_nascimento.mes, &dados.data_nascimento.ano);
        fscanf(ficheiro, "%s", dados.cartao_de_cidadao);
        fscanf(ficheiro, "%d", &dados.telefone);
        fscanf(ficheiro, "%s", dados.email);
        lista_pacientes->pessoa.id++; // Estamos a adicionar ao id do header para sabermos quantos doentes estão armazenados
        dados.pessoa_registo = NULL;
        insere_pacientes(lista_pacientes, dados);
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
