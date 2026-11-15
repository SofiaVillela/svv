#ifndef BANQUEIRO_H
#define BANQUEIRO_H

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

#endif