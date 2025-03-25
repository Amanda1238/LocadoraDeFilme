#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h> //usa par corrigir erros
#include "cliente.h"
#include "filme.h"
#include "aluguel.h"
#include "menu.c"

// definindo macros para limpar a tela e a entrada a depender do sistema
#ifdef _WIN32 // caso seja windows 32 bits
char command[] = "cls";
#define clean fflush // caso seja windows 64 bits
#elif _WIN64
char command[] = "cls";
#define clean fflush
#else
#include <stdio_ext.h> // importando ext caso nao for windows
char command[] = "clear";
#define clean __fpurge
#endif


int main()
{
    setlocale(LC_ALL,"Portuguese");
    // declaração
    BibliotecaCliente lib_clientes;
    BibliotecaFilme lib_filmes;
    BibliotecaAluguel lib_alugueis;

    // carrega arquivo
    carrega_clientes_do_arquivo(&lib_clientes);
    carrega_filmes_do_arquivo(&lib_filmes);
    carrega_alugueis_do_arquivo(&lib_alugueis);

    print_biblioteca(&lib_clientes);
    print_biblioteca_filme(&lib_filmes);
    print_biblioteca_aluguel(&lib_alugueis);

    seleciona_main_menu(&lib_alugueis, &lib_clientes, &lib_filmes);


    return 0;
}

