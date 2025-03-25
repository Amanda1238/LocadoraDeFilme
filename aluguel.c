#include "aluguel.h"
#include <string.h>
#include <stdio.h>
#include <locale.h> //usa par corrigir erros
#include "cliente.h"
#include "filme.h"

#define INTERVALO 10
#define ALUGUEL_ARQ "aluguel.bin"
#define QTD_ALUGUEL_ARQ "aluguel_qtd.bin"


void libera_biblioteca(BibliotecaAluguel *lib_alugueis){
    // essa linha quando foir fazer um deleta alguel, usar ela
    free(lib_alugueis->alugueis);
}

void print_biblioteca_aluguel(BibliotecaAluguel *lib_alugueis){
    printf("\n=============== INICIO LIB Aluguel ===============\n");
    for(int i=0; i < lib_alugueis->quantidade ; i++){
        printf("ID Aluguel: %d\n", lib_alugueis->alugueis[i].id_aluguel);
        printf("CPF Cliente: %s\n", lib_alugueis->alugueis[i].cpf_cliente);
        printf("ID Filme: %d\n", lib_alugueis->alugueis[i].id_filme);
        printf("Data Devolução: %d/%d/%d\n",
            lib_alugueis->alugueis[i].dataDevolucao.dia,
            lib_alugueis->alugueis[i].dataDevolucao.mes,
            lib_alugueis->alugueis[i].dataDevolucao.ano);
        printf("----------------------------------------\n");
    }
    printf("\n=============== FIM LIB Aluguel ===============\n");
}

int verifica_data(Data *data) {
    // Verifica se o dia, mês e ano estão dentro dos limites especificados
    if (data->dia < 1 || data->dia > 30) {
        printf("\nERRO! O dia deve estar entre 1 e 30.\n");
        return 0;
    }
    if (data->mes < 1 || data->mes > 12) {
        printf("\nERRO! O mês deve estar entre 1 e 12.\n");
        return 0;
    }
    if (data->ano < 1970 || data->ano > 2025) {
        printf("\nERRO! O ano deve estar entre 1970 e 2025.\n");
        return 0;
    }
    return 1; // Se passar em todas as verificações, retorna 1 (data válida)
}

///////////////////////////
//                       //
//       Auxiliares      //
//                       //
///////////////////////////
void iniciaBiblotecaAluguel (BibliotecaAluguel *lib_alugueis, int quantidade){
    lib_alugueis->quantidade = quantidade;
    lib_alugueis->capacidade = quantidade + INTERVALO;
    lib_alugueis->alugueis = (Aluguel *)malloc(sizeof(Aluguel) * (quantidade + INTERVALO));
    if (lib_alugueis->alugueis == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    printf("alocou\n"); // não ultiliza so para verificar se entrou na função
}

void realoca_biblioteca_aluguel (BibliotecaAluguel *lib_alugueis){

    // exemplo
    // existe 9 clientes na base e a capacidade é 10
    // se adicionar mais um cliente a capacidade vai ser aumentada para 20
    if (lib_alugueis->quantidade + 1 >= lib_alugueis->capacidade) {
        lib_alugueis->capacidade += INTERVALO;
        lib_alugueis->alugueis = (Aluguel *)realloc(lib_alugueis->alugueis, lib_alugueis->capacidade * sizeof(Aluguel));
        if (lib_alugueis->alugueis == NULL) {
            printf("Erro ao alocar memória!\n");
            return;
        }
        printf("realocou\n"); // não ultiliza so para verificar se entrou na função
    }
}
int get_indice_aluguel (BibliotecaAluguel *lib_alugueis, int codigo){
    for(int i=0; i < lib_alugueis->quantidade ; i++){ // percorre os filmes cadastrados
        if(lib_alugueis->alugueis[i].id_aluguel == codigo){ // compara o codigo do filme atual
            return i; // returna o indice do codigo procurado
            }
    }
    return -1; // C�digo n�o encontrado
}
///////////////////////////
//                       //
//       Arquivos        //
//                       //
///////////////////////////
void carrega_alugueis_do_arquivo(BibliotecaAluguel *lib_alugueis){
    // struct do arquivo
    FILE *file_qtd_alugueis = fopen(QTD_ALUGUEL_ARQ, "rb");

    int qtd_alguel;

    // se o arquivo existir
    // se o arquivo n exister essa variavel é null ai n passa no if
    if (file_qtd_alugueis) {

        //fread( qual_variavel, tamanho_da_variavel, quantidade_de_variaveis, arquivo);

        if (fread(&qtd_alguel, sizeof(int), 1, file_qtd_alugueis) != 1) {
            printf("Erro: Falha ao ler a quantidade de clientes do arquivo.\n");
            fclose(file_qtd_alugueis);
            return;
        }
        fclose(file_qtd_alugueis);
    } else{
        qtd_alguel = 0;
    }
    printf("Quantidade de Alugueis: %d\n", qtd_alguel);

    iniciaBiblotecaAluguel(lib_alugueis, qtd_alguel);

    FILE *file_alugueis = fopen(ALUGUEL_ARQ, "rb");
    if (file_alugueis) {
        if (fread(lib_alugueis->alugueis, sizeof(Aluguel), qtd_alguel, file_alugueis) != (size_t)qtd_alguel) {
            printf("Erro: Falha ao ler os dados do arquivo de clientes.\n");
            free(lib_alugueis->alugueis);
            fclose(file_alugueis);
            return;
        }
        fclose(file_alugueis);
    }

}

void salva_alugueis_no_arquivo(BibliotecaAluguel *lib_alugueis){

    FILE *file = fopen(ALUGUEL_ARQ, "wb");
    if (file) {
        fwrite(lib_alugueis->alugueis, sizeof(Aluguel), lib_alugueis->capacidade, file);
        fclose(file);
    }

    FILE *file_qtd = fopen(QTD_ALUGUEL_ARQ, "wb");
    if (file) {
        fwrite(&lib_alugueis->quantidade, sizeof(int), 1, file_qtd);
        fclose(file);
    }
}


///////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////

void cadastra_aluguel(BibliotecaAluguel *lib_alugueis, BibliotecaFilme *lib_filmes, BibliotecaCliente *lib_clientes){
    int valido, ehValido, ehInt, verifica, verificacao, repeticao, id_filme;
    char entrada1[20], entrada[20];
    // inicializa o vetor de struct se não tiver alocado
    if (lib_alugueis->alugueis == NULL) {
        iniciaBiblotecaAluguel(lib_alugueis, 0);
    }

    if (lib_clientes->quantidade == 0) {
        printf("\nErro! Você não pode fazer cadastro porque não há clientes.\n");
        return; // Sai do loop
    }

    if (lib_filmes->quantidade == 0) {
        printf("\nErro! Você não pode fazer cadastro porque não há Filmes.\n");
        return; // Sai do loop
    }



    Aluguel novo_aluguel;
    Data dataDevolucao;
    //-------------------------------------- Verificação do ID do aluguel --------------------------
    do {
        ehValido = 1;

        printf("\nDigite o ID do aluguel: ");
        scanf("%s", entrada1);  // Lê como string para verificar cada caractere
        while (getchar() != '\n'); // Limpa o buffer

        // Verifica se a entrada contém apenas números
        for (int i = 0; i < strlen(entrada1); i++) {
            if (!isdigit(entrada1[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O ID do aluguel deve conter apenas números positivos! Tente novamente.\n");
        } else {
            ehInt = atoi(entrada1); // Converte para inteiro

            // Verifica se o ID do aluguel já existe
            verificacao = get_indice_aluguel(lib_alugueis, ehInt); // Retorna -1 se não encontrado
            if (verificacao != -1) {
                printf("\nErro: O ID do aluguel já existe! Escolha um diferente.\n");
                ehValido = 0;
            } else {
                novo_aluguel.id_aluguel = ehInt;
            }
        }

    } while (!ehValido);
    //-------------------------------------- Verificação do cpf_cliente --------------------------
    do {
        verificacao = 0;

        printf("Digite o Cpf do cliente [deve conter 11 números]: ");
        scanf("%s", novo_aluguel.cpf_cliente);
        while (getchar() != '\n');

        // 1° Verifica se há algum caractere que não seja número
        int apenas_numeros = 1; // Variável auxiliar para validar os caracteres
        for (int i = 0; i < strlen(novo_aluguel.cpf_cliente); i++) {
            if (!isdigit(novo_aluguel.cpf_cliente[i])) {
                apenas_numeros = 0;
                break;
            }
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else {
            // 2° Se passou no teste de caracteres, verifica o tamanho
            if (strlen(novo_aluguel.cpf_cliente) != 11) {
                printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
                verificacao = 1;
            } else {
                // 3° Se passou nas duas verificações anteriores, verifica se o CPF já foi cadastrado
                repeticao = get_indice_cliente(lib_clientes, novo_aluguel.cpf_cliente);
                if (repeticao == -1) {
                    printf("\nERRO! O CPF não está cadastrado. Tente outro\n");
                    verificacao = 1;
                }
            }
        }

    } while (verificacao != 0);
    //-------------------------------------- Verificação do id_filme --------------------------
    do {
        ehValido = 1;

        printf("\nDigite o código do filme: ");
        scanf("%s", entrada);  // Lê como string para verificar cada caractere


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
            ehInt = atoi(entrada); // Converte para inteiro
            id_filme = get_indice_filme(lib_filmes, ehInt); //retorna -1 se não econtrado

            if(id_filme == -1){
                printf("\nERRO! O Codigo do filme não está cadastrado, tente outro\n");
                ehValido = 0;
            }else{
                if(!lib_filmes->filmes[id_filme].disponibilidade){
                    printf("\nERRO! Esse filme está indisponível, digite um que está disponível\n");
                    ehValido = 0;
                }else{
                    novo_aluguel.id_filme = ehInt;
                }
            }
        }

    } while (!ehValido);
    //-------------------------------------- Verificação do dataDevolução --------------------------

    do {
        ehValido = 0;
        printf("\nEscreva a data de devolução (DD MM AAAA): ");
        scanf("%d %d %d",
                &dataDevolucao.dia,
                &dataDevolucao.mes,
                &dataDevolucao.ano);

        if (!verifica_data(&dataDevolucao)) { // Corrigido o ponteiro
            printf("\nERRO! Data inválida. Tente novamente.\n");
            ehValido = 0;
        } else {
            novo_aluguel.dataDevolucao.dia = dataDevolucao.dia;
            novo_aluguel.dataDevolucao.mes = dataDevolucao.mes;
            novo_aluguel.dataDevolucao.ano = dataDevolucao.ano;
             // Corrigido o ponteiro
            ehValido = 1;
            printf("Data Devolução: %d/%d/%d\n",
                novo_aluguel.dataDevolucao.dia,
                novo_aluguel.dataDevolucao.mes,
                novo_aluguel.dataDevolucao.ano);
            printf("Deu bom a data\n");
        }

    } while (!ehValido); // Corrigido o ponteiro

    // Realoca espaço caso necessário
    realoca_biblioteca_aluguel(lib_alugueis);

    // Adiciona o aluguel à biblioteca **corretamente**
    lib_alugueis->alugueis[lib_alugueis->quantidade] = novo_aluguel; // Copia os valores, não o ponteiro!
    lib_alugueis->quantidade++; // Incrementa a contagem de aluguéis

    printf("\nFilme alugado com sucesso!\n");

    // Salva no arquivo
    salva_alugueis_no_arquivo(lib_alugueis);


}

///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////
void deleta_aluguel(BibliotecaAluguel *lib_alugueis){
    int id_aluguel, indice, ehValido;
    char entrada[20];
    do {
        ehValido = 1;
        printf("\nDigite o código do aluguel (ou 'sair' para voltar ao menu): ");
        scanf("%s", entrada);
        while (getchar() != '\n');

        if (strcmp(entrada, "sair") == 0) {
            printf("\nVoltando ao menu...\n");
            return;
        }

        // Verifica se a entrada contém apenas números
        for (int i = 0; i < strlen(entrada); i++) {
            if (!isdigit(entrada[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O código do aluguel deve conter apenas números! Tente novamente.\n");
        } else {
            id_aluguel = atoi(entrada); // Converte para inteiro
            // Retorna -1 se o ID não existir e retorna o índice caso exista
            indice = get_indice_aluguel(lib_alugueis, id_aluguel);
            if(indice == -1){
                printf("\nCódigo de aluguel não existe\n");
                ehValido = 0;
            }
        }
    } while (!ehValido);

    for (int i = indice; i < lib_alugueis->quantidade - 1; i++) {
        lib_alugueis->alugueis[i] = lib_alugueis->alugueis[i + 1];
    }

    lib_alugueis->quantidade--; // Diminui a contagem de aluguéis
    printf("\nAluguel deletado com sucesso!\n");
    salva_alugueis_no_arquivo(lib_alugueis);
}


void deleta_filme(BibliotecaFilme *lib_filmes, BibliotecaAluguel *lib_alugueis){
    int ehValido, ehInt, codigo, indice;
    char entrada1[10];

    do {
        ehValido = 1;

        printf("\nDigite o código do filme para deletar (ou sair para voltar ao menu): ");
        scanf("%s", entrada1);  // Lê como string para verificar cada caractere
        while (getchar() != '\n'); // Limpa o buffer

        if(strcmp(entrada1, "sair") == 0){
            printf("\nVoltando ao menu...\n");
            return;
        }

        // Verifica se a entrada contém apenas dígitos
        for (int i = 0; i < strlen(entrada1); i++) {
            if (!isdigit(entrada1[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O código deve conter apenas números! Tente novamente.\n");
        } else {
            ehInt = atoi(entrada1); // Converte para inteiro
            indice = get_indice_filme(lib_filmes, ehInt); // Retorna -1 se não encontrado

            if (indice == -1) {
                printf("\nErro: Código de filme não encontrado!\n");
                return;
            } else {
                codigo = ehInt;
            }
        }

    } while (!ehValido);

    // Verifica se o filme está em algum aluguel
    for (int i = 0; i < lib_alugueis->quantidade; i++) {
        if (lib_alugueis->alugueis[i].id_filme == codigo) {
            printf("\nErro: O filme está alugado e não pode ser deletado!\n");
            return;
        }
    }

    // Deletando o filme
    for (int i = indice; i < lib_filmes->quantidade - 1; i++) {
        lib_filmes->filmes[i] = lib_filmes->filmes[i + 1];
    }

    lib_filmes->quantidade--; // Diminui a contagem de filmes
    printf("\nFilme deletado com sucesso!\n");

    salva_filmes_no_arquivo(lib_filmes); // Salva as informações no arquivo
}


///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_aluguel(BibliotecaAluguel *lib_alugueis){
    int ver_aluguel, indice, ehValido;
    char entrada[10];

    do {
        ehValido = 1;
        printf("\nDigite o codigo do Aluguel: ");
        scanf("%d", &ver_aluguel);
        // verifica se o aluguel ja existe e -1 se não existir e o indice se existir
        indice = get_indice_aluguel(lib_alugueis, ver_aluguel);

        if(indice == -1){
            printf("\nO codigo não existe!");
            ehValido = 0;
        }
    } while (!ehValido); // Corrigido o ponteiro

    printf("\nEscolha uma opção para alteração: ");
    printf("\n\n================ Menu ===================\n");
    printf("\n1- Data de Devolução ");
    printf("\n2- Sair");
    printf("\n\n=========================================\n");

    int op;
    scanf("%d", &op);
    switch(op){
        case 1:
            do {
                ehValido = 1;
                printf("\nDigite a nova data de devolução (dd mm aaaa): ");
                Data dataDevolucao;

                scanf("%d %d %d",
                    &dataDevolucao.dia,
                    &dataDevolucao.mes,
                    &dataDevolucao.ano);

                if (!verifica_data(&dataDevolucao)) {
                    ehValido = 0;
                    printf("\nErro: Data inválida! Tente novamente.\n");
                } else {
                    printf("\nData de devolução alterada com sucesso!\n");
                    lib_alugueis->alugueis[indice].dataDevolucao = dataDevolucao;
                }

            } while (!ehValido);
            break;
        case 2:
            printf("\nSaindo...");
            break;
        default:
            printf("\nOpção invalida!");
            break;
    }
    salva_alugueis_no_arquivo(lib_alugueis);

}
///////////////////////////
//                       //
//     Consulta CPF      //
//                       //
///////////////////////////
void consultar_cpf_aluguel (BibliotecaAluguel *lib_alugueis){
    char cpf[12];
    printf("Digite o CPF que deseja consultar: ");
    scanf("%s", cpf);
    printf("\n========================== Menu por CPF =========================\n");
    for(int i=0; i<lib_alugueis->quantidade; i++){
        if(strcmp(cpf, lib_alugueis->alugueis[i].cpf_cliente)==0){
            printf("\nID do Aluguel: %d", lib_alugueis->alugueis[i].id_aluguel);
            printf("\nCPF do Cliente: %s", lib_alugueis->alugueis[i].cpf_cliente);
            printf("\nCodigo do Filme: %d", lib_alugueis->alugueis[i].id_filme);
            printf("\nData de Devolução: %02d/%02d/%d\n",
                    lib_alugueis->alugueis[i].dataDevolucao.dia,
                    lib_alugueis->alugueis[i].dataDevolucao.mes,
                    lib_alugueis->alugueis[i].dataDevolucao.ano);
            printf("\n-----------------------------------------------------------------\n");
        }
    }
    printf("\n=================================================================\n");

}
///////////////////////////
//                       //
//   Consulta id_filme   //
//                       //
///////////////////////////
void consultar_id_filme (BibliotecaAluguel *lib_alugueis){
    int id_filme;
    printf("Digite o id_filme: ");
    scanf("%d", &id_filme);

    printf("\n========================== Menu por Id_Filme =========================\n");
    for(int i=0; i<lib_alugueis->quantidade; i++){
        if(id_filme == lib_alugueis->alugueis[i].id_filme){
            printf("\nID do Aluguel: %d", lib_alugueis->alugueis[i].id_aluguel);
            printf("\nCPF do Cliente: %s", lib_alugueis->alugueis[i].cpf_cliente);
            printf("\nCodigo do Filme: %d", lib_alugueis->alugueis[i].id_filme);
            printf("\nData de Devolução: %02d/%02d/%d\n",
                    lib_alugueis->alugueis[i].dataDevolucao.dia,
                    lib_alugueis->alugueis[i].dataDevolucao.mes,
                    lib_alugueis->alugueis[i].dataDevolucao.ano);
            printf("\n-----------------------------------------------------------------\n");

        }
    }
    printf("\n======================================================================\n");




}






///////////////////////////
//                       //
//    Gerar relatorio    //
//                       //
///////////////////////////
void gerar_relatorio(BibliotecaAluguel *lib_alugueis, BibliotecaCliente *lib_clientes, BibliotecaFilme *lib_filmes) {
    char cpfBusca[12];
    printf("Digite o CPF do cliente: ");
    scanf("%11s", cpfBusca);

    // abre o arquivo
    FILE *arquivo = fopen("relatorio_aluguel.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //escreve no relatorio
    fprintf(arquivo, "Relatório de Aluguéis para o CPF: %s\n", cpfBusca);
    fprintf(arquivo, "========================================\n");

    int encontrou = 0;

    // percorre os alugueis
    for (int i = 0; i < lib_alugueis->quantidade; i++) {
        // o strcomp retorna 0 se forem iguais
        //retorna um valor negativo → se a primeira string for menor (em ordem ASCII).
        //retorna um positivo → se a primeira string for maior (em ordem ASCII).
        if (strcmp(lib_alugueis->alugueis[i].cpf_cliente, cpfBusca) == 0) {
            encontrou = 1;

            // Buscar informações do cliente
            Cliente *cliente = NULL;
            //percorre os clientes
            for (int j = 0; j < lib_clientes->quantidade; j++) {
                // verifica onde esta o cpf na biblioteca clientes
                if (strcmp(lib_clientes->clientes[j].cpf, cpfBusca) == 0) {
                    //adiciona o endereço dessa estrutura
                    cliente = &lib_clientes->clientes[j];
                    break; // para o looping do for
                }
            }

            // Buscar informações do filme
            Filme *filme = NULL;
            // percorre todos os filmes
            for (int k = 0; k < lib_filmes->quantidade; k++) {
                // verifica de o codigo do filme é igual o id_filme do aluguel
                if (lib_filmes->filmes[k].codigo == lib_alugueis->alugueis[i].id_filme) {
                    // adiciona na struct filme que guarda apenas 1 registro
                    filme = &lib_filmes->filmes[k];
                    break; // ao encontrar para o looping
                }
            }

            // escreve o arquivo
            fprintf(arquivo, "ID do Aluguel: %d\n", lib_alugueis->alugueis[i].id_aluguel);
            fprintf(arquivo, "Data de Devolução: %02d/%02d/%d\n",
                    lib_alugueis->alugueis[i].dataDevolucao.dia,
                    lib_alugueis->alugueis[i].dataDevolucao.mes,
                    lib_alugueis->alugueis[i].dataDevolucao.ano);

            if (cliente != NULL) {
                fprintf(arquivo, "Cliente: %s\n", cliente->nome);
                fprintf(arquivo, "Idade: %d\n", cliente->idade);
                fprintf(arquivo, "Número de Aluguéis: %d\n", cliente->numAlugueis);
            } else {
                fprintf(arquivo, "Cliente não encontrado.\n");
            }

            if (filme != NULL) {
                fprintf(arquivo, "Filme: %s\n", filme->titulo);
                fprintf(arquivo, "Gênero: %s\n", filme->genero);
                fprintf(arquivo, "Preço: R$ %.2f\n", filme->preco);
            } else {
                fprintf(arquivo, "Filme não encontrado.\n");
            }

            fprintf(arquivo, "----------------------------------------\n");
        }// termina o if
    } // termino do primeiro for

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhum aluguel encontrado para o CPF informado.\n");
    } else {
        printf("Relatório gerado com sucesso! Confira o arquivo 'relatorio_aluguel.txt'.\n");
    }
}
