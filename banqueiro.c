#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banqueiro.h"

// Libera uma matriz com 'linhas' linhas. Aceita NULL.
static void liberarUmaMatriz(int **matriz, int linhas) {
    if (matriz == NULL) {
        return;
    }
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Aloca uma matriz linhas x colunas.
// Retorna NULL se falhar, sem deixar nada alocado.
static int **alocarUmaMatriz(int linhas, int colunas) {
    int **matriz = malloc(linhas * sizeof(int *));
    if (matriz == NULL) {
        return NULL;
    }
    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(int));
        if (matriz[i] == NULL) {
            liberarUmaMatriz(matriz, i);   // libera as linhas 0 até i-1
            return NULL;
        }
    }
    return matriz;
}

int alocarMatrizes(Banco *banco) {
    int linhas = banco->numeroDeClientes;
    int colunas = banco->numeroDeRecursos;

    banco->maximum = alocarUmaMatriz(linhas, colunas);
    if (banco->maximum == NULL) {
        fprintf(stderr, "Erro ao alocar matriz maximum.\n");
        return 1;
    }

    banco->allocation = alocarUmaMatriz(linhas, colunas);
    if (banco->allocation == NULL) {
        fprintf(stderr, "Erro ao alocar matriz allocation.\n");
        liberarUmaMatriz(banco->maximum, linhas);
        banco->maximum = NULL;
        return 1;
    }

    banco->need = alocarUmaMatriz(linhas, colunas);
    if (banco->need == NULL) {
        fprintf(stderr, "Erro ao alocar matriz need.\n");
        liberarUmaMatriz(banco->maximum, linhas);
        liberarUmaMatriz(banco->allocation, linhas);
        banco->maximum = NULL;
        banco->allocation = NULL;
        return 1;
    }

    return 0;
}

void liberarBanco(Banco *banco) {
    liberarUmaMatriz(banco->maximum, banco->numeroDeClientes);
    liberarUmaMatriz(banco->allocation, banco->numeroDeClientes);
    liberarUmaMatriz(banco->need, banco->numeroDeClientes);
    free(banco->available);

    banco->maximum = NULL;
    banco->allocation = NULL;
    banco->need = NULL;
    banco->available = NULL;
}