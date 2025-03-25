#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cliente.h"
#include "filme.h"
#include "aluguel.h"

// Mostrar menu principal
int mostrar_menu_principal(){
    int op;
    printf("\n==================== CineAluga =======================\n");
    printf("\n1- Cadastrar, Alterar, Consultar, Remover Cliente ");
    printf("\n2- Cadastrar, Alterar, Consultar, Remover Filme ");
    printf("\n3- Cadastrar, Alterar, Remover Alugueis ");
    printf("\n4- Consultar todos os clientes");
    printf("\n5- Consultar todos os filmes");
    printf("\n6- Consultar todos os alugueis");
    printf("\n7- Gerar relatorio");
    printf("\n8- Sair");
    printf("\n=====================================================\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &op);
    return op;
}
int mostrar_opcao_cliente(){
    int op;
    printf("\n=============== Clientes ===============");
    printf("\n1- Cadastrar");
    printf("\n2- Alterar");
    printf("\n3- Consultar");
    printf("\n4- Remover");
    printf("\n========================================\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &op);
    return op;

}
int mostrar_opcao_filme(){
    int op;
    printf("\n=============== Filmes ===============");
    printf("\n1- Cadastrar");
    printf("\n2- Alterar");
    printf("\n3- Consultar");
    printf("\n4- Remover");
    printf("\n========================================\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &op);
    return op;

}
int mostrar_opcao_aluguel(){
    int op;
    printf("\n=============== Alugueis ===============");
    printf("\n1- Cadastrar");
    printf("\n2- Alterar");
    printf("\n3- Remover");
    printf("\n========================================\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &op);
    return op;

}
int mostrar_consulta_aluguel(){
    int op;
    printf("\n=============== Consulta Alugueis ===============");
    printf("\n1- CPF");
    printf("\n2- Codigo do aluguel");
    printf("\n=================================================\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &op);
    return op;

}
void invalidar(){
    printf("\n op��o invalida");
}

void seleciona_main_menu(BibliotecaAluguel *lib_alugueis, BibliotecaCliente *lib_clientes, BibliotecaFilme *lib_filmes){
    int op, menu_cliente,  menu_filme, menu_aluguel, consulta_aluguel;
    do{
        op =  mostrar_menu_principal();
        switch(op){
            case 1:
                menu_cliente = mostrar_opcao_cliente();
                switch(menu_cliente){
                    case 1:
                        cadastra_cliente(lib_clientes);
                        break;
                    case 2:
                        atualiza_cliente(lib_clientes);
                        break;
                    case 3:
                        consulta_cliente(lib_clientes);
                        break;
                    case 4:
                        deleta_cliente(lib_clientes);
                        break;
                    default:
                        invalidar();
                        break;
                    }
                break;
            case 2:
                menu_filme = mostrar_opcao_filme();
                switch(menu_filme){
                    case 1:
                        cadastra_filme(lib_filmes);
                        break;
                    case 2:
                        atualiza_filme(lib_filmes);
                        break;
                    case 3:
                        consulta_filme(lib_filmes);
                        break;
                    case 4:
                        deleta_filme(lib_filmes, lib_alugueis);
                        break;
                    default:
                        invalidar();
                        break;
                    }
                break;
            case 3:
                menu_aluguel = mostrar_opcao_aluguel();
                switch(menu_aluguel){
                    case 1:
                        cadastra_aluguel(lib_alugueis, lib_filmes, lib_clientes);
                        break;
                    case 2:
                        atualiza_aluguel(lib_alugueis);
                        break;
                    case 3:
                        deleta_aluguel(lib_alugueis);
                        break;
                    default:
                        invalidar();
                        break;
                    }
                break;
            case 4:
                print_biblioteca(lib_clientes);
                break;
            case 5:
                print_biblioteca_filme(lib_filmes);
                break;
            case 6:
                print_biblioteca_aluguel(lib_alugueis);
                break;
            case 7:
                gerar_relatorio(lib_alugueis, lib_clientes, lib_filmes);
                break;
            case 8:
                break;
            default:
                invalidar();
                break;
        }
    }while(op != 8);
}