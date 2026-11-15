#ifndef ENTRADA_H
#define ENTRADA_H
#include "banqueiro.h"

int *lerArgumentos(int argc, char **argv);
int contarClientes(const char *nomeArquivo, int quantidadeTipoRecurso);
int preencheMatriz(const char *nomeArquivo, Banco *banco);
int validarComandos(const char *nomeArquivo, Banco *banco);
int executarComandos(const char *nomeArquivo, Banco *banco);
#endif