#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct{
    int dia, mes, ano;
} data;

typedef struct{
    data data_do_registo;
    int tensao_min, tensao_max, peso, altura;
} registo;

// Nódulo que guarda um registo de um paciente
typedef struct bloco_registo{
    registo ficha;
    struct bloco_registo* prox;
} bloco_registo;

// Lista que armazena todos os registos de um paciente
typedef bloco_registo* REGISTOS;

typedef struct{
    data data_nascimento;
    char cartao_de_cidadao[16], email[40], nome[40];
    int telefone, id;
} info;

// Nódulo que guarda as informações de um paciente e todos os seus registos (que é uma Fila)
typedef struct bloco{
    info pessoa;
    REGISTOS pessoa_registo;
    struct bloco* prox;
} bloco;

// Lista que armazena tudo de todos os pacientes
typedef bloco* PACIENTES;

void running();
PACIENTES cria();
void imprime(PACIENTES lista);

#endif