#include <stdio.h>
#include <stdlib.h>
#include "../lib/declaracoes.h"
#include "../lib/estruturas.h"

/////////////////////////////// Ficheiro Registos ///////////////////////////////

void insere_registo(bloco_registo registo, PACIENTES lista_pacientes, int id) {
    PACIENTES paciente = find_id(lista_pacientes,id);
    if (paciente != NULL) {
        if (paciente->pessoa.pessoa_registo == NULL) { // ou seja, é o primeiro registo
            paciente->pessoa.pessoa_registo = (REGISTOS) malloc(sizeof(bloco_registo));
            paciente->pessoa.pessoa_registo->reg = registo.reg;
            paciente->pessoa.pessoa_registo->prox = NULL;
        } else {                                       // ou seja, não é o primeiro registo
            REGISTOS novo_registo = (REGISTOS) malloc(sizeof(bloco_registo)); 
            novo_registo->reg = registo.reg;
            novo_registo->prox = paciente->pessoa.pessoa_registo;
            paciente->pessoa.pessoa_registo = novo_registo;
        }
    } else {
        printf("Paciente com ID %d não encontrado\n", id);
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
        bloco_registo lista_registo;
        fscanf(ficheiro, "%d/%d/%d", &lista_registo.reg.data_registo.dia, &lista_registo.reg.data_registo.mes, &lista_registo.reg.data_registo.ano);
        fscanf(ficheiro, "%d", &lista_registo.reg.tensao_max);
        fscanf(ficheiro, "%d", &lista_registo.reg.tensao_min);
        fscanf(ficheiro, "%d", &lista_registo.reg.peso);
        fscanf(ficheiro, "%d", &lista_registo.reg.altura);
        insere_registo(lista_registo, lista_pacientes, id);
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