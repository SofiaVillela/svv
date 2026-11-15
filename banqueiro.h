#ifndef BANQUEIRO_H
#define BANQUEIRO_H
#include <stdio.h>

typedef struct {
    int numeroDeRecursos;
    int numeroDeClientes;
    int *available;
    int **maximum;
    int **allocation;
    int **need;
} Banco;

int alocarMatrizes(Banco *banco);
void liberarBanco(Banco *banco);
int estadoSeguro(Banco *banco);
int request(Banco *banco, int idCliente, int *recursosPedidos);
int release(Banco *banco, int idCliente, int *recursosPedidos);
void imprimirEstado(Banco *banco, FILE *file);

#endif