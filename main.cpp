#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_CLIENTES 100
#define TAM_NOME 100
#define TAM_ENDERECO 200
#define TAM_SENHA 5 
#define ARQUIVO "clientes.txt"

// Códigos de cor ANSI
#define COR_RESET "\033[0m"
#define COR_VERDE "\033[1;32m"
#define COR_VERMELHO "\033[1;31m"
#define COR_CIANO "\033[1;36m"

// Estrutura para armazenar os dados dos clientes
typedef struct {
    char nome[TAM_NOME];
    char endereco[TAM_ENDERECO];
    char senha[TAM_SENHA];  
} Cliente;

// Funções para aparência
void limparTela() {
    system("clear || cls");
}

void centralizarTexto(const char* texto, const char* cor) {
    int larguraTerminal = 50;  
    int padding = (larguraTerminal - strlen(texto)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", cor, texto, COR_RESET);
}

// Função para carregar clientes do arquivo
int carregarClientes(Cliente clientes[], int *qtd) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        return 0; 
    }
    while (fscanf(arquivo, "%[^\n]\n%[^\n]\n%[^\n]\n", clientes[*qtd].nome, clientes[*qtd].endereco, clientes[*qtd].senha) != EOF) {
        (*qtd)++;
    }
    fclose(arquivo);
    return 1;
}

// Função para salvar clientes no arquivo
void salvarClientes(Cliente clientes[], int qtd) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%s\n%s\n%s\n", clientes[i].nome, clientes[i].endereco, clientes[i].senha);
    }
    fclose(arquivo);
}

// Função para cadastrar um novo cliente
void cadastrarCliente(Cliente clientes[], int *qtd) {
    if (*qtd >= MAX_CLIENTES) {
        centralizarTexto("Número máximo de clientes atingido!", COR_VERMELHO);
        return;
    }

    printf("\nDigite o nome do cliente: ");
    getchar(); 
    fgets(clientes[*qtd].nome, TAM_NOME, stdin);
    clientes[*qtd].nome[strcspn(clientes[*qtd].nome, "\n")] = 0; 

    printf("Digite o endereço do cliente: ");
    fgets(clientes[*qtd].endereco, TAM_ENDERECO, stdin);
    clientes[*qtd].endereco[strcspn(clientes[*qtd].endereco, "\n")] = 0; 

    // Cadastrar senha de 4 dígitos
    do {
        printf("Digite uma senha de 4 dígitos: ");
        scanf("%s", clientes[*qtd].senha);
    } while (strlen(clientes[*qtd].senha) != 4); 

    (*qtd)++;
    salvarClientes(clientes, *qtd);
    centralizarTexto("Cliente cadastrado com sucesso!", COR_VERDE);
}

// Função para listar os clientes cadastrados
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

// Função para autenticar o cliente
void autenticarCliente(Cliente clientes[], int qtd) {
    char senha[TAM_SENHA];
    int autenticado = 0;

    printf("\nDigite sua senha de 4 dígitos: ");
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

// Função para excluir um cliente
void excluirCliente(Cliente clientes[], int *qtd) {
    if (*qtd == 0) {
        centralizarTexto("Nenhum cliente cadastrado.", COR_VERMELHO);
        return;
    }

    int indice;
    listarClientes(clientes, *qtd);
    printf("\nDigite o número do cliente para excluir: ");
    scanf("%d", &indice);

    if (indice < 1 || indice > *qtd) {
        centralizarTexto("Cliente inválido!", COR_VERMELHO);
        return;
    }

    for (int i = indice - 1; i < *qtd - 1; i++) {
        clientes[i] = clientes[i + 1];
    }
    (*qtd)--;
    salvarClientes(clientes, *qtd);
    centralizarTexto("Cliente excluído com sucesso!", COR_VERDE);
}

// Função para exibir o menu
void menu() {
    printf("\n" COR_CIANO "========== MENU ==========\n" COR_RESET);
    printf("1. Listar clientes\n");
    printf("2. Cadastrar cliente\n");
    printf("3. Autenticar cliente\n");
    printf("4. Excluir cliente\n");
    printf("5. Sair\n");
    printf(COR_CIANO "==========================\n" COR_RESET);
    printf("Escolha uma opção: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configuração para suportar caracteres em português

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

        limparTela();  

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
                centralizarTexto("Saindo... Até logo!", COR_VERDE);
                break;
            default:
                centralizarTexto("Opção inválida! Tente novamente.", COR_VERMELHO);
                break;
        }

        printf("\nPressione Enter para continuar...");
        getchar();
        getchar();

    } while (opcao != 5);

    return 0;
}

