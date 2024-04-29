#include "estruturas.h"

int main() {
    PACIENTES informacao = cria();
    REGISTOS lista_registos = lerRegistos("registos.txt"); 
    info* listadoentes = lerDoentes("doentes.txt"); 
    running(informacao);
    imprime(informacao);
    return 0;
}