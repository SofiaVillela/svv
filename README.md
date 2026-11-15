# Implementação 5 — Algoritmo do Banqueiro
 
Algoritmo do banqueiro em C. Disciplina Infraestrutura de Software (CESAR School).
 
## Como usar
 
```bash
make              # compila e gera o executável banker
./banker 10 5 7   # cada número é o total de um tipo de recurso
make clean        # apaga o executável e o result.txt
```
 
O programa lê `customer.txt` e `commands.txt` da pasta atual e grava o resultado em `result.txt`.
 
- `customer.txt`: uma linha por cliente, com o máximo de cada recurso separado por vírgula.
- `commands.txt`: um comando por linha. `RQ` pede recursos, `RL` libera recursos e `*` grava o estado atual.
## Arquivos
 
- `main.c`: chama as etapas e libera a memória.
- `entrada.c`: lê e valida os argumentos e os arquivos; executa os comandos.
- `banqueiro.c`: struct `Banco`, algoritmo de segurança, pedido, liberação e impressão do estado.
## Erros tratados
 
O programa mostra uma mensagem e encerra quando:
 
- não há argumentos, ou algum argumento não é um inteiro não negativo;
- `customer.txt` ou `commands.txt` não existe ou não abre;
- `customer.txt` está vazio, tem valor inválido ou tem número de colunas diferente do número de argumentos;
- `commands.txt` tem comando desconhecido, cliente inexistente, valor inválido ou número de valores diferente do número de recursos.
## Decisões
 
- Liberação maior que o alocado: negada, sem alterar nada.
- `commands.txt` vazio: aceito, nenhum comando é executado.
- Linhas vazias são ignoradas.
- `make clean` também apaga o `result.txt`.
- Com muitos recursos ou números grandes, as colunas da tabela podem desalinhar.
## Testes
 
- Com os arquivos de exemplo, o `result.txt` é idêntico ao do professor.
- Testado com 2 e 4 tipos de recurso e com cada caso de erro.
- Sem vazamentos de memória no `valgrind`.
- Registro em `evidencias.log`.
 