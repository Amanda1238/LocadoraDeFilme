#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <string.h>
#include <stdio.h>


typedef struct {
    char cpf[12];
    char nome[100];
    int idade;
    int numAlugueis;
}Cliente;

typedef struct {
    int quantidade; // Quantidade de clientes cadastrados
    int capacidade; // Capacidade atual do vetor
    Cliente *clientes; // ["1","10", "17", "20"]
}BibliotecaCliente;

void iniciaBiblotecaCliente(BibliotecaCliente *lib_clientes, int quantidade);
void realoca_biblioteca(BibliotecaCliente *lib_clientes);
int get_indice_cliente(BibliotecaCliente *lib_clientes, char cpf[12]);

void carrega_clientes_do_arquivo(BibliotecaCliente *lib_clientes);
void salva_clientes_no_arquivo(BibliotecaCliente *lib_clientes);

void cadastra_cliente(BibliotecaCliente *lib_clientes);
void consulta_cliente(BibliotecaCliente *lib_clientes);
void atualiza_cliente(BibliotecaCliente *lib_clientes);
void deleta_cliente(BibliotecaCliente *lib_clientes);
void print_biblioteca(BibliotecaCliente *lib_clientes);




#include "cliente.c"

#endif // CLIENTE_H_INCLUDED
