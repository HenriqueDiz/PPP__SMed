#include "../lib/estruturas.h"

#ifndef DECLARACOES_H
#define DECLARACOES_H

void limpar_buffer();
int verifica_numeros(const char* input);
PACIENTES find_id(PACIENTES lista,int id);
int input_numeros(int flag);
char* input_strings(int flag);
data input_data();

void running(PACIENTES informacao);

PACIENTES load_pacientes();
void save_pacientes(PACIENTES lista_pacientes);
void insere_pacientes(PACIENTES lista, info novo);
PACIENTES destroi_pacientes(PACIENTES lista);

void load_registros(PACIENTES lista_pacientes);
void save_registros(PACIENTES lista_pacientes);
void insere_registo(registo novo, PACIENTES lista, int id);
REGISTOS destroi_registo(REGISTOS lista);


#endif
