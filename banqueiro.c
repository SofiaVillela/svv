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

int estadoSeguro(Banco *banco){
    int *work = malloc(banco->numeroDeRecursos * sizeof(int));
    int *finish = malloc(banco->numeroDeClientes * sizeof(int));

    if(work == NULL || finish == NULL){
        free(work);
        free(finish);
        fprintf(stderr, "Erro ao alocar vetor\n");
        return 0; //inseguro
    }

    for(int i = 0; i < banco->numeroDeRecursos; i++){
        work[i] = banco->available[i];
    }
    for(int i = 0; i < banco->numeroDeClientes; i++){
        finish[i] = 0;
    }

    int alguemTerminou = 1; 
    while(alguemTerminou){
        alguemTerminou = 0;
        for(int i = 0; i < banco->numeroDeClientes; i++){
            if(finish[i] == 1){
                continue;
            }
            int quantidadeMenores = 0;
            for(int j = 0; j < banco->numeroDeRecursos; j++){
                if(work[j] >= banco->need[i][j]){
                    quantidadeMenores++;
                }
            }
            if(quantidadeMenores == banco->numeroDeRecursos){
                for(int j = 0; j< banco->numeroDeRecursos; j++){
                    work[j] = work[j] + banco->allocation[i][j];
                }
                finish[i] = 1;
                alguemTerminou =1 ;
            }
        }
    }
    int seguro =0;
    for(int i = 0; i < banco->numeroDeClientes; i++){
        if(finish[i] == 1) seguro++;
    }
    int retornar = 0  ;
    if(seguro == banco->numeroDeClientes) retornar = 1;

    free(work);
    free(finish);
   
    return retornar;
}

int request(Banco *banco, int idCliente, int *recursosPedidos){
    for(int j = 0; j < banco->numeroDeRecursos; j++){
        if(recursosPedidos[j] > banco->need[idCliente][j]){
            return 1;

        }
    }
    for(int j = 0; j < banco->numeroDeRecursos; j++){
        if(recursosPedidos[j] > banco->available[j]){
            return 2;

        }
    }

    for(int j = 0; j < banco->numeroDeRecursos; j++){
        banco->available[j] = banco->available[j] - recursosPedidos[j];
        banco->allocation[idCliente][j] = banco->allocation[idCliente][j] + recursosPedidos[j];
        banco->need[idCliente][j] = banco->need[idCliente][j] -recursosPedidos[j];
    }

    if(estadoSeguro(banco) == 1) return 0;
    
    for(int j = 0; j < banco->numeroDeRecursos; j++){
        banco->available[j] = banco->available[j] + recursosPedidos[j];
        banco->allocation[idCliente][j] = banco->allocation[idCliente][j] - recursosPedidos[j];
        banco->need[idCliente][j] = banco->need[idCliente][j] + recursosPedidos[j];
    }
    return 3;
}

int release(Banco *banco, int idCliente, int *recursosRetorna){
    for(int j = 0; j < banco->numeroDeRecursos; j++){
        if(recursosRetorna[j] > banco->allocation[idCliente][j]){
            return 1;
        }
    }
    for(int j = 0; j < banco->numeroDeRecursos; j++){
        banco->available[j] = banco->available[j] + recursosRetorna[j];
        banco->allocation[idCliente][j] = banco->allocation[idCliente][j] - recursosRetorna[j];
        banco->need[idCliente][j] = banco->need[idCliente][j] + recursosRetorna[j];
    }
    return 0;
}