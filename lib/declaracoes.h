#include "../lib/estruturas.h"

#ifndef DECLARACOES_H
#define DECLARACOES_H

#define RESET   "\x1b[0m"
#define YELLOW  "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RED   "\x1b[91m"
#define GREEN "\x1b[92m"

void limpar_buffer();
int verifica_numeros(const char* input);
PACIENTES find_id(PACIENTES lista,int id);
int input_numeros(int flag);
char* input_strings(int flag);
data input_data();
int comparar_datas(data d1, data d2);

void running(PACIENTES informacao);

PACIENTES load_pacientes();
void save_pacientes(PACIENTES lista_pacientes);
void insere_pacientes(PACIENTES lista, info novo);
PACIENTES destroi_pacientes(PACIENTES lista);

void load_registos(PACIENTES lista_pacientes);
void save_registos(PACIENTES lista_pacientes);
void insere_registos(registo novo, PACIENTES lista, int id);
REGISTOS destroi_registos(REGISTOS lista);


#endif