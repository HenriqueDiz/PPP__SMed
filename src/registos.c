#include <stdio.h>
#include <stdlib.h>
#include "../lib/declaracoes.h"
#include "../lib/estruturas.h"

/////////////////////////////// Ficheiro Registos ///////////////////////////////

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

int comparar_datas(data d1, data d2) {
    if (d1.ano < d2.ano) return -1;
    if (d1.ano > d2.ano) return 1;
    if (d1.mes < d2.mes) return -1;
    if (d1.mes > d2.mes) return 1;
    if (d1.dia < d2.dia) return -1;
    if (d1.dia > d2.dia) return 1;
    return 0;
}

// Ordenamos os registos dos mais antigos para os mais recentes
void procura_registo(REGISTOS lista, data epoca, REGISTOS* ant, REGISTOS* actual){
    *ant = lista; *actual = lista->prox;
    while ((*actual) != NULL && comparar_datas((*actual)->reg.data_registo, epoca) < 0) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if ((*actual) != NULL && comparar_datas((*actual)->reg.data_registo, epoca) != 0)
        *actual = NULL; 
}

void insere_registo(registo novo, PACIENTES lista, int id) {
    PACIENTES paciente = find_id(lista,id); // Procuramos pelo nódulo do paciente
    if (paciente->pessoa.pessoa_registo == NULL) // Se não houver registos, criamos uma Fila de Registos para esse paciente
        paciente->pessoa.pessoa_registo = cria_registos();
    REGISTOS no, ant, inutil;
    no = (REGISTOS) malloc (sizeof(bloco_registo));
    if (no != NULL) {
        no->reg = novo;
        procura_registo(paciente->pessoa.pessoa_registo, novo.data_registo, &ant, &inutil);
        no->prox = ant->prox;
        ant->prox = no;
    }
}

void load_registros(PACIENTES lista_pacientes) {
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
        insere_registo(ficha, lista_pacientes, id);
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