#include "../lib/estruturas.h"
#include "../lib/declaracoes.h"

int main() {
    PACIENTES lista = load_pacientes(); // Fazemos load da informação dos pacientes se existir, caso contrário criamos a lista e o ficheiro
    load_registros(lista);              // Fazemos o mesmo para o ficheiro dos registos
    running(lista);                     // Função responsável pelas opções do programa
    return 0;
}