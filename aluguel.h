#ifndef ALUGUEL_H_INCLUDED
#define ALUGUEL_H_INCLUDED

#include "filme.h"
#include "cliente.h"
#include <string.h>
#include <stdio.h>
#include <locale.h> 

typedef struct{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct {
    int id_aluguel;
    char cpf_cliente[12];
    int id_filme;
    Data dataDevolucao;
}Aluguel;

typedef struct {
    int quantidade; // Quantidade de clientes cadastrados
    int capacidade; // Capacidade atual do vetor
    Aluguel *alugueis; // ["1","10", "17", "20"]
}BibliotecaAluguel;

void libera_biblioteca(BibliotecaAluguel *lib_alugueis);
void print_biblioteca_aluguel(BibliotecaAluguel *lib_alugueis); // Função para mostrar todos os alugueis

int verifica_data(Data *data);

void iniciaBiblotecaAluguel (BibliotecaAluguel *lib_alugueis, int quantidade);
void realoca_biblioteca_aluguel(BibliotecaAluguel *lib_alugueis);   // Função para realocar memória
int get_indice_aluguel (BibliotecaAluguel *lib_alugueis, int codigo); // Função para retornar o índice de um aluguel

void carrega_alugueis_do_arquivo(BibliotecaAluguel *lib_alugueis); // Função para carregar os alugueis do arquivo
void salva_alugueis_no_arquivo(BibliotecaAluguel *lib_alugueis); // Função para salvar os alugueis no arquivo

void cadastra_aluguel(BibliotecaAluguel *lib_alugueis, BibliotecaFilme *lib_filmes, BibliotecaCliente *lib_clientes); // Função para cadastrar um aluguel
void deleta_aluguel(BibliotecaAluguel *lib_alugueis); // Função para deletar um aluguel
void atualiza_aluguel(BibliotecaAluguel *lib_alugueis); // Função para atualizar um aluguel
void consulta_cpf_aluguel(BibliotecaAluguel *lib_alugueis); // Função para consultar um aluguel por id_filme
void consultar_id_filme (BibliotecaAluguel *lib_alugueis); // Função para consultar um aluguel por id_filme

void gerar_relatorio(BibliotecaAluguel *lib_alugueis, BibliotecaCliente *lib_clientes, BibliotecaFilme *lib_filmes); // Função para gerar um relatório

void deleta_filme(BibliotecaFilme *lib_filmes, BibliotecaAluguel *lib_alugueis);

#include "aluguel.c"

#endif // ALUGUEL_H_INCLUDED
