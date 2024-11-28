#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_CLIENTES 100
#define TAM_NOME 100
#define TAM_ENDERECO 200
#define TAM_SENHA 5 // 4 d�gitos + '\0'
#define ARQUIVO "clientes.txt"

// C�digos de cor ANSI
#define COR_RESET "\033[0m"
#define COR_VERDE "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_CIANO "\033[1;36m"

// Estrutura para armazenar os dados dos clientes
typedef struct {
    char nome[TAM_NOME];
    char endereco[TAM_ENDERECO];
    char senha[TAM_SENHA];  // Senha de 4 d�gitos
} Cliente;

// Fun��es para apar�ncia
void limparTela() {
    system("clear || cls");
}

void centralizarTexto(const char* texto, const char* cor) {
    int larguraTerminal = 50;  // Ajuste conforme necess�rio
    int padding = (larguraTerminal - strlen(texto)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", cor, texto, COR_RESET);
}

// Fun��o para carregar clientes do arquivo
int carregarClientes(Cliente clientes[], int *qtd) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        return 0; // Arquivo ainda n�o existe
    }
    while (fscanf(arquivo, "%[^\n]\n%[^\n]\n%[^\n]\n", clientes[*qtd].nome, clientes[*qtd].endereco, clientes[*qtd].senha) != EOF) {
        (*qtd)++;
    }
    fclose(arquivo);
    return 1;
}

// Fun��o para salvar clientes no arquivo
void salvarClientes(Cliente clientes[], int qtd) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%s\n%s\n%s\n", clientes[i].nome, clientes[i].endereco, clientes[i].senha);
    }
    fclose(arquivo);
}

// Fun��o para cadastrar um novo cliente
void cadastrarCliente(Cliente clientes[], int *qtd) {
    if (*qtd >= MAX_CLIENTES) {
        centralizarTexto("N�mero m�ximo de clientes atingido!", COR_VERMELHO);
        return;
    }

    printf("\nDigite o nome do cliente: ");
    getchar(); // Limpar o buffer do teclado
    fgets(clientes[*qtd].nome, TAM_NOME, stdin);
    clientes[*qtd].nome[strcspn(clientes[*qtd].nome, "\n")] = 0; // Remover o '\n'

    printf("Digite o endere�o do cliente: ");
    fgets(clientes[*qtd].endereco, TAM_ENDERECO, stdin);
    clientes[*qtd].endereco[strcspn(clientes[*qtd].endereco, "\n")] = 0; // Remover o '\n'

    // Cadastrar senha de 4 d�gitos
    do {
        printf("Digite uma senha de 4 d�gitos: ");
        scanf("%s", clientes[*qtd].senha);
    } while (strlen(clientes[*qtd].senha) != 4); // Garantir que a senha tem 4 d�gitos

    (*qtd)++;
    salvarClientes(clientes, *qtd);
    centralizarTexto("Cliente cadastrado com sucesso!", COR_VERDE);
}

// Fun��o para listar os clientes cadastrados
void listarClientes(Cliente clientes[], int qtd) {
    if (qtd == 0) {
        centralizarTexto("Nenhum cliente cadastrado.", COR_VERMELHO);
        return;
    }

    printf("\n========= Lista de Clientes =========\n");
    for (int i = 0; i < qtd; i++) {
        printf("Cliente %d: %s - %s\n", i + 1, clientes[i].nome, clientes[i].endereco);
    }
    printf("=====================================\n");
}

// Fun��o para autenticar o cliente
void autenticarCliente(Cliente clientes[], int qtd) {
    char senha[TAM_SENHA];
    int autenticado = 0;

    printf("\nDigite sua senha de 4 d�gitos: ");
    scanf("%s", senha);

    for (int i = 0; i < qtd; i++) {
        if (strcmp(clientes[i].senha, senha) == 0) {
            centralizarTexto("Acesso liberado!", COR_VERDE);
            printf("Bem-vindo, %s.\n", clientes[i].nome);
            autenticado = 1;
            break;
        }
    }

    if (!autenticado) {
        centralizarTexto("Senha incorreta. Acesso negado.", COR_VERMELHO);
    }
}

// Fun��o para excluir um cliente
void excluirCliente(Cliente clientes[], int *qtd) {
    if (*qtd == 0) {
        centralizarTexto("Nenhum cliente cadastrado.", COR_VERMELHO);
        return;
    }

    int indice;
    listarClientes(clientes, *qtd);
    printf("\nDigite o n�mero do cliente para excluir: ");
    scanf("%d", &indice);

    if (indice < 1 || indice > *qtd) {
        centralizarTexto("Cliente inv�lido!", COR_VERMELHO);
        return;
    }

    for (int i = indice - 1; i < *qtd - 1; i++) {
        clientes[i] = clientes[i + 1];
    }
    (*qtd)--;
    salvarClientes(clientes, *qtd);
    centralizarTexto("Cliente exclu�do com sucesso!", COR_VERDE);
}

// Fun��o para exibir o menu
void menu() {
    printf("\n" COR_CIANO "========== MENU ==========\n" COR_RESET);
    printf("1. Listar clientes\n");
    printf("2. Cadastrar cliente\n");
    printf("3. Autenticar cliente\n");
    printf("4. Excluir cliente\n");
    printf("5. Sair\n");
    printf(COR_CIANO "==========================\n" COR_RESET);
    printf("Escolha uma op��o: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura��o para suportar caracteres em portugu�s

    Cliente clientes[MAX_CLIENTES];
    int qtdClientes = 0;
    int opcao;

    // Carrega clientes do arquivo se existir
    carregarClientes(clientes, &qtdClientes);

    do {
        limparTela();
        centralizarTexto("=== Bem-vindo ao Sistema de Academia ===", COR_CIANO);
        menu();
        scanf("%d", &opcao);

        limparTela();  // Limpa a tela antes de cada a��o para melhorar a organiza��o

        switch (opcao) {
            case 1:
                listarClientes(clientes, qtdClientes);
                break;
            case 2:
                cadastrarCliente(clientes, &qtdClientes);
                break;
            case 3:
                autenticarCliente(clientes, qtdClientes);
                break;
            case 4:
                excluirCliente(clientes, &qtdClientes);
                break;
            case 5:
                centralizarTexto("Saindo... At� logo!", COR_VERDE);
                break;
            default:
                centralizarTexto("Op��o inv�lida! Tente novamente.", COR_VERMELHO);
                break;
        }

        printf("\nPressione Enter para continuar...");
        getchar();
        getchar();

    } while (opcao != 5);

    return 0;
}

