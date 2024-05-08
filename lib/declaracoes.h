#include "../lib/estruturas.h"

#ifndef DECLARACOES_H
#define DECLARACOES_H

void limpar_buffer();
int verifica_numeros(const char* input);

void running(PACIENTES informacao);

PACIENTES load_pacientes();
void save_pacientes(PACIENTES lista_pacientes);
void insere_pacientes(PACIENTES lista, info novo);

void load_registros(PACIENTES lista_pacientes);
void save_registros(PACIENTES lista_pacientes);
void insere_registo(bloco_registo registo, PACIENTES lista_pacientes, int id);

#endif