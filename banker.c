#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) { 
    if(argc < 2) {
        fprintf(stderr, "Erro: numero de argumentos invalido\n");
        return 1;
    }
    int quantidadeTipoRecurso = argc - 1;


    int *available = malloc(quantidadeTipoRecurso * sizeof(int));
    if (available == NULL) {
        fprintf(stderr,"Erro na alocação de memória.\n");
        return 1;
    }

    char *fim;
    long valor;
    for(int i = 0; i < quantidadeTipoRecurso; i++){
        valor = strtol(argv[i + 1], &fim, 10);
        if (fim == argv[i + 1]) { 
            fprintf(stderr,"Erro: %s não é um número.\n", argv[i+1]);
            free(available);
            return 1;
        }
        
        if (*fim != '\0') {
            fprintf(stderr,"Erro: '%s' contém texto após o número.\n", argv[i+1]);
            free(available);
            return 1;
        }
        if(valor < 0){
            fprintf(stderr,"Erro: %ld é um número negativo.\n", valor);
            free(available);
            return 1;
        }
        available[i] = valor;
    }
    return 0;
}