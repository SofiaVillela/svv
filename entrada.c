#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"

int *lerArgumentos(int argc, char **argv) {
    int quantidadeTipoRecurso = argc - 1;

    int *available = malloc(quantidadeTipoRecurso * sizeof(int));
    if (available == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        return NULL;
    }

    char *fim;
    long valor;
    for (int i = 0; i < quantidadeTipoRecurso; i++) {
        valor = strtol(argv[i + 1], &fim, 10);
        if (fim == argv[i + 1]) {
            fprintf(stderr, "Erro: %s não é um número.\n", argv[i + 1]);
            free(available);
            return NULL;
        }
        if (*fim != '\0') {
            fprintf(stderr, "Erro: '%s' contém texto após o número.\n", argv[i + 1]);
            free(available);
            return NULL;
        }
        if (valor < 0) {
            fprintf(stderr, "Erro: %ld é um número negativo.\n", valor);
            free(available);
            return NULL;
        }
        available[i] = valor;
    }

    return available;
}

int contarClientes(const char *nomeArquivo, int quantidadeTipoRecurso) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir %s.\n", nomeArquivo);
        return -1;
    }

    char linha[1000];
    int contadorClientes = 0;
    int numeroLinha = 0;

    while (fgets(linha, sizeof(linha), file) != NULL) {
        numeroLinha++;

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            continue;
        }

        if (linha[strlen(linha) - 1] == ',') {
            fprintf(stderr, "Erro: linha %d de %s termina com vírgula.\n", numeroLinha, nomeArquivo);
            fclose(file);
            return -1;
        }

        contadorClientes++;
        char *token = strtok(linha, ",");
        int colunas = 0;

        while(token != NULL){
            char *fim;
            long valor = strtol(token, &fim, 10);

            if(fim == token|| *fim != '\0'||valor < 0){
                fprintf(stderr, "Erro: valor '%s' inválido na linha %d de %s.\n", token, numeroLinha, nomeArquivo);
                fclose(file);
                return -1;
            }

            colunas++;
            token = strtok(NULL, ",");
        }
        if(colunas != quantidadeTipoRecurso){
            fprintf(stderr, "Erro: linha %d de %s tem %d colunas, esperado %d.\n", numeroLinha, nomeArquivo, colunas, quantidadeTipoRecurso);
            fclose(file);
            return -1;
        }
    }
    fclose(file);

    if (contadorClientes == 0) {
        fprintf(stderr, "Erro: %s está vazio.\n", nomeArquivo);
        return -1;
    }

    return contadorClientes;
}

int preencheMatriz(const char*nomeArquivo, Banco *banco){
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir %s.\n", nomeArquivo);
        return 1;
    }
    char linha[1000];
    int cliente = 0;

    while (fgets(linha, sizeof(linha), file) != NULL) {
        int recurso = 0;
        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            continue;
        }
        
        char *token = strtok(linha, ",");
        while(token != NULL){
            banco->maximum[cliente][recurso]= strtol(token, NULL, 10);
            banco->allocation[cliente][recurso] = 0;
            banco->need[cliente][recurso] = banco->maximum[cliente][recurso] ;

            token = strtok(NULL, ",");
            recurso++;
        }
        cliente++;
    }
    fclose(file);
    return 0;
}

int validarComandos(const char *nomeArquivo, Banco *banco){
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir %s.\n", nomeArquivo);
        return 1;
    }

    char linha[1000];
    int numeroLinha = 0;

    while (fgets(linha, sizeof(linha), file) != NULL) {
        numeroLinha++;

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            continue;
        }

        char *token = strtok(linha, " ");
        if (token == NULL) {
            continue;   // linha só com espaços
        }

        if (strcmp(token, "*") == 0) {
            // '*' precisa estar sozinho na linha
            if (strtok(NULL, " ") != NULL) {
                fprintf(stderr, "Erro: linha %d de %s: '*' deve estar sozinho na linha.\n", numeroLinha, nomeArquivo);
                fclose(file);
                return 1;
            }
            continue;
        }

        if (strcmp(token, "RQ") != 0 && strcmp(token, "RL") != 0) {
            fprintf(stderr, "Erro: linha %d de %s: comando '%s' desconhecido.\n", numeroLinha, nomeArquivo, token);
            fclose(file);
            return 1;
        }

        token = strtok(NULL, " ");
        if (token == NULL) {
            fprintf(stderr, "Erro: linha %d de %s: falta o id do cliente.\n", numeroLinha, nomeArquivo);
            fclose(file);
            return 1;
        }
        char *fim;
        long cliente = strtol(token, &fim, 10);
        if (fim == token || *fim != '\0') {
            fprintf(stderr, "Erro: linha %d de %s: id do cliente '%s' inválido.\n", numeroLinha, nomeArquivo, token);
            fclose(file);
            return 1;
        }

        if (cliente < 0 || cliente >= banco->numeroDeClientes) {
            fprintf(stderr, "Erro: linha %d de %s: cliente %ld não existe (válidos: 0 a %d).\n", numeroLinha, nomeArquivo, cliente, banco->numeroDeClientes - 1);
            fclose(file);
            return 1;
        }

        // 3º token em diante: valores dos recursos
        int quantidadeValores = 0;
        token = strtok(NULL, " ");
        while (token != NULL) {
            long valor = strtol(token, &fim, 10);
            if (fim == token || *fim != '\0' || valor < 0) {
                fprintf(stderr, "Erro: linha %d de %s: valor '%s' inválido.\n", numeroLinha, nomeArquivo, token);
                fclose(file);
                return 1;
            }
            quantidadeValores++;
            token = strtok(NULL, " ");
        }

        if (quantidadeValores != banco->numeroDeRecursos) {
            fprintf(stderr, "Erro: linha %d de %s tem %d valores, esperado %d.\n", numeroLinha, nomeArquivo, quantidadeValores, banco->numeroDeRecursos);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

static void escreverVetor(FILE *saida, int *vetor, int tamanho) {
    for (int j = 0; j < tamanho; j++) {
        if (j == 0) {
            fprintf(saida, "%d", vetor[j]);
        } else {
            fprintf(saida, " %d", vetor[j]);
        }
    }
}

int executarComandos(const char *nomeArquivo, Banco *banco){
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir %s.\n", nomeArquivo);
        return 1;
    }

    FILE *resultado = fopen("result.txt", "w");
    if (resultado == NULL) {
        fprintf(stderr, "Erro: não foi possível criar result.txt.\n");
        fclose(file);
        return 1;
    }

    int *valores = malloc(banco->numeroDeRecursos * sizeof(int));
    if (valores == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        fclose(file);
        fclose(resultado);
        return 1;
    }
    char linha[1000];

    while (fgets(linha, sizeof(linha), file) != NULL) {

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            continue;
        }

        char *token = strtok(linha, " ");
        if (token == NULL) {
            continue;   // linha só com espaços
        }

        if(strcmp(token, "*") == 0){
            imprimirEstado(banco, resultado);
            continue;
        }
        int ehRequest = (strcmp(token, "RQ") == 0);
        token = strtok(NULL, " ");
    
        int idCliente = strtol(token, NULL, 10);

        for(int i = 0; i < banco->numeroDeRecursos; i++){
            token = strtok(NULL, " ");
            valores[i] = strtol(token, NULL, 10);
        }
        int retorno;
        if(ehRequest) retorno =  request(banco, idCliente, valores);
        else retorno = release(banco, idCliente, valores);
        int n = banco->numeroDeRecursos;

        if (ehRequest) {
            if (retorno == 0) {
                fprintf(resultado, "Allocate to customer %d the resources ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, "\n");
            } else if (retorno == 1) {
                fprintf(resultado, "The customer %d request ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, " was denied because exceed its maximum allocation\n");
            } else if (retorno == 2) {
                fprintf(resultado, "The resources ");
                escreverVetor(resultado, banco->available, n);
                fprintf(resultado, " are not enough to customer %d request ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, " \n");
            } else if (retorno == 3) {
                fprintf(resultado, "The customer %d request ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, " was denied because result in an unsafe state\n");
            }
        } else {
            if (retorno == 0) {
                fprintf(resultado, "Release from customer %d the resources ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, "\n");
            } else {
                fprintf(resultado, "The customer %d release ", idCliente);
                escreverVetor(resultado, valores, n);
                fprintf(resultado, " was denied because exceed its current allocation\n");
            }
        }
    }
    fclose(file);
    fclose(resultado);
    free(valores);
    return 0;
}