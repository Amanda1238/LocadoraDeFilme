#include "filme.h"
#include "aluguel.h"
#include "cliente.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <locale.h> //usa par corrigir erros

#define INTERVALO 10
#define FILME_ARQ "filmes.bin"
#define QTD_FILME_ARQ "filmes_qtd.bin"

///////////////////////////
//                       //
//       Auxiliares      //
//                       //
///////////////////////////

void iniciaBiblotecaFilme (BibliotecaFilme *lib_filmes, int quantidade){
    lib_filmes->quantidade = quantidade;
    lib_filmes->capacidade = quantidade + INTERVALO;
    lib_filmes->filmes = (Filme *)malloc(sizeof(Filme) * (quantidade + INTERVALO));
    if (lib_filmes->filmes == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    printf("alocou\n"); // não ultiliza so para verificar se entrou na função
}

void realoca_biblioteca_filme(BibliotecaFilme *lib_filmes){

    // exemplo
    // existe 9 filmes na base e a capacidade é 10
    // se adicionar mais um filme a capacidade vai ser aumentada para 20
    if (lib_filmes->quantidade + 1 >= lib_filmes->capacidade) {
        lib_filmes->capacidade += INTERVALO;
        lib_filmes->filmes = (Filme *)realloc(lib_filmes->filmes, lib_filmes->capacidade * sizeof(Filme));
        if (lib_filmes->filmes == NULL) {
            printf("Erro ao alocar memória!\n");
            return;
        }
        printf("realocou\n"); // não ultiliza so para verificar se entrou na função
    }
}

int get_indice_filme(BibliotecaFilme *lib_filmes, int codigo){
    for(int i=0; i < lib_filmes->quantidade ; i++){ // percorre os filmes cadastrados
        if(lib_filmes->filmes[i].codigo == codigo){ // compara o codigo do filme atual
            return i; // returna o indice do codigo procurado
            }
    }
    return -1; // C�digo n�o encontrado
}
int verifica_filmes_disponiveis(BibliotecaFilme *lib_filmes) {
    int disponiveis = 0;

    for (int i = 0; i < lib_filmes->quantidade; i++) {
        if (lib_filmes->filmes[i].disponibilidade) { // Se disponível
            printf("\nFilme disponível: Código %d - %s", lib_filmes->filmes[i].codigo, lib_filmes->filmes[i].titulo);
            disponiveis++;
        }
    }

    if (disponiveis == 0) {
        printf("\nNenhum filme disponível no momento.\n");
        return -1; // Retorna -1 se não houver filmes disponíveis
    }

    return disponiveis; // Retorna a quantidade de filmes disponíveis
}
///////////////////////////
//                       //
//       Arquivos        //
//                       //
///////////////////////////
void carrega_filmes_do_arquivo(BibliotecaFilme *lib_filmes){
    // struct do arquivo
    FILE *file_qtd_filmes = fopen(QTD_FILME_ARQ, "rb");

    int qtd_filme;

    // se o arquivo existir
    // se o arquivo n exister essa variavel é null ai n passa no if
    if (file_qtd_filmes) {

        //fread( qual_variavel, tamanho_da_variavel, quantidade_de_variaveis, arquivo);

        if (fread(&qtd_filme, sizeof(int), 1, file_qtd_filmes) != 1) {
            printf("Erro: Falha ao ler a quantidade de filmes do arquivo.\n");
            fclose(file_qtd_filmes);
            return;
        }
        fclose(file_qtd_filmes);
    } else{
        qtd_filme = 0;
    }
    printf("Quantidade de Filmes: %d\n", qtd_filme);

    iniciaBiblotecaFilme(lib_filmes, qtd_filme);

    FILE *file_filmes = fopen(FILME_ARQ, "rb");
    if (file_filmes) {
        if (fread(lib_filmes->filmes, sizeof(Filme), qtd_filme, file_filmes) != (size_t)qtd_filme) {
            printf("Erro: Falha ao ler os dados do arquivo de filmes.\n");
            free(lib_filmes->filmes);
            fclose(file_filmes);
            return;
        }
        fclose(file_filmes);
    }

}
void salva_filmes_no_arquivo(BibliotecaFilme *lib_filmes){

    FILE *file = fopen(FILME_ARQ, "wb");
    if (file) {
        fwrite(lib_filmes->filmes, sizeof(Filme), lib_filmes->capacidade, file);
        fclose(file);
    }

    FILE *file_qtd = fopen(QTD_FILME_ARQ, "wb");
    if (file) {
        fwrite(&lib_filmes->quantidade, sizeof(int), 1, file_qtd);
        fclose(file);
    }
}
///////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////

void cadastra_filme(BibliotecaFilme *lib_filmes){
    int verificacao, ehValido, ehFloat, code_filme, ehLetra, ehAlph, ehBool, verifica_bool;
    char entrada[10], entrada_code_filme[10], entrada2[10];

    // inicializa o vetor de struct se não tiver alocado
    if (lib_filmes->filmes == NULL) {
        iniciaBiblotecaFilme(lib_filmes, 0);
    }

    //----------------------------------- verificaçao codigo --------------------------------
    Filme novo_filme;

    do {
        ehValido = 1;

        printf("\nDigite o código do filme: ");
        scanf("%s", entrada_code_filme);  // Lê como string para verificar cada caractere


        // Verifica se a entrada contém apenas dígitos
        for (int i = 0; i < strlen(entrada_code_filme); i++) {
            if (!isdigit(entrada_code_filme[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O código deve conter apenas números! Tente novamente.\n");
        } else {
            code_filme = atoi(entrada_code_filme); // Converte para inteiro
            verificacao = get_indice_filme(lib_filmes, code_filme); //retorna -1 se não econtrado
            if(verificacao != -1){
                printf("\nErro: O codigo do filme não pode ser repetdido");
                ehValido = 0;
            }else{
                novo_filme.codigo = code_filme;
            }
        }

    } while (!ehValido);

//--------------------------------- verificacao titulo -----------------------------------------------
    do{
        ehLetra = 1;// Assume que é válido até encontrar um caractere inválido

        printf("Titulo do Filme: ");
        scanf(" %[^\n]", novo_filme.titulo);
        fflush(stdin);


        // Verifica se a entrada contém apenas letras e espaços
        for(int i =0; i < strlen(novo_filme.titulo); i++){
            if(!isalpha(novo_filme.titulo[i]) && novo_filme.titulo[i] != ' '){
                ehLetra = 0; // Se encontrar algo que não seja letra ou espaço, marca como inválido
                break; //para na hora
            }
        }
        if (!ehLetra) {
            printf("\nErro: O título deve conter apenas letras e espaços. Tente novamente.\n");
        }
    }while(!ehLetra);
//----------------------------------- verificacao genero ---------------------------------------------
    do{
        ehAlph = 1;


        printf("Genero do Filme: ");
        scanf(" %[^\n]", novo_filme.genero);
        fflush(stdin);

        for(int i=0;i<strlen(novo_filme.genero); i++){
            if(!isalpha(novo_filme.genero[i]) && novo_filme.genero[i] != ' '){
                ehAlph = 0; // Se encontrar algo que não seja letra ou espaço, marca como inválido
                break; //para na hora
            }

        }
        if (!ehAlph) {
            printf("\nErro: O gênero deve conter apenas letras e espaços. Tente novamente.\n");
        }


    }while(!ehAlph);
//----------------------------------- verificacao preco ---------------------------------------------


    // Verificação se preço é um numero de float
    do {
        ehValido = 1;

        printf("\nDigite o Pre�o do filme: ");
        if (scanf("%s", entrada2) != 1 || getchar() != '\n') {  // L� como string para verificar cada caractere e verifica se há caracteres extras
            ehValido = 0;
            while (getchar() != '\n'); // Limpa o buffer do stdin
        } else {
            // Verifica se a entrada cont�m apenas d�gitos e ponto
            int ponto_contador = 0;
            for (int i = 0; i < strlen(entrada2); i++) {
                if (!isdigit(entrada2[i])) {
                    if (entrada2[i] == '.' && ponto_contador == 0) {
                        ponto_contador++;
                    } else {
                        ehValido = 0; // Se encontrar algo que n�o seja n�mero ou mais de um ponto, marca como inv�lido
                        break;
                    }
                }
            }
        }

        if (!ehValido) {
            printf("\nErro: O pre�o deve conter apenas n�meros e no m�ximo um ponto! Tente novamente.\n");
        } else {
            novo_filme.preco = atof(entrada2); // Converte para float
        }

    } while (!ehValido);
// ---------------------------------------- verificacao disponibilidade ---------------------------------
        do{
            ehBool = 0;
            printf("Disponibilidade do Filme [true ou false]: ");
            scanf("%s", entrada);

            // Verifica se TODOS os caracteres da entrada são letras (sem números ou símbolos)
            for (int i = 0; i < strlen(entrada); i++) {
                if (!isalpha(entrada[i])) {
                    ehBool = 1; // Marca erro
                    break; // Para a verificação na hora
                }
            }

            if (ehBool == 0) { // Se a entrada tiver apenas letras, verifica se é "true" ou "false"
                if (strcmp(entrada, "true") == 0) {
                    novo_filme.disponibilidade = true;
                } else if (strcmp(entrada, "false") == 0) {
                    novo_filme.disponibilidade = false;
                } else {
                    printf("\nErro: A disponibilidade deve ser 'true' ou 'false'.\n");
                    ehBool = 1;
                }
            } else {
                printf("\nErro: A entrada deve conter apenas letras (sem números ou símbolos).\n");
            }

            while (getchar() != '\n'); // Limpa o buffer do stdin

        }while(ehBool != 0);

    // função que verifica se precisa reallocar memoria
    realoca_biblioteca_filme(lib_filmes);


    lib_filmes->filmes[lib_filmes->quantidade] = novo_filme;
    lib_filmes->quantidade++;

    printf("\nFilme cadastrado com sucesso!\n");
    salva_filmes_no_arquivo(lib_filmes);
}

///////////////////////////
//                       //
//       Consulta        //
//                       //
///////////////////////////
void consulta_filme(BibliotecaFilme *lib_filmes) {
    int id_filme, flag = 0;
    char entrada[10];
    int ehValido;

    do {
        ehValido = 1;
        printf("\nUse o codigo do filme para a pesquisa (ou sair para voltar ao menu): ");
        scanf("%s", entrada);  // Lê como string para verificar cada caractere
        while (getchar() != '\n'); // Limpa o buffer



        if(strcmp(entrada, "sair") == 0){
            printf("\nVoltando ao menu...\n");
            return;
        }

        // Verifica se a entrada contém apenas dígitos
        for (int i = 0; i < strlen(entrada); i++) {
            if (!isdigit(entrada[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O código deve conter apenas números! Tente novamente.\n");
        } else {
            id_filme = atoi(entrada); // Converte para inteiro
            if (id_filme <= 0) {
                printf("\nErro: O código deve ser um número positivo! Tente novamente.\n");
                ehValido = 0;
            }
        }

    } while (!ehValido);

    for (int i = 0; i < lib_filmes->quantidade; i++) {
        if (id_filme == lib_filmes->filmes[i].codigo) { // verifica se o filme existe e retorna a 1 se existir
            flag = 1;
            break;
        }
    }

    if (flag == 1) {
        for (int i = 0; i < lib_filmes->quantidade; i++) {
            if (id_filme == lib_filmes->filmes[i].codigo) {
                printf("\n============= Catalogo Filme ===================\n");
                printf("\nCodigo do filme: %d", lib_filmes->filmes[i].codigo);
                printf("\nTitulo: %s", lib_filmes->filmes[i].titulo);
                printf("\nGenero: %s", lib_filmes->filmes[i].genero);
                printf("\nPreço: %.2f", lib_filmes->filmes[i].preco);
                if (lib_filmes->filmes[i].disponibilidade) {
                    printf("\nFilme Disponível!\n");
                } else {
                    printf("\nFilme Indisponivel...\n");
                }
            }
        }
        printf("\nConsulta feita com sucesso!\n");
    } else {
        printf("\nCódigo não encontrado!\n"); // se o filme não existir
    }
}

///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_filme(BibliotecaFilme *lib_filmes){
    int ver_filme, op, ehLetra, indice, ehAlph, ehValido, ehBool;
    char entrada[10], entrada2[10];
    printf("\nDigite o codigo do Filme: ");
    scanf("%d", &ver_filme);
    // verifica se o filme ja existe e - 1 se não existir e o indice de existir
    indice = get_indice_filme(lib_filmes, ver_filme);

    if(indice == -1){
        printf("\nO codigo não existe!");
    }else{
        printf("\nEscolha uma opção para alteração: ");
        printf("\n\n================ Menu ===================\n");
        printf("\n1- Titulo ");
        printf("\n2- Genero ");
        printf("\n3- Preço ");
        printf("\n4- Disponibilidade");
        printf("\n5- Sair");
        printf("\n\n=========================================\n");
        scanf("%d", &op);
        switch(op){
            case 1:
                do{
                    ehLetra = 1;// Assume que é válido até encontrar um caractere inválido

                    printf("Titulo do Filme: ");
                    scanf(" %[^\n]", lib_filmes->filmes[indice].titulo);
                    fflush(stdin);


                    // Verifica se a entrada contém apenas letras e espaços
                    for(int i =0; i < strlen(lib_filmes->filmes[indice].titulo); i++){
                        if(!isalpha(lib_filmes->filmes[indice].titulo[i]) && lib_filmes->filmes[indice].titulo[i] != ' '){
                            ehLetra = 0; // Se encontrar algo que não seja letra ou espaço, marca como inválido
                            break; //para na hora
                        }
                    }
                    if (!ehLetra) {
                    printf("\nErro: O título deve conter apenas letras e espaços. Tente novamente.\n");
                    }
                }while(!ehLetra);
                break;
            case 2:
                do{
                    ehAlph = 1;


                    printf("Genero do Filme: ");
                    scanf(" %[^\n]", lib_filmes->filmes[indice].genero);
                    fflush(stdin);

                    for(int i=0;i<strlen(lib_filmes->filmes[indice].genero); i++){
                        if(!isalpha(lib_filmes->filmes[indice].genero[i]) && lib_filmes->filmes[indice].genero[i] != ' '){
                            ehAlph = 0; // Se encontrar algo que não seja letra ou espaço, marca como inválido
                            break; //para na hora
                        }

                    }
                    if (!ehAlph) {
                        printf("\nErro: O gênero deve conter apenas letras e espaços. Tente novamente.\n");
                    }


                }while(!ehAlph);
                break;
            case 3:
                do {
                    ehValido = 1;

                    printf("\nDigite o Preço do filme: ");
                    if (scanf("%s", entrada2) != 1 || getchar() != '\n') {  // L� como string para verificar cada caractere e verifica se há caracteres extras
                        ehValido = 0;
                        while (getchar() != '\n'); // Limpa o buffer do stdin
                    } else {
                        // Verifica se a entrada cont�m apenas d�gitos e ponto
                        int ponto_contador = 0;
                        for (int i = 0; i < strlen(entrada2); i++) {
                            if (!isdigit(entrada2[i])) {
                                if (entrada2[i] == '.' && ponto_contador == 0) {
                                    ponto_contador++;
                                } else {
                                    ehValido = 0; // Se encontrar algo que n�o seja n�mero ou mais de um ponto, marca como inv�lido
                                    break;
                                }
                            }
                        }
                    }

                    if (!ehValido) {
                        printf("\nErro: O preço deve conter apenas numeros e no maximo um ponto! Tente novamente.\n");
                    } else {
                        lib_filmes->filmes[indice].preco = atof(entrada2); // Converte para float
                    }

                } while (!ehValido);
                break;
            case 4:
                //fazer a vericação disponibilidade
                do{
                    ehBool = 0;
                    printf("Disponibilidade do Filme [true ou false]: ");
                    scanf("%s", entrada);

                    // Verifica se TODOS os caracteres da entrada são letras (sem números ou símbolos)
                    for (int i = 0; i < strlen(entrada); i++) {
                        if (!isalpha(entrada[i])) {
                            ehBool = 1; // Marca erro
                            break; // Para a verificação na hora
                        }
                    }

                    if (ehBool == 0) { // Se a entrada tiver apenas letras, verifica se é "true" ou "false"
                        if (strcmp(entrada, "true") == 0) {
                            lib_filmes->filmes[indice].disponibilidade = true;
                        } else if (strcmp(entrada, "false") == 0) {
                            lib_filmes->filmes[indice].disponibilidade = false;
                        } else {
                            printf("\nErro: A disponibilidade deve ser 'true' ou 'false'.\n");
                            ehBool = 1;
                        }
                    } else {
                        printf("\nErro: A entrada deve conter apenas letras (sem números ou símbolos).\n");
                    }

                    while (getchar() != '\n'); // Limpa o buffer do stdin

                }while(ehBool != 0);
                break;
            case 5:
                printf("\nSaindo...");
                break;
            default:
                printf("\nOpção invalida!");
                break;

        }
        salva_filmes_no_arquivo(lib_filmes);

    }


}

///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////

///////////////////////////
//                       //
//     Mostra todos      //
//                       //
///////////////////////////
// consulta dos os filmes
void print_biblioteca_filme (BibliotecaFilme *lib_filmes){
    printf("\n=============== INICIO LIB Filme ===============\n");
    for(int i=0; i < lib_filmes->quantidade ; i++){
        printf("Codigo: %d\n", lib_filmes->filmes[i].codigo);
        printf("Titulo: %s\n", lib_filmes->filmes[i].titulo);
        printf("Genero: %s\n", lib_filmes->filmes[i].genero);
        printf("Preco: %f\n", lib_filmes->filmes[i].preco);
        if((lib_filmes->filmes[i].disponibilidade)){
                printf("\nDisponibilidade: Filme Disponível!\n");
        }else{
                    printf("\nDisponibilidade: Filme Indisponivel...\n");
        }
        printf("----------------------------------------\n");
    }
    printf("\n=============== FIM LIB Filme ===============\n");
}
