#ifndef FILME_H_INCLUDED
#define FILME_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct{
	int codigo;
	char titulo[100];
	char genero[100];
	float preco;
	bool disponibilidade;
} Filme;

typedef struct {
    int quantidade; // Quantidade de clientes cadastrados
    int capacidade; // Capacidade atual do vetor
    Filme *filmes; // ["1","10", "17", "20"]
}BibliotecaFilme;

void iniciaBiblotecaFilme (BibliotecaFilme *lib_filmes, int quantidade);
void realoca_biblioteca_filme(BibliotecaFilme *lib_filmes);
int get_indice_filme(BibliotecaFilme *lib_filmes, int codigo);
int verifica_filmes_disponiveis(BibliotecaFilme *lib_filmes);

void carrega_filmes_do_arquivo(BibliotecaFilme *lib_filmes);
void salva_filmes_no_arquivo(BibliotecaFilme *lib_filmes);

void cadastra_filme(BibliotecaFilme *lib_filmes);
void consulta_filme(BibliotecaFilme *lib_filmes);
void atualiza_filme(BibliotecaFilme *lib_filmes);

void print_biblioteca_filme (BibliotecaFilme *lib_filmes);

#include "filme.c"
#endif // FILME_H_INCLUDED
