#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Estrutura para as Datas
typedef struct{
    int dia, mes, ano;
} data;

// Estrutura para o registo
typedef struct{
    data data_registo;
    int tensao_min, tensao_max, peso, altura;
} registo;

// Nódulo que guarda um registo de um paciente
typedef struct bloco_registo{
    registo reg;
    struct bloco_registo* prox;
} bloco_registo;

// Lista que armazena todos os registos de um paciente
typedef bloco_registo* REGISTOS;

// Estrutura com a informação de um paciente - informações pessoais e registos
typedef struct{
    data data_nascimento;
    char cartao_de_cidadao[15], email[51], nome[51];
    int telefone, id;
    REGISTOS pessoa_registo;
} info;

// Nódulo que guarda informações de um paciente
typedef struct bloco{
    info pessoa;
    struct bloco* prox;
} bloco;

// Lista que armazena tudo de todos os pacientes
typedef bloco* PACIENTES;

#endif