#include <stdio.h>
#include <stdlib.h>
#include "entrada.h"
#include "banqueiro.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Erro: numero de argumentos invalido\n");
        return 1;
    }
   
    
    Banco banco;
    banco.maximum = banco.allocation = banco.need = NULL;
    banco.numeroDeRecursos = argc - 1;

    banco.available = lerArgumentos(argc, argv);
    if (banco.available == NULL) {
        return 1;
    }

    banco.numeroDeClientes= contarClientes("customer.txt",banco.numeroDeRecursos);
    if (banco.numeroDeClientes == -1) {
        free(banco.available);
        return 1;
    }
    if(alocarMatrizes(&banco) == 1) {
        free(banco.available);
        return 1;
    }
    if(preencheMatriz("customer.txt",&banco) == 1) {
        free(banco.available);
        return 1;
    }
    printf("MAXIMUM\n");
    for (int i = 0; i < banco.numeroDeClientes; i++) {
        for (int j = 0; j < banco.numeroDeRecursos; j++) {
            printf("%d ", banco.maximum[i][j]);
        }
        printf("\n");
    }

    printf("ALLOCATION\n");
    for (int i = 0; i < banco.numeroDeClientes; i++) {
        for (int j = 0; j < banco.numeroDeRecursos; j++) {
            printf("%d ", banco.allocation[i][j]);
        }
        printf("\n");
    }

    printf("NEED\n");
    for (int i = 0; i < banco.numeroDeClientes; i++) {
        for (int j = 0; j < banco.numeroDeRecursos; j++) {
            printf("%d ", banco.need[i][j]);
        }
        printf("\n");
    }
    liberarBanco(&banco);
    return 0;
}