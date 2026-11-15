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
    if (validarComandos("commands.txt", &banco) == 1) {
        liberarBanco(&banco);
        return 1;
    }
        // ---------- TESTES DO REQUEST ----------
    int resultado;

    // Teste 1: cliente 0 pede 0 2 1 → esperado 0 (alocado)
    int pedido1[] = {0, 2, 1};
    resultado = request(&banco, 0, pedido1);
    printf("Teste 1: retorno %d (esperado 0)\n", resultado);
    printf("  available: %d %d %d (esperado 10 3 6)\n",
           banco.available[0], banco.available[1], banco.available[2]);
    printf("  allocation[0]: %d %d %d (esperado 0 2 1)\n",
           banco.allocation[0][0], banco.allocation[0][1], banco.allocation[0][2]);
    printf("  need[0]: %d %d %d (esperado 7 3 2)\n",
           banco.need[0][0], banco.need[0][1], banco.need[0][2]);

    // Teste 2: cliente 2 pede 0 1 0 → need dele é 9 0 2 → esperado 1 (excede o máximo)
    int pedido2[] = {0, 1, 0};
    resultado = request(&banco, 2, pedido2);
    printf("Teste 2: retorno %d (esperado 1)\n", resultado);

    // Teste 3: mais três pedidos válidos do exemplo → esperado 0 em todos
    int pedido3a[] = {3, 0, 2};
    int pedido3b[] = {2, 0, 0};
    int pedido3c[] = {0, 0, 2};
    printf("Teste 3a: retorno %d (esperado 0)\n", request(&banco, 2, pedido3a));
    printf("Teste 3b: retorno %d (esperado 0)\n", request(&banco, 1, pedido3b));
    printf("Teste 3c: retorno %d (esperado 0)\n", request(&banco, 4, pedido3c));
    printf("  available: %d %d %d (esperado 5 3 2)\n",
           banco.available[0], banco.available[1], banco.available[2]);

    // Teste 4: cliente 0 pede 6 0 0 → need 7 (ok), mas available 5 → esperado 2
    int pedido4[] = {6, 0, 0};
    resultado = request(&banco, 0, pedido4);
    printf("Teste 4: retorno %d (esperado 2)\n", resultado);
    printf("  available: %d %d %d (esperado 5 3 2, sem mudança)\n",
           banco.available[0], banco.available[1], banco.available[2]);
             // Teste 5: cliente 0 devolve 0 1 1 → esperado 0 (liberado)
    int devolve5[] = {0, 1, 1};
    resultado = release(&banco, 0, devolve5);
    printf("Teste 5: retorno %d (esperado 0)\n", resultado);
    printf("  available: %d %d %d (esperado 5 4 3)\n",
           banco.available[0], banco.available[1], banco.available[2]);
    printf("  allocation[0]: %d %d %d (esperado 0 1 0)\n",
           banco.allocation[0][0], banco.allocation[0][1], banco.allocation[0][2]);
    printf("  need[0]: %d %d %d (esperado 7 4 3)\n",
           banco.need[0][0], banco.need[0][1], banco.need[0][2]);

    // Teste 6: cliente 0 devolve 0 5 0 → só tem 1 do recurso B → esperado 1
    int devolve6[] = {0, 5, 0};
    resultado = release(&banco, 0, devolve6);
    printf("Teste 6: retorno %d (esperado 1)\n", resultado);
    printf("  available: %d %d %d (esperado 5 4 3, sem mudança)\n",
           banco.available[0], banco.available[1], banco.available[2]);

    imprimirEstado(&banco, stdout);
    liberarBanco(&banco);   
    return 0;
}

