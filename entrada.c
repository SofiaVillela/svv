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

int contarClientes(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir %s.\n", nomeArquivo);
        return -1;
    }

    char linha[1000];
    int contadorClientes = 0;
    while (fgets(linha, sizeof(linha), file) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            continue;
        }
        contadorClientes++;
        printf("%s\n", linha);
    }
    fclose(file);

    if (contadorClientes == 0) {
        fprintf(stderr, "Erro: %s está vazio.\n", nomeArquivo);
        return -1;
    }

    return contadorClientes;
}
