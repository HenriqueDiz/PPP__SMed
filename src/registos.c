#include <stdio.h>
#include <stdlib.h>
#include "../lib/declaracoes.h"
#include "../lib/estruturas.h"

/////////////////////////////// Ficheiro Registos ///////////////////////////////

// Funcão para Criar a Lista de Registos
REGISTOS cria_registos(){
    REGISTOS aux;
    registo novo = {{0,0,0},0,0,0,0};  // Definimos a informações do Header e iremos usar o 'peso' para armazenar o número de registos
    aux = (REGISTOS) malloc (sizeof(bloco_registo));
    if (aux != NULL) {
        aux->reg = novo;
        aux->prox = NULL;
    }
    return aux;
}

// Função para Ordenar todos os registos dos mais antigos para os mais recentes na Lista de Registos
void procura_registo(REGISTOS lista, data epoca, REGISTOS* ant, REGISTOS* actual){
    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && comparar_datas((*actual)->reg.data_registo, epoca) < 0) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && comparar_datas((*actual)->reg.data_registo, epoca) != 0)
        *actual = NULL; 
}

// Função para Inserirmos o Registo na Lista de Registos
void insere_registos(registo novo, PACIENTES lista, int id) {
    PACIENTES paciente = find_id(lista,id); // Procuramos pelo nódulo do paciente
    if (paciente->pessoa.pessoa_registo == NULL) // Se não houver registos, criamos uma Lista de Registos para esse paciente
        paciente->pessoa.pessoa_registo = cria_registos();
    REGISTOS no, ant, inutil;
    no = (REGISTOS) malloc (sizeof(bloco_registo));
    if (no != NULL) {
        no->reg = novo;
        paciente->pessoa.pessoa_registo->reg.peso++; // Adicionamos ao Header para controlarmos o número de Registos do Paciente
        procura_registo(paciente->pessoa.pessoa_registo, novo.data_registo, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

// Função para Destruir a Lista de Registos
REGISTOS destroi_registos(REGISTOS lista){
    REGISTOS temp;
    size_t size = lista->reg.peso; // Vamos buscar o tamanho ao Header
    for (size_t i = 0; i < size; i++){
        temp = lista;
        lista = lista->prox;
        free (temp);
    }
    free(lista);
    return NULL;
}

// Função para Carregar a informação do Ficheiro de Texto na Lista de Registos
void load_registos(PACIENTES lista_pacientes) {
    FILE* ficheiro = fopen("docs/registos.txt", "r");
    if (ficheiro == NULL) {
        printf("Ficheiro registos.txt não encontrado, sendo criado novo (Processo: Loading)\n"); 
        ficheiro = fopen("docs/registos.txt", "w");
    }
    int id;
    while (fscanf(ficheiro, "%d", &id) == 1) {
        registo ficha;
        fscanf(ficheiro, "%d/%d/%d", &ficha.data_registo.dia, &ficha.data_registo.mes, &ficha.data_registo.ano);
        fscanf(ficheiro, "%d", &ficha.tensao_max);
        fscanf(ficheiro, "%d", &ficha.tensao_min);
        fscanf(ficheiro, "%d", &ficha.peso);
        fscanf(ficheiro, "%d", &ficha.altura);
        insere_registos(ficha, lista_pacientes, id);
    }
    if (fclose(ficheiro) == EOF){
        printf("Erro ao fechar o ficheiro registos.txt (Processo: Loading)\n");        
        exit(1);
    }
}

// Função para Escrever a informação da Lista de Registos no Ficheiro de Texto
void save_registos(PACIENTES lista_pacientes) {
    FILE* ficheiro = fopen("docs/registos.txt", "w");
    if (ficheiro == NULL) {
        printf("Erro ao fechar o ficheiro registos.txt (Processo: Saving)\n");   
        exit(1);
    }
    PACIENTES paciente = lista_pacientes;
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