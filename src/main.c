#include "estruturas.h"

int main() {
    PACIENTES lista = load_pacientes(); // Fazemos load da informação dos pacientes se existir, caso contrário criamos a lista e o ficheiro
    load_registros(lista);              // Fazemos o mesmo para o ficheiro dos registos
    running(lista);                     // Função responsável pelas opções do programa
    save_pacientes(lista);              // Guardamos a informação no ficheiro dos pacientes 
    save_registros(lista);              // Guardamos a informação no ficheiro dos registos
    return 0;
}