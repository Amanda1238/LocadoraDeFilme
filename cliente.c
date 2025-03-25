#include "cliente.h"
#include "aluguel.h"
#include <string.h>
#include <stdio.h>
#include <locale.h> //usa par corrigir erros

#define INTERVALO 10
#define CLIENTE_ARQ "clientes.bin"
#define QTD_CLIENTE_ARQ "clientes_qtd.bin"

///////////////////////////
//                       //
//       Auxiliares      //
//                       //
///////////////////////////
void iniciaBiblotecaCliente(BibliotecaCliente *lib_clientes, int quantidade){
    lib_clientes->quantidade = quantidade;
    lib_clientes->capacidade = quantidade + INTERVALO;
    lib_clientes->clientes = (Cliente *)malloc(sizeof(Cliente) * (quantidade + INTERVALO));
    if (lib_clientes->clientes == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    printf("alocou\n"); // não ultiliza so para verificar se entrou na função
}

void realoca_biblioteca(BibliotecaCliente *lib_clientes){

    // exemplo
    // existe 9 clientes na base e a capacidade é 10
    // se adicionar mais um cliente a capacidade vai ser aumentada para 20
    if (lib_clientes->quantidade + 1 >= lib_clientes->capacidade) {
        lib_clientes->capacidade += INTERVALO;
        lib_clientes->clientes = (Cliente *)realloc(lib_clientes->clientes, lib_clientes->capacidade * sizeof(Cliente));
        if (lib_clientes->clientes == NULL) {
            printf("Erro ao alocar memória!\n");
            return;
        }
        printf("realocou\n"); // não ultiliza so para verificar se entrou na função
    }
}


int get_indice_cliente(BibliotecaCliente *lib_clientes, char cpf[12]){
    for(int i=0; i < lib_clientes->quantidade ; i++){ // percorre os filmes cadastrados
        if(strcmp(lib_clientes->clientes[i].cpf, cpf) == 0){ // compara o codigo do filme atual
            return i; // returna o indice do codigo procurado
            }
    }
    return -1; // Código não encontrado
}
///////////////////////////
//                       //
//       Arquivos        //
//                       //
///////////////////////////
void carrega_clientes_do_arquivo(BibliotecaCliente *lib_clientes){
    // struct do arquivo
    FILE *file_qtd_clientes = fopen(QTD_CLIENTE_ARQ, "rb");

    int qtd_cliente;

    // se o arquivo existir
    // se o arquivo n exister essa variavel é null ai n passa no if
    if (file_qtd_clientes) {

        //fread( qual_variavel, tamanho_da_variavel, quantidade_de_variaveis, arquivo);

        if (fread(&qtd_cliente, sizeof(int), 1, file_qtd_clientes) != 1) {
            printf("Erro: Falha ao ler a quantidade de clientes do arquivo.\n");
            fclose(file_qtd_clientes);
            return;
        }
        fclose(file_qtd_clientes);
    } else{
        qtd_cliente = 0;
    }
    printf("Quantidade de clientes: %d\n", qtd_cliente);

    iniciaBiblotecaCliente(lib_clientes, qtd_cliente);

    FILE *file_clientes = fopen(CLIENTE_ARQ, "rb");
    if (file_clientes) {
        if (fread(lib_clientes->clientes, sizeof(Cliente), qtd_cliente, file_clientes) != (size_t)qtd_cliente) {
            printf("Erro: Falha ao ler os dados do arquivo de clientes.\n");
            free(lib_clientes->clientes);
            fclose(file_clientes);
            return;
        }
        fclose(file_clientes);
    }

}

void salva_clientes_no_arquivo(BibliotecaCliente *lib_clientes){

    FILE *file = fopen(CLIENTE_ARQ, "wb");
    if (file) {
        fwrite(lib_clientes->clientes, sizeof(Cliente), lib_clientes->capacidade, file);
        fclose(file);
    }

    FILE *file_qtd = fopen(QTD_CLIENTE_ARQ, "wb");
    if (file) {
        fwrite(&lib_clientes->quantidade, sizeof(int), 1, file_qtd);
        fclose(file);
    }
}



///////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////

void cadastra_cliente(BibliotecaCliente *lib_clientes){

    if (lib_clientes->clientes == NULL) {
        iniciaBiblotecaCliente(lib_clientes, 0);
    }

    int verificacao, repeticao, ehNome, ehIdade, verificacao_idade, ehAluguel, verifica_aluguel;

    //----------------------------------- verificaçao cpf --------------------------------

    Cliente novo_cliente;
    printf("\n======================= Cadastro Cliente ================================\n");
    do {
        verificacao = 0;

        printf("Digite seu CPF [deve conter 11 números]: ");
        scanf("%s", novo_cliente.cpf);
        while (getchar() != '\n');

        // 1° Verifica se há algum caractere que não seja número
        int apenas_numeros = 1; // Variável auxiliar para validar os caracteres
        for (int i = 0; i < strlen(novo_cliente.cpf); i++) {
            if (!isdigit(novo_cliente.cpf[i])) {
                apenas_numeros = 0;
                break;
            }
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else {
            // 2° Se passou no teste de caracteres, verifica o tamanho
            if (strlen(novo_cliente.cpf) != 11) {
                printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
                verificacao = 1;
            } else {
                // 3° Se passou nas duas verificações anteriores, verifica se o CPF já foi cadastrado
                repeticao = get_indice_cliente(lib_clientes, novo_cliente.cpf);
                if (repeticao != -1) {
                    printf("\nERRO! O CPF já está cadastrado, digite novamente.\n");
                    verificacao = 1;
                }
            }
        }

    } while (verificacao != 0);
//----------------------------------- verificaçao nome --------------------------------

    do {
        ehNome = 0;
        printf("Digite seu nome: ");

        // Melhor alternativa: usar fgets()
        fgets(novo_cliente.nome, sizeof(novo_cliente.nome), stdin);

        // Remover o '\n' no final da string, se existir
        size_t len = strlen(novo_cliente.nome);
        if (len > 0 && novo_cliente.nome[len - 1] == '\n') {
            novo_cliente.nome[len - 1] = '\0';
        }

        // Verificar se contém apenas letras e espaços
        for (int i = 0; i < strlen(novo_cliente.nome); i++) {
            if (!isalpha(novo_cliente.nome[i]) && novo_cliente.nome[i] != ' ') {
                ehNome = 1;
                printf("\nERRO! Apenas letras são permitidas. Tente novamente.");
                break;
            }
        }

    } while (ehNome != 0);

    //----------------------------------- verificaçao idade --------------------------------

    do{
        ehIdade=0 ;
        printf("\nDigite sua idade: ");
        verificacao_idade = scanf("%d", &novo_cliente.idade); // retorna 1 se for um inteiro
        while (getchar() != '\n');
        // verifica se um inteiro
        if (verificacao_idade != 1) {
            printf("\nERRO! A idade precisa ser um número. Digite novamente.");
            ehIdade = 1;
        // so aceita maior de idade
        } else if (novo_cliente.idade < 18) {
            printf("\nERRO! Você precisa ter 18 anos ou mais para se cadastrar.");
            ehIdade = 1;
        }

    }while(ehIdade !=0);

    //----------------------------------- verificaçao numAlugueis --------------------------------
    do{
        ehAluguel = 0;
        printf("Digite numeros de alugueis já feito: ");
        verifica_aluguel = scanf("%d", &novo_cliente.numAlugueis);
        while (getchar() != '\n');

        // verifica se um inteiro
        if (verifica_aluguel != 1) {
            printf("\nERRO! O aluguel precisa ser um número. Digite novamente.\n");
            ehAluguel = 1;
        // tem que ser um numero positivo
        } else if (novo_cliente.numAlugueis < 0) {
            printf("\nERRO! Não é aceitos numeros negativos \n");
            ehAluguel = 1;
        }

    }while(ehAluguel != 0);

    // o realoca_biblioteca é chamado para verificar se a capacidade do vetor é suficiente se for adicionado
    // mais um cliente por isso é chamado antes
    realoca_biblioteca(lib_clientes);

    lib_clientes->clientes[lib_clientes->quantidade] = novo_cliente;
    lib_clientes->quantidade++;

    salva_clientes_no_arquivo(lib_clientes);
    printf("\n=========================================================================\n");
    printf("\nCadastro realizado com sucesso :)");
}

///////////////////////////
//                       //
//       Consulta        //
//                       //
///////////////////////////

void consulta_cliente(BibliotecaCliente *lib_clientes) {

    char cpf[12]; // CPF deve ser uma string, não um int
    int flag = 0, verificacao; // Inicializa flag com 0

    //falta tratamento de erro do id do filme
    do {
        verificacao = 0;

        printf("Digite seu CPF [deve conter 11 números] (ou sair para voltar ao menu): ");
        scanf("%11s", cpf); // Garante que não ultrapassa o tamanho do array
        while (getchar() != '\n'); // Limpa o buffer de entrada

        // 1° Verifica se há algum caractere que não seja número
        int apenas_numeros = 1;
        for (int i = 0; cpf[i] != '\0'; i++) {
            if (!isdigit((unsigned char)cpf[i])) { // Evita comportamento indefinido
                apenas_numeros = 0;
                break;
            }
        }

        if(strcmp(cpf, "sair") == 0){
            printf("\nVoltando ao menu...\n");
            return;
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else if (strlen(cpf) != 11) { // 2° Verifica o tamanho
            printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
            verificacao = 1;
        }

    } while (verificacao != 0);
    // Verifica se o CPF existe
    for (int i = 0; i < lib_clientes->quantidade; i++) {
        if (strcmp(lib_clientes->clientes[i].cpf, cpf) == 0) { // Comparação correta para strings
            flag = 1; // Marca que encontrou
            printf("\n============= Dados do Cliente ===================\n");
            printf("\nCPF do cliente: %s", lib_clientes->clientes[i].cpf);
            printf("\nNome do cliente: %s", lib_clientes->clientes[i].nome);
            printf("\nIdade do cliente: %d", lib_clientes->clientes[i].idade);
            printf("\nNúmero de aluguéis feitos: %d\n", lib_clientes->clientes[i].numAlugueis);
            printf("\nConsulta feita com sucesso!");
            break; // Sai do loop após encontrar
        }
    }

    if (flag == 0) {
        printf("\nCPF não encontrado! \n");
    }
}
///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_cliente(BibliotecaCliente *lib_clientes){
    int  verificacao,verifica_aluguel, verificacao_idade, op, ehNome, indice, ehIdade, ehAluguel;
    char cpf[12];
    //fazer verificacao
    do {
        verificacao = 0;

        printf("Digite seu CPF [deve conter 11 números]: ");
        scanf("%11s", cpf); // Garante que não ultrapassa o tamanho do array
        while (getchar() != '\n'); // Limpa o buffer de entrada

        // 1° Verifica se há algum caractere que não seja número
        int apenas_numeros = 1;
        for (int i = 0; cpf[i] != '\0'; i++) {
            if (!isdigit((unsigned char)cpf[i])) { // Evita comportamento indefinido
                apenas_numeros = 0;
                break;
            }
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else if (strlen(cpf) != 11) { // 2° Verifica o tamanho
            printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
            verificacao = 1;
        }

    } while (verificacao != 0);
    // retorna -1 se não encontrar o CPF// se não ele retorna o indice que o numero está
    indice = get_indice_cliente(lib_clientes, cpf);
    if(indice == -1){
        printf("\nO codigo não existe!");
        return;
    }
    printf("\nEscolha uma opção para alteração: ");
    printf("\n\n================ Menu ===================\n");
    printf("\n1- Nome ");
    printf("\n2- Idade ");
    printf("\n3- Numeros de alugueis já feitos ");
    printf("\n4- Sair");
    printf("\n\n=========================================\n");
    scanf("%d", &op);
    while (getchar() != '\n');

    switch(op){
        case 1:
            do {
                ehNome = 0;
                printf("Digite seu nome: ");

                // Melhor alternativa: usar fgets()
                fgets(lib_clientes->clientes[indice].nome, sizeof(lib_clientes->clientes[indice].nome), stdin);

                // Remover o '\n' no final da string, se existir
                size_t len = strlen(lib_clientes->clientes[indice].nome);
                if (len > 0 && lib_clientes->clientes[indice].nome[len - 1] == '\n') {
                    lib_clientes->clientes[indice].nome[len - 1] = '\0';
                }

                // Verificar se contém apenas letras e espaços
                for (int i = 0; i < strlen(lib_clientes->clientes[indice].nome); i++) {
                    if (!isalpha(lib_clientes->clientes[indice].nome[i]) && lib_clientes->clientes[indice].nome[i] != ' ') {
                        ehNome = 1;
                        printf("\nERRO! Apenas letras são permitidas. Tente novamente.\n");
                        break;
                    }
                }

            } while (ehNome != 0);
            break;
        case 2:
            do{
                ehIdade=0 ;
                printf("Digite sua idade: ");
                verificacao_idade = scanf("%d", &lib_clientes->clientes[indice].idade); // retorna 1 se for um inteiro
                while (getchar() != '\n');
                // verifica se um inteiro
                if (verificacao_idade != 1) {
                    printf("\nERRO! A idade precisa ser um número. Digite novamente.\n");
                    ehIdade = 1;
                // so aceita maior de idade
                } else if (lib_clientes->clientes[indice].idade < 18) {
                    printf("\nERRO! Você precisa ter 18 anos ou mais para se cadastrar.\n");
                    ehIdade = 1;
                }

            }while(ehIdade !=0);

            break;
        case 3:
            do {
            ehAluguel = 0;
            printf("\nDigite numeros de alugueis já feito: ");
            verifica_aluguel = scanf("%d", &lib_clientes->clientes[indice].numAlugueis);
            while (getchar() != '\n');

            // verifica se um inteiro
            if (verifica_aluguel != 1) {
                printf("\nERRO! O aluguel precisa ser um número. Digite novamente.\n");
                ehAluguel = 1;
            // tem que ser um numero positivo
            } else if (lib_clientes->clientes[indice].numAlugueis < 0) {
                printf("\nERRO! Não é aceitos numeros negativos \n");
                ehAluguel = 1;
            } else {
                // Verifica se há algum caractere que não seja número
                char buffer[50];
                snprintf(buffer, sizeof(buffer), "%d", lib_clientes->clientes[indice].numAlugueis);
                for (int i = 0; buffer[i] != '\0'; i++) {
                if (!isdigit((unsigned char)buffer[i])) {
                    printf("\nERRO! O aluguel precisa conter apenas números. Digite novamente.\n");
                    ehAluguel = 1;
                    break;
                }
                }
            }

            } while (ehAluguel != 0);
            break;
        case 4:
            printf("\nSaindo...");
            break;
        default:
            printf("\nOpção invalida!");
            break;

    }
    //salva se caso alterar
    salva_clientes_no_arquivo(lib_clientes);

}
///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////

void deleta_cliente(BibliotecaCliente *lib_clientes) {
    int indice, verificacao;
    char cpf[12];

    do {
        verificacao = 0;

        printf("\nDigite o CPF do cliente (ou 'sair' para voltar ao menu): ");
        scanf("%11s", cpf);
        while (getchar() != '\n');

        if (strcmp(cpf, "sair") == 0) {
            printf("\nVoltando ao menu...\n");
            return;
        }

        // Verifica se há algum caractere que não seja número
        int apenas_numeros = 1;
        for (int i = 0; cpf[i] != '\0'; i++) {
            if (!isdigit((unsigned char)cpf[i])) {
                apenas_numeros = 0;
                break;
            }
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else if (strlen(cpf) != 11) {
            printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
            verificacao = 1;
        }

    } while (verificacao != 0);

    // Retorna -1 se o código não existir e retorna o índice caso exista
    indice = get_indice_cliente(lib_clientes, cpf);

    if (indice == -1) {
        printf("\nCódigo de cliente não existe.\n");
        return;
    }

    for (int i = indice; i < lib_clientes->quantidade - 1; i++) {
        lib_clientes->clientes[i] = lib_clientes->clientes[i + 1];
    }

    lib_clientes->quantidade--; // Diminui a contagem de clientes
    printf("\nCliente deletado com sucesso!\n");
    salva_clientes_no_arquivo(lib_clientes);
}

///////////////////////////
//                       //
//     Mostra todos      //
//                       //
///////////////////////////
// consulta dos os Clientes
void print_biblioteca(BibliotecaCliente *lib_clientes){
    printf("\n=============== INICIO LIB Cliente ===============\n");
    for(int i=0; i < lib_clientes->quantidade ; i++){
        printf("CPF: %s\n", lib_clientes->clientes[i].cpf);
        printf("Nome: %s\n", lib_clientes->clientes[i].nome);
        printf("Idade: %d\n", lib_clientes->clientes[i].idade);
        printf("Numero de alugueis: %d\n", lib_clientes->clientes[i].numAlugueis);
        printf("----------------------------------------\n");
    }
    printf("\n=============== FIM LIB CLIENTE ===============\n");
}
