#include <stdio.h>
#include <stdlib.h>
#include "entrada.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Erro: numero de argumentos invalido\n");
        return 1;
    }
    int quantidadeTipoRecurso = argc - 1;

    int *available = lerArgumentos(argc, argv);
    if (available == NULL) {
        return 1;
    }

    int quantidadeClientes = contarClientes("customer.txt",quantidadeTipoRecurso);
    if (quantidadeClientes == -1) {
        free(available);
        return 1;
    }

    printf("recursos: %d | clientes: %d\n", quantidadeTipoRecurso, quantidadeClientes);

    free(available);
    return 0;
}