#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h> //usa par corrigir erros

#define INTERVALO 10
#define FILME_ARQ "filmes.bin"
#define CLIENTE_ARQ "clientes.bin"
#define ALUGUEL_ARQ "alugueis.bin"
#define QUANTIDADE_ARQ "quantidades.bin"

// backup macros
#define FILME_ARQ_BACKUP "backup/filmes.bin"
#define CLIENTE_ARQ_BACKUP "backup/clientes.bin"
#define ALUGUEL_ARQ_BACKUP "backup/alugueis.bin"
#define QUANTIDADE_ARQ_BACKUP "backup/quantidades.bin"


///////////////////////////
//                       //
//    Registro Filme     //
//                       //
///////////////////////////
typedef struct{
	int codigo;
	char titulo[100];
	char genero[100];
	float preco;
	bool disponibilidade;
} Filme;
///////////////////////////
//                       //
//    Registro Aluguel   //
//                       //
///////////////////////////
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
///////////////////////////
//                       //
//  Arquivos auxiliares  //
//                       //
///////////////////////////
// inicializa as quantidade
void ler_quantidades (int *qtd_filme, int *qtd_cliente, int *qtd_aluguel);

Filme *ler_filme(int qtd_filme);
Cliente *ler_cliente(int qtd_cliente);
Aluguel *ler_aluguel(int qtd_aluguel);


//////////////////////
//                  //
//     ESCRITA      //
//                  //
//////////////////////
void escrever_filme(Filme *filmes, int qtd_filme, char *modo);
void escrever_cliente(Cliente *clientes, int qtd_clientes, char *modo);
void escrever_aluguel(Aluguel *alugueis, int qtd_aluguel, char *modo);

///////////////////////////
//                       //
//  Arquivos auxiliares  //
//       Clientes        //
///////////////////////////

int indice_atualiza_cliente (Cliente **usuario, int *quant_cliente, char *cpf);
int verifica_cpf_cliente(Cliente **usuario, int *quant_cliente, char *cpf);
int aloca_cliente(Cliente **usuario, int *quant_cliente, int *alocacao);
void realoca_cliente(Cliente **usuario, int *quant_cliente, int *alocacao);

///////////////////////////
//                       //
//  Arquivos auxiliares  //
//       Filme           //
///////////////////////////

int indice_atualiza(Filme **catalogo, int *quant_filme, int codigo);
// verifica se o codigo é repetido ou não
int verifica_codigo_filme(Filme **catalogo, int *quant_filme, int codigo);
// inicializa o vetor
int iniciaBiblioteca(Filme **catalogo, int *quant_filme, int *alocacao);
// realoca memoria
void realocaMemoriaBiblioteca(Filme **catalogo, int *quant_filme, int *alocacao);

//////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////
//cliente
void cadastra_cliente(Cliente **usuario , int *quant_cliente, int *alocacao);
void cadastra_filme(Filme **catalogo , int *quant_filme, int *alocacao);

///////////////////////////
//                       //
//       Consulta        //
//                       //
///////////////////////////
void consulta_cliente(Cliente **usuario, int *quant_cliente);
void consulta_filme(Filme **catalogo, int *quant_filme );

///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_cliente(Cliente **usuario, int *quant_cliente, int *alocacao);
void atualiza_filme(Filme **catalogo, int *quant_filme, int *alocacao);

///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////

void deleta_cliente(Cliente **usuario, int *quant_cliente, int *alocacao);
void deleta_filme(Filme **catalogo, int *quant_filme, int *alocacao);


//////////////////////
//                  //
//       LER        //
//                  //
//////////////////////
Filme *ler_filme(int qtd_filme) {
    FILE *file = fopen(FILME_ARQ, "rb");
    Filme *filmes = (Filme *)malloc(sizeof(Filme) * (INTERVALO + qtd_filme));

    if (!filmes) {
        printf("Erro: Falha ao alocar memória para filmes.\n");
        return NULL;
    }

    if (file) {
        if (fread(filmes, sizeof(Filme), qtd_filme, file) != (size_t)qtd_filme) {
            printf("Erro: Falha ao ler os dados do arquivo de filmes.\n");
            free(filmes);
            fclose(file);
            return NULL;
        }
        fclose(file);
    } else {
        FILE *backup_file = fopen(FILME_ARQ_BACKUP, "rb");

        if (backup_file) {
            if (fread(filmes, sizeof(Filme), qtd_filme, backup_file) != (size_t)qtd_filme) {
                printf("Erro: Falha ao ler os dados do backup de filmes.\n");
                free(filmes);
                fclose(backup_file);
                return NULL;
            }
            fclose(backup_file);

            // Restaurando backup
            FILE *file_restore = fopen(FILME_ARQ, "wb");
            if (file_restore) {
                if (fwrite(filmes, sizeof(Filme), qtd_filme, file_restore) != (size_t)qtd_filme) {
                    printf("Erro: Falha ao restaurar o arquivo de filmes.\n");
                    fclose(file_restore);
                    free(filmes);
                    return NULL;
                }
                fclose(file_restore);
            } else {
                printf("Erro: Não foi possível recriar o arquivo de filmes a partir do backup.\n");
            }
        } else {
            //printf("Erro: Nenhum arquivo de filmes disponível.\n");
            free(filmes);
            return NULL;
        }
    }

    return filmes;
}

Aluguel *ler_aluguel(int qtd_aluguel) {
    FILE *file = fopen(ALUGUEL_ARQ, "rb");
    Aluguel *alugueis = (Aluguel *)malloc(sizeof(Aluguel) * (INTERVALO + qtd_aluguel));

    if (!alugueis) {
        printf("Erro: Falha ao alocar memória para aluguéis.\n");
        return NULL;
    }

    if (file) {
        if (fread(alugueis, sizeof(Aluguel), qtd_aluguel, file) != (size_t)qtd_aluguel) {
            printf("Erro: Falha ao ler os dados do arquivo de aluguéis.\n");
            free(alugueis);
            fclose(file);
            return NULL;
        }
        fclose(file);
    } else {
        FILE *backup_file = fopen(ALUGUEL_ARQ_BACKUP, "rb");

        if (backup_file) {
            if (fread(alugueis, sizeof(Aluguel), qtd_aluguel, backup_file) != (size_t)qtd_aluguel) {
                printf("Erro: Falha ao ler os dados do backup de aluguéis.\n");
                free(alugueis);
                fclose(backup_file);
                return NULL;
            }
            fclose(backup_file);

            // Tentando restaurar o backup no arquivo original
            FILE *file_restore = fopen(ALUGUEL_ARQ, "wb");
            if (file_restore) {
                if (fwrite(alugueis, sizeof(Aluguel), qtd_aluguel, file_restore) != (size_t)qtd_aluguel) {
                    printf("Erro: Falha ao restaurar o arquivo de aluguéis.\n");
                    fclose(file_restore);
                    free(alugueis);
                    return NULL;
                }
                fclose(file_restore);
            } else {
                printf("Erro: Não foi possível recriar o arquivo de aluguéis a partir do backup.\n");
            }
        } else {
            //printf("Erro: Nenhum arquivo de aluguéis disponível.\n");
            free(alugueis);
            return NULL;
        }
    }

    return alugueis;
}
/////////////////////////
//                     //
//   TAMANHO DO VETOR  //
//                     //
/////////////////////////

void ler_quantidades (int *qtd_filme, int *qtd_cliente, int *qtd_aluguel) {
  FILE *file;

  file = fopen(QUANTIDADE_ARQ, "rb"); //#define QUANTIDADE_ARQ "quantidades.bin"

  // valor padrao
  *qtd_filme = 0;
  *qtd_cliente = 0;
  *qtd_aluguel = 0;

  if (file) {
    fread(qtd_filme, sizeof(int), 1, file);
    fread(qtd_cliente, sizeof(int), 1, file);
    fread(qtd_aluguel, sizeof(int), 1, file);
    fclose(file);
  } else {
    FILE *backup_file = fopen(QUANTIDADE_ARQ_BACKUP, "rb"); //#define QUANTIDADE_ARQ_BACKUP "backup/quantidades.bin"


    if (backup_file) {
      fread(qtd_filme, sizeof(int), 1, backup_file);
      fread(qtd_cliente, sizeof(int), 1, backup_file);
      fread(qtd_aluguel, sizeof(int), 1, backup_file);

      FILE *file = fopen(QUANTIDADE_ARQ, "wb");

      if (file) {
          fwrite(qtd_filme, sizeof(int), 1, file);
          fwrite(qtd_cliente, sizeof(int), 1, file);
          fwrite(qtd_aluguel, sizeof(int), 1, file);
          fclose(file);
      }

      fclose(backup_file);
    } else {
      // caso nao exista um arquivo e nem um backup, entao criaremos ele com os valores em padrao, ou, inicial, que seria 0
      FILE *file_ = fopen(QUANTIDADE_ARQ, "wb");
      if (file_) {
        fwrite(qtd_filme, sizeof(int), 1, file_);
        fwrite(qtd_cliente, sizeof(int), 1, file_);
        fwrite(qtd_aluguel, sizeof(int), 1, file_);
        fclose(file_);
      }
    }
  }

}
//////////////////////
//                  //
//     ESCRITA      //
//                  //
//////////////////////
void escrever_filme (Filme *filmes, int qtd_filme, char *modo) {
  FILE *file = fopen(FILME_ARQ, modo);

  if (file) {
    fwrite(filmes, sizeof(Filme), qtd_filme, file);
    fclose(file);
  }
}

void escrever_cliente (Cliente *clientes, int qtd_cliente, char *modo) {
  FILE *file = fopen(CLIENTE_ARQ, modo);

  if (file) {
    fwrite(clientes, sizeof(Cliente), qtd_cliente, file);
    fclose(file);
  }
}

void escrever_aluguel (Aluguel *alugueis, int qtd_aluguel, char *modo) {
  FILE *file = fopen(ALUGUEL_ARQ, modo);

  if (file) {
    fwrite(alugueis, sizeof(Aluguel), qtd_aluguel, file);
    fclose(file);
  }
}

///////////////////////////
//                       //
//       Cliente         //
//                       //
///////////////////////////

/*// Função para receber dados de clientes.bin
void inicia_cliente(Cliente **usuario, int *quant_cliente, int *alocacao) {
    FILE *fp = fopen("cliente.bin", "rb");

    if (fp == NULL) {
        printf("Arquivo não encontrado. Começando um novo banco de usuários.\n");
        *quant_cliente = 0;
        *alocacao = 10;  // Início com espaço para 10 clientes
        *usuario = (Cliente *)malloc((*alocacao) * sizeof(Cliente));
        if (*usuario == NULL) {
            printf("Erro ao alocar memória!\n");
            return;
        }
        return;
    }

    // Lê a quantidade de clientes armazenados
    fread(quant_cliente, sizeof(int), 1, fp);

    // Aloca memória suficiente para os clientes lidos
    *alocacao = *quant_cliente + 10;  // Reserva um pouco mais de espaço
    *usuario = (Cliente *)malloc((*alocacao) * sizeof(Cliente));
    if (*usuario == NULL) {
        printf("Erro ao alocar memória!\n");
        fclose(fp);
        exit(1);
    }

    // Lê os clientes do arquivo para o vetor
    fread(*usuario, sizeof(Cliente), *quant_cliente, fp);
    fclose(fp);
}
void salvaArquivoBibliotecaCliente(Cliente **usuario, int *quant_cliente, int *alocacao) {
    FILE *fp = fopen("cliente.bin", "wb");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Escreve a quantidade de clientes no início do arquivo
    fwrite(&quant_cliente, sizeof(int), 1, fp);

    // Escreve todos os clientes de uma vez
    fwrite(usuario, sizeof(Cliente), quant_cliente, fp);

    fclose(fp);
}

/////////////////////////////////////////////////////////////////////////
void salvaBackUpArquivoBibliotecaCliente(Cliente **usuario, int *quant_cliente, int *alocacao) {
    FILE *fp = fopen("clienteBackUp.bin", "wb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para backup!\n");
        return;
    }

    // Escreve a quantidade de clientes no início do arquivo
    fwrite(&quant_cliente, sizeof(int), 1, fp);

    // Escreve todos os clientes no backup
    fwrite(usuario, sizeof(Cliente), quant_cliente, fp);

    fclose(fp);
}
*/

///////////////////////////
//                       //
//  funções auxiliares   //
//                       //
///////////////////////////
// retorna o indice que o filme esta presente
int indice_atualiza_cliente (Cliente **usuario, int *quant_cliente, char *cpf){
    for(int i=0; i<*quant_cliente; i++){ // percorre os filmes cadastrados
        if(strcmp((*usuario)[i].cpf, cpf) == 0){ // compara o codigo do filme atual
            return i; // returna o indice do codigo procurado
            }
    }
    return -1; // Código não encontrado
}
// verifica se o codigo é repetido ou não


int aloca_cliente(Cliente **usuario, int *quant_cliente, int *alocacao){

    // se tiver o arquivo pegar do arquivo
    //inicializa se não tiver arquivo
    if (*quant_cliente == 0) {
        *usuario = (Cliente *) malloc((*alocacao) * sizeof(Cliente));
        if (*usuario == NULL) {
            printf("Erro ao alocar memória!\n");
            return 1;
        }
        printf("alocou\n"); // não ultiliza so para verificar se entrou na função
    }
    return 0;
}
// realoca memoria

void realoca_cliente(Cliente **usuario, int *quant_cliente, int *alocacao){

    if(*quant_cliente % 10 == 0){ // Se atingir múltiplo de 10, realoca
        int novo_tamanho = *quant_cliente + *alocacao;
        Cliente *pessoa = realloc(*usuario, sizeof(Cliente) * novo_tamanho);
        if (pessoa == NULL) { // Se realloc falhar
            printf("Erro ao realocar memória! Mantendo a alocação atual.\n");
            return;  // Sai da função sem alterar o ponteiro
        }
        *usuario = pessoa; // Atualiza o ponteiro apenas se realloc tiver sucesso
        *alocacao += 10;  // Aumenta a alocação para próximas realocações
        printf("\nfoi realocado com sucesso\n");
    }
}

/*void salvaArquivoBibliotecaFilmes(BibliotecaFilme *biblioteca){

}*/
///////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////

void cadastra_cliente(Cliente **usuario , int *quant_cliente, int *alocacao){
    int verificacao, repeticao, ehNome, ehIdade, verificacao_idade, ehAluguel, verifica_aluguel;
    // inicializa o vetor de struct se não tiver alocado
    aloca_cliente(usuario, quant_cliente, alocacao);
//----------------------------------- verificaçao cpf --------------------------------

    // 0 amanda, 1 matheus 
    do {
        verificacao = 0;

        printf("Digite seu CPF [deve conter 11 números]: ");
        scanf("%s", (*usuario)[*quant_cliente].cpf);
        while (getchar() != '\n');

        // 1° Verifica se há algum caractere que não seja número
        int apenas_numeros = 1; // Variável auxiliar para validar os caracteres
        for (int i = 0; i < strlen((*usuario)[*quant_cliente].cpf); i++) {
            if (!isdigit((*usuario)[*quant_cliente].cpf[i])) {
                apenas_numeros = 0;
                break;
            }
        }

        if (!apenas_numeros) {
            printf("\nERRO! O CPF deve conter apenas números.\n");
            verificacao = 1;
        } else {
            // 2° Se passou no teste de caracteres, verifica o tamanho
            if (strlen((*usuario)[*quant_cliente].cpf) != 11) {
                printf("\nERRO! O CPF deve conter exatamente 11 números.\n");
                verificacao = 1;
            } else {
                // 3° Se passou nas duas verificações anteriores, verifica se o CPF já foi cadastrado
                repeticao = verifica_cpf_cliente(usuario, quant_cliente, (*usuario)[*quant_cliente].cpf);
                if (repeticao == 1) {
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
        fgets((*usuario)[*quant_cliente].nome, sizeof((*usuario)[*quant_cliente].nome), stdin);

        // Remover o '\n' no final da string, se existir
        size_t len = strlen((*usuario)[*quant_cliente].nome);
        if (len > 0 && (*usuario)[*quant_cliente].nome[len - 1] == '\n') {
            (*usuario)[*quant_cliente].nome[len - 1] = '\0';
        }

        // Verificar se contém apenas letras e espaços
        for (int i = 0; i < strlen((*usuario)[*quant_cliente].nome); i++) {
            if (!isalpha((*usuario)[*quant_cliente].nome[i]) && (*usuario)[*quant_cliente].nome[i] != ' ') {
                ehNome = 1;
                printf("\nERRO! Apenas letras são permitidas. Tente novamente.\n");
                break;
            }
        }

    } while (ehNome != 0);
//----------------------------------- verificaçao idade --------------------------------

    do{
        ehIdade=0 ;
        printf("Digite sua idade: ");
        verificacao_idade = scanf("%d", &(*usuario)[*quant_cliente].idade); // retorna 1 se for um inteiro
        while (getchar() != '\n');
        // verifica se um inteiro
        if (verificacao_idade != 1) {
            printf("\nERRO! A idade precisa ser um número. Digite novamente.\n");
            ehIdade = 1;
        // so aceita maior de idade
        } else if ((*usuario)[*quant_cliente].idade < 18) {
            printf("\nERRO! Você precisa ter 18 anos ou mais para se cadastrar.\n");
            ehIdade = 1;
        }

    }while(ehIdade !=0);
//----------------------------------- verificaçao numAlugueis --------------------------------
    do{
        ehAluguel = 0;
        printf("\nDigite numeros de alugueis já feito: ");
        verifica_aluguel = scanf("%d", &(*usuario)[*quant_cliente].numAlugueis);
        while (getchar() != '\n');

        // verifica se um inteiro
        if (verifica_aluguel != 1) {
            printf("\nERRO! O aluguel precisa ser um número. Digite novamente.\n");
            ehAluguel = 1;
        // tem que ser um numero positivo
        } else if ((*usuario)[*quant_cliente].numAlugueis < 0) {
            printf("\nERRO! Não é aceitos numeros negativos \n");
            ehAluguel = 1;
        }

    }while(ehAluguel != 0);

    (*quant_cliente)++;

    // função que verifica se precisa reallocar memoria
    realoca_cliente(usuario, quant_cliente, alocacao);

    //salvaArquivoBibliotecaFilmes(biblioteca);
    //salvaArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);
    //salvaBackUpArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);

    printf("\ndeu bom\n");
}

///////////////////////////
//                       //
//       Consulta        //
//                       //
///////////////////////////

void consulta_cliente(Cliente **usuario, int *quant_cliente) {
    char cpf[12]; // CPF deve ser uma string, não um int
    int flag = 0; // Inicializa flag com 0

    //falta tratamento de erro do id do filme
    printf("\nDigite seu CPF para a pesquisa: ");
    scanf("%s", cpf);
    while (getchar() != '\n'); // Limpa buffer

    // Verifica se o CPF existe
    for (int i = 0; i < *quant_cliente; i++) {
        if (strcmp((*usuario)[i].cpf, cpf) == 0) { // Comparação correta para strings
            flag = 1; // Marca que encontrou
            printf("\n============= Dados do Cliente ===================\n");
            printf("\nCPF do cliente: %s", (*usuario)[i].cpf);
            printf("\nNome do cliente: %s", (*usuario)[i].nome);
            printf("\nIdade do cliente: %d", (*usuario)[i].idade);
            printf("\nNúmero de aluguéis feitos: %d\n", (*usuario)[i].numAlugueis);
            break; // Sai do loop após encontrar
        }
    }

    if (flag == 0) {
        printf("\nCPF não encontrado! \n");
    }

    printf("\ndeu bom\n");
}
///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_cliente(Cliente **usuario, int *quant_cliente, int *alocacao){
    int  verificacao,verifica_aluguel, verificacao_idade, op, ehNome, indice, ehIdade, ehAluguel;
    char ver_cliente[12];
    printf("\nDigite o CPF do cliente: ");
    scanf("%s", ver_cliente);
    while (getchar() != '\n');
    // verifica se o filme ja existe e 1 se existir
    verificacao = verifica_cpf_cliente(usuario, quant_cliente, ver_cliente);
    if(verificacao == 0){
        printf("\nO codigo não existe!");
    }else{
        printf("\nEscolha uma opção para alteração: ");
        printf("\n\n================ Menu ===================\n");
        printf("\n1- Nome ");
        printf("\n2- Idade ");
        printf("\n3- Numeros de alugueis já feitos ");
        printf("\n4- Sair");
        printf("\n\n=========================================\n");
        scanf("%d", &op);
        while (getchar() != '\n');
        // retorna o indice do codigo igual
        indice = indice_atualiza_cliente (usuario, quant_cliente, ver_cliente);
        switch(op){
            case 1:
                do {
                    ehNome = 0;
                    printf("Digite seu nome: ");

                    // Melhor alternativa: usar fgets()
                    fgets((*usuario)[indice].nome, sizeof((*usuario)[indice].nome), stdin);

                    // Remover o '\n' no final da string, se existir
                    size_t len = strlen((*usuario)[indice].nome);
                    if (len > 0 && (*usuario)[indice].nome[len - 1] == '\n') {
                        (*usuario)[indice].nome[len - 1] = '\0';
                    }

                    // Verificar se contém apenas letras e espaços
                    for (int i = 0; i < strlen((*usuario)[indice].nome); i++) {
                        if (!isalpha((*usuario)[indice].nome[i]) && (*usuario)[indice].nome[i] != ' ') {
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
                    verificacao_idade = scanf("%d", &(*usuario)[indice].idade); // retorna 1 se for um inteiro
                    while (getchar() != '\n');
                    // verifica se um inteiro
                    if (verificacao_idade != 1) {
                        printf("\nERRO! A idade precisa ser um número. Digite novamente.\n");
                        ehIdade = 1;
                    // so aceita maior de idade
                    } else if ((*usuario)[indice].idade < 18) {
                        printf("\nERRO! Você precisa ter 18 anos ou mais para se cadastrar.\n");
                        ehIdade = 1;
                    }

                }while(ehIdade !=0);

                break;
            case 3:
                do{
                    ehAluguel = 0;
                    printf("\nDigite numeros de alugueis já feito: ");
                    verifica_aluguel = scanf("%d", &(*usuario)[indice].numAlugueis);
                    while (getchar() != '\n');

                    // verifica se um inteiro
                    if (verifica_aluguel != 1) {
                        printf("\nERRO! O aluguel precisa ser um número. Digite novamente.\n");
                        ehAluguel = 1;
                    // tem que ser um numero positivo
                    } else if ((*usuario)[indice].numAlugueis < 0) {
                        printf("\nERRO! Não é aceitos numeros negativos \n");
                        ehAluguel = 1;
                    }

                }while(ehAluguel != 0);
                break;
            case 4:
                printf("\nSaindo...");
                break;
            default:
                printf("\nOpção invalida!");
                break;

        }

    }


    //salvaArquivoBibliotecaFilmes(biblioteca);
    //salvaArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);
    //salvaBackUpArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);
}
///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////

void deleta_cliente(Cliente **usuario, int *quant_cliente, int *alocacao){
    int verifica, cod, indice;
    char cpf[12];
    printf("\nDigite o CPF do cliente: ");
    scanf("%s", &cpf);
    while (getchar() != '\n');
    //returna 1 se o codigo existir
    verifica = verifica_cpf_cliente(usuario, quant_cliente, cpf);
    if(verifica == 0){
        printf("\n codigo de filme não existe");
    }else{
        // retorna o indice do codigo
        indice = indice_atualiza_cliente (usuario, quant_cliente, cpf);
        for (int i = indice; i < (*quant_cliente) - 1; i++) {
            (*usuario)[i] = (*usuario)[i + 1];
        }

        (*quant_cliente)--; // Diminui a contagem de filmes
        printf("\nFilme deletado com sucesso! \n");
    }



    //salvaArquivoBibliotecaFilmes(biblioteca);
    //salvaArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);
    //salvaBackUpArquivoBibliotecaCliente(usuario,quant_cliente,alocacao);

    printf("deu bom\n");
}
///////////////////////////
//                       //
//       Filme           //
//                       //
///////////////////////////
///////////////////////////
//                       //
//    Arquivos Filmes    //
//                       //
///////////////////////////

// Função para receber dados de filmes.bin
/*void inicia_filme(Filme **catalogo, int *quant_filme, int *alocacao) {
    FILE *fp = fopen("filme.bin", "rb");

    if (fp == NULL) {
        printf("Arquivo não encontrado. Começando um novo catálogo.\n");
        *quant_filme = 0;
        *alocacao = 10;  // Início com espaço para 10 filmes
        *catalogo = (Filme *)malloc((*alocacao) * sizeof(Filme));
        if (*catalogo == NULL) {
            printf("Erro ao alocar memória!\n");
            return;
        }
        return;
    }

    // Lê a quantidade de filmes armazenados
    fread(quant_filme, sizeof(int), 1, fp);

    // Aloca memória suficiente para os filmes lidos
    *alocacao = *quant_filme + 10;  // Reserva um pouco mais de espaço
    *catalogo = (Filme *)malloc((*alocacao) * sizeof(Filme));
    if (*catalogo == NULL) {
        printf("Erro ao alocar memória!\n");
        fclose(fp);
        exit(1);
    }

    // Lê os filmes do arquivo para o vetor
    fread(*catalogo, sizeof(Filme), *quant_filme, fp);
    fclose(fp);
}
void salvaArquivoBibliotecaFilmes(Filme *catalogo, int *quant_filme, int *alocacao) {
    FILE *fp = fopen("filme.bin", "wb");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Escreve a quantidade de filmes no início do arquivo
    fwrite(&quant_filme, sizeof(int), 1, fp);

    // Escreve todos os filmes de uma vez
    fwrite(catalogo, sizeof(Filme), quant_filme, fp);

    fclose(fp);
}

/////////////////////////////////////////////////////////////////////////
void salvaBackUpArquivoBibliotecaFilmes(Filme *catalogo, int *quant_filme, int *alocacao) {
    FILE *fp = fopen("filmeBackUp.bin", "wb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para backup!\n");
        return;
    }

    // Escreve a quantidade de filmes no início do arquivo
    fwrite(&quant_filme, sizeof(int), 1, fp);

    // Escreve todos os filmes no backup
    fwrite(catalogo, sizeof(Filme), quant_filme, fp);

    fclose(fp);
}
*/

///////////////////////////
//                       //
//  funções auxiliares   //
//                       //
///////////////////////////
/*void escrever_filme (BibliotecaFilmes *biblioteca, char *modo) {

}*/
// retorna o indice que o filme esta presente
int indice_atualiza(Filme **catalogo, int *quant_filme, int codigo){
    for(int i=0; i<*quant_filme; i++){ // percorre os filmes cadastrados
        if((*catalogo)[i].codigo == codigo){ // compara o codigo do filme atual
            return i; // returna o indice do codigo procurado
            }
    }
    return -1; // Código não encontrado
}
// verifica se o codigo é repetido ou não
int verifica_codigo_filme(Filme **catalogo, int *quant_filme, int codigo){

    if(*quant_filme > 0){
        for(int i=0; i<*quant_filme; i++){ // percorre os filmes cadastrados
            if((*catalogo)[i].codigo == codigo){ // compara o codigo do filme atual
                return 1; // returna 1 se o codigo existir
            }
        }
    }
    //salvaArquivoBibliotecaFilmes(biblioteca);

    printf("deu bom\n");
    return 0;// retorna 0 se for um codigo novo
}
// inicializa o vetor
int iniciaBiblioteca(Filme **catalogo, int *quant_filme, int *alocacao){

    // se tiver o arquivo pegar do arquivo
    //inicializa se não tiver arquivo
    if (*quant_filme == 0) {
        *catalogo = (Filme *) malloc((*alocacao) * sizeof(Filme));
        if (*catalogo == NULL) {
            printf("Erro ao alocar memória!\n");
            return 1;
        }
        printf("alocou\n"); // não ultiliza so para verificar se entrou na função
    }
    return 0;
}
// realoca memoria
void realocaMemoriaBiblioteca(Filme **catalogo, int *quant_filme, int *alocacao){

    if(*quant_filme % 10 == 0){ // Se atingir múltiplo de 10, realoca
        int novo_tamanho = *quant_filme + *alocacao;
        Filme *temp = realloc(*catalogo, sizeof(Filme) * novo_tamanho);
        if (temp == NULL) { // Se realloc falhar
            printf("Erro ao realocar memória! Mantendo a alocação atual.\n");
            return;  // Sai da função sem alterar o ponteiro
        }
        *catalogo = temp; // Atualiza o ponteiro apenas se realloc tiver sucesso
        *alocacao += 10;  // Aumenta a alocação para próximas realocações
        printf("\nfoi realocado com sucesso\n");
    }
}

/*void salvaArquivoBibliotecaFilmes(BibliotecaFilme *biblioteca){

}*/
///////////////////////////
//                       //
//       Cadastro        //
//                       //
///////////////////////////

void cadastra_filme(Filme **catalogo , int *quant_filme, int *alocacao){
    int verificacao, ehValido, ehFloat, ehInt, ehLetra, ehAlph, ehBool, verifica_bool;
    char entrada[10], entrada1[10], entrada2[10];
    // inicializa o vetor de struct se não tiver alocado
    iniciaBiblioteca(catalogo, quant_filme, alocacao);
//----------------------------------- verificaçao codigo --------------------------------

    do {
        ehValido = 1;

        printf("\nDigite o código do filme: ");
        scanf("%s", entrada1);  // Lê como string para verificar cada caractere

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
            (*catalogo)[*quant_filme].codigo = atoi(entrada1); // Converte para inteiro
        }

    } while (!ehValido);

//--------------------------------- verificacao titulo -----------------------------------------------
    do{
        ehLetra = 1;// Assume que é válido até encontrar um caractere inválido

        printf("Titulo do Filme: ");
        scanf(" %[^\n]", (*catalogo)[*quant_filme].titulo);
        fflush(stdin);


        // Verifica se a entrada contém apenas letras e espaços
        for(int i =0; i < strlen((*catalogo)[*quant_filme].titulo); i++){
            if(!isalpha((*catalogo)[*quant_filme].titulo[i]) && (*catalogo)[*quant_filme].titulo[i] != ' '){
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
        scanf(" %[^\n]", (*catalogo)[*quant_filme].genero);
        fflush(stdin);

        for(int i=0;i<strlen((*catalogo)[*quant_filme].genero); i++){
            if(!isalpha((*catalogo)[*quant_filme].genero[i]) && (*catalogo)[*quant_filme].genero[i] != ' '){
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

        printf("\nDigite o Preço do filme: ");
        scanf("%s", entrada2);  // Lê como string para verificar cada caractere

        // Verifica se a entrada contém apenas dígitos
        for (int i = 0; i < strlen(entrada2); i++) {
            if (!isdigit(entrada2[i])) {
                ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                break;
            }
        }

        if (!ehValido) {
            printf("\nErro: O código deve conter apenas números! Tente novamente.\n");
        } else {
            (*catalogo)[*quant_filme].codigo = atoi(entrada2); // Converte para inteiro
        }

    } while (!ehValido);
// ---------------------------------------- verificacao disponibilidade ---------------------------------
        do{
            ehBool = 0;
            printf("Disponibilidade do Filme [true ou false]: ");
            scanf("%s", &entrada);

            // Verifica se TODOS os caracteres da entrada são letras (sem números ou símbolos)
            for (int i = 0; i < strlen(entrada); i++) {
                if (!isalpha(entrada[i])) {
                    ehBool = 1; // Marca erro
                    break; // Para a verificação na hora
                }
            }

            if (ehBool == 0) { // Se a entrada tiver apenas letras, verifica se é "true" ou "false"
                if (strcmp(entrada, "true") == 0) {
                    (*catalogo)[*quant_filme].disponibilidade = true;
                } else if (strcmp(entrada, "false") == 0) {
                    (*catalogo)[*quant_filme].disponibilidade = false;
                } else {
                    printf("\nErro: A disponibilidade deve ser 'true' ou 'false'.\n");
                    ehBool = 1;
                }
            } else {
                printf("\nErro: A entrada deve conter apenas letras (sem números ou símbolos).\n");
            }

            while (getchar() != '\n'); // Limpa o buffer do stdin

        }while(ehBool != 0);


    // adiciona a quantidade de filme para + 1
    (*quant_filme)++;

    // função que verifica se precisa reallocar memoria
    realocaMemoriaBiblioteca(catalogo, quant_filme, alocacao);

    //salvaArquivoBibliotecaFilmes(biblioteca);
    //salvaArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);
    //salvaBackUpArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);

    printf("\nFilme cadastrado com sucesso!\n"); // depois de fazer a verificacao da disponibilidade troca na funçao
                           //void atualiza_filme(Filme **catalogo, int *quant_filme)
}

///////////////////////////
//                       //
//       Consulta        //
//                       //
///////////////////////////
void consulta_filme(Filme **catalogo, int *quant_filme ){
    int id_filme, flag;
    printf("\nUse o codigo do filme para a pesquisa: ");
    // **faltaaaa:  tratamento de erro do id_filme
    scanf("%d", &id_filme);

    for(int i=0; i<*quant_filme; i++){
        if(id_filme == (*catalogo)[i].codigo){ //verifica se o filme existe e retorna a 1 se existir
            flag = 1;
        }
    }
    if(flag == 1){
        for(int i=0;i<*quant_filme;i++){
            if(id_filme == ((*catalogo)[i].codigo)){
                printf("\n============= Catalogo Filme ===================\n");
                printf("\nCodigo do filme: %d",((*catalogo)[i].codigo));
                printf("\nTitulo: %s",((*catalogo)[i].titulo));
                printf("\nGenero: %s",((*catalogo)[i].genero));
                printf("\nPreço: %.2f",((*catalogo)[i].preco));
                if(((*catalogo)[i].disponibilidade == "true")){
                    printf("\nFilme Disponível!\n");
                }else{
                    printf("\nFilme Indisponivel...\n");
                }
            }
        }

    }else{
        printf("\n Código não encontrado! \n"); // se o filme não existir
    }

    printf("deu bom\n");
}
///////////////////////////
//                       //
//       alteração       //
//                       //
///////////////////////////

void atualiza_filme(Filme **catalogo, int *quant_filme, int *alocacao){
    int ver_filme, verificacao, op, ehLetra, indice, ehAlph, ehValido, ehBool;
    char entrada[10], entrada2[10];
    printf("\nDigite o codigo do Filme: ");
    scanf("%d", &ver_filme);
    // verifica se o filme ja existe e 1 se existir
    verificacao = verifica_codigo_filme(catalogo, quant_filme, ver_filme);
    if(verificacao == 0){
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
        // retorna o indice do codigo igual
        indice = indice_atualiza(catalogo, quant_filme, ver_filme);
        switch(op){
            case 1:
                do{
                    ehLetra = 1;// Assume que é válido até encontrar um caractere inválido

                    printf("Titulo do Filme: ");
                    scanf(" %[^\n]", (*catalogo)[indice].titulo);
                    fflush(stdin);


                    // Verifica se a entrada contém apenas letras e espaços
                    for(int i =0; i < strlen((*catalogo)[indice].titulo); i++){
                        if(!isalpha((*catalogo)[indice].titulo[i]) && (*catalogo)[indice].titulo[i] != ' '){
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
                    scanf(" %[^\n]", (*catalogo)[indice].genero);
                    fflush(stdin);

                    for(int i=0;i<strlen((*catalogo)[indice].genero); i++){
                        if(!isalpha((*catalogo)[indice].genero[i]) && (*catalogo)[indice].genero[i] != ' '){
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
                    scanf("%s", entrada2);  // Lê como string para verificar cada caractere

                    // Verifica se a entrada contém apenas dígitos
                    for (int i = 0; i < strlen(entrada2); i++) {
                        if (!isdigit(entrada2[i])) {
                            ehValido = 0; // Se encontrar algo que não seja número, marca como inválido
                            break;
                        }
                    }

                    if (!ehValido) {
                        printf("\nErro: O código deve conter apenas números! Tente novamente.\n");
                    } else {
                        (*catalogo)[indice].codigo = atoi(entrada2); // Converte para inteiro
                    }

                } while (!ehValido);
                break;
            case 4:
                //fazer a vericação disponibilidade
                do{
                    ehBool = 0;
                    printf("Disponibilidade do Filme [true ou false]: ");
                    scanf("%s", &entrada);

                    // Verifica se TODOS os caracteres da entrada são letras (sem números ou símbolos)
                    for (int i = 0; i < strlen(entrada); i++) {
                        if (!isalpha(entrada[i])) {
                            ehBool = 1; // Marca erro
                            break; // Para a verificação na hora
                        }
                    }

                    if (ehBool == 0) { // Se a entrada tiver apenas letras, verifica se é "true" ou "false"
                        if (strcmp(entrada, "true") == 0) {
                            (*catalogo)[indice].disponibilidade = true;
                        } else if (strcmp(entrada, "false") == 0) {
                            (*catalogo)[indice].disponibilidade = false;
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

    }

    //salvaArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);
    //salvaBackUpArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);

    //salvaArquivoBibliotecaFilmes(biblioteca);
}
///////////////////////////
//                       //
//       Deletar         //
//                       //
///////////////////////////

void deleta_filme(Filme **catalogo, int *quant_filme, int *alocacao){
    int verifica, cod, indice, ehValido;
    char entrada1[10];

    do {
        ehValido = 1;

        printf("\nDigite o código do filme: ");
        scanf("%s", entrada1);  // Lê como string para verificar cada caractere

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
            cod = atoi(entrada1); // Converte para inteiro
        }

    } while (!ehValido);

    //returna 1 se o codigo existir
    verifica = verifica_codigo_filme(catalogo, quant_filme, cod );

    if(verifica == 0){
        printf("\n codigo de filme não existe");
    }else{
        // retorna o indice do codigo
        indice = indice_atualiza(catalogo, quant_filme, cod);
        for (int i = indice; i < (*quant_filme) - 1; i++) {
            (*catalogo)[i] = (*catalogo)[i + 1];
        }

        (*quant_filme)--; // Diminui a contagem de filmes
    }



    //salvaArquivoBibliotecaFilmes(biblioteca);
    //salvaArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);
    //salvaBackUpArquivoBibliotecaFilmes(catalogo,quant_filme,alocacao);


    printf("deu bom\n");
}
///////////////////////////
//                       //
//   mostrar catalogo    //
//                       //
///////////////////////////
void mostrar_catalogo(Filme **catalogo, int *quant_filme){
    for(int i=0; i<*quant_filme;i++){
        printf("\n============= Catalogo Filme ===================\n");
        printf("\nCodigo do filme: %d",(*catalogo)[i].codigo);
        printf("\nTitulo: %s",(*catalogo)[i].titulo);
        printf("\nGenero: %s",(*catalogo)[i].genero);
        printf("\nPreço: %.2f",(*catalogo)[i].preco);
        if(((*catalogo)[i].disponibilidade == 1)){
            printf("\nFilme Dispon�vel!\n");
        }else{
            printf("\nFilme Indisponivel...\n");
        }

    }
}
///////////////////////////
//                       //
//      Aluguel          //
//                       //
///////////////////////////