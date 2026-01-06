#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTES 100
#define ARQ_CLIENTES "clientes.csv"

typedef struct {
    int id;
    char nome[60];
    char telefone[20];
    char email[60];
    int ativo; // 1 = ativo, 0 = removido
} Cliente;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerLinha(char *dest, int tam) {
    fgets(dest, tam, stdin);
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}

int menu() {
    int op;
    printf("\n==== CRUD DE CLIENTES ====\n");
    printf("1) Cadastrar cliente\n");
    printf("2) Listar clientes\n");
    printf("3) Buscar por ID\n");
    printf("4) Editar cliente\n");
    printf("5) Remover cliente\n");
    printf("6) Salvar em arquivo\n");
    printf("7) Carregar do arquivo\n");
    printf("0) Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    limparBuffer();
    return op;
}

int encontrarIndicePorId(Cliente v[], int n, int id) {
    for (int i = 0; i < n; i++) {
        if (v[i].ativo == 1 && v[i].id == id) return i;
    }
    return -1;
}

void cadastrarCliente(Cliente v[], int *n) {
    if (*n >= MAX_CLIENTES) {
        printf("Limite de clientes atingido.\n");
        return;
    }

    Cliente c;
    c.ativo = 1;

    printf("ID (numero): ");
    scanf("%d", &c.id);
    limparBuffer();

    // evita ID repetido
    if (encontrarIndicePorId(v, *n, c.id) != -1) {
        printf("ID ja cadastrado.\n");
        return;
    }

    printf("Nome: ");
    lerLinha(c.nome, sizeof(c.nome));

    printf("Telefone: ");
    lerLinha(c.telefone, sizeof(c.telefone));

    printf("Email: ");
    lerLinha(c.email, sizeof(c.email));

    v[*n] = c;
    (*n)++;

    printf("Cliente cadastrado com sucesso.\n");
}

void listarClientes(Cliente v[], int n) {
    printf("\n--- LISTA DE CLIENTES ---\n");
    int encontrou = 0;
    for (int i = 0; i < n; i++) {
        if (v[i].ativo == 1) {
            printf("ID: %d | Nome: %s | Tel: %s | Email: %s\n",
                   v[i].id, v[i].nome, v[i].telefone, v[i].email);
            encontrou = 1;
        }
    }
    if (!encontrou) printf("Nenhum cliente cadastrado.\n");
}

void buscarPorId(Cliente v[], int n) {
    int id;
    printf("Digite o ID: ");
    scanf("%d", &id);
    limparBuffer();

    int idx = encontrarIndicePorId(v, n, id);
    if (idx == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    printf("Encontrado: ID: %d | Nome: %s | Tel: %s | Email: %s\n",
           v[idx].id, v[idx].nome, v[idx].telefone, v[idx].email);
}

void editarCliente(Cliente v[], int n) {
    int id;
    printf("Digite o ID do cliente para editar: ");
    scanf("%d", &id);
    limparBuffer();

    int idx = encontrarIndicePorId(v, n, id);
    if (idx == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    printf("Novo nome (enter para manter): ");
    char temp[60];
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(v[idx].nome, temp, sizeof(v[idx].nome));

    printf("Novo telefone (enter para manter): ");
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(v[idx].telefone, temp, sizeof(v[idx].telefone));

    printf("Novo email (enter para manter): ");
    lerLinha(temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(v[idx].email, temp, sizeof(v[idx].email));

    printf("Cliente atualizado.\n");
}

void removerCliente(Cliente v[], int n) {
    int id;
    printf("Digite o ID do cliente para remover: ");
    scanf("%d", &id);
    limparBuffer();

    int idx = encontrarIndicePorId(v, n, id);
    if (idx == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    v[idx].ativo = 0; // remocao logica
    printf("Cliente removido.\n");
}

void salvarArquivo(Cliente v[], int n) {
    FILE *f = fopen(ARQ_CLIENTES, "w");
    if (!f) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    // cabecalho simples (opcional)
    fprintf(f, "id,nome,telefone,email,ativo\n");

    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%s,%s,%d\n",
                v[i].id, v[i].nome, v[i].telefone, v[i].email, v[i].ativo);
    }

    fclose(f);
    printf("Dados salvos em %s\n", ARQ_CLIENTES);
}

void carregarArquivo(Cliente v[], int *n) {
    FILE *f = fopen(ARQ_CLIENTES, "r");
    if (!f) {
        printf("Arquivo %s nao encontrado.\n", ARQ_CLIENTES);
        return;
    }

    char linha[256];
    *n = 0;

    // pula cabecalho
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f) && *n < MAX_CLIENTES) {
        Cliente c;
        char *p = strtok(linha, ",");
        if (!p) continue;
        c.id = atoi(p);

        p = strtok(NULL, ",");
        if (!p) continue;
        strncpy(c.nome, p, sizeof(c.nome));
        c.nome[strcspn(c.nome, "\n")] = 0;

        p = strtok(NULL, ",");
        if (!p) continue;
        strncpy(c.telefone, p, sizeof(c.telefone));
        c.telefone[strcspn(c.telefone, "\n")] = 0;

        p = strtok(NULL, ",");
        if (!p) continue;
        strncpy(c.email, p, sizeof(c.email));
        c.email[strcspn(c.email, "\n")] = 0;

        p = strtok(NULL, ",");
        c.ativo = p ? atoi(p) : 1;

        v[*n] = c;
        (*n)++;
    }

    fclose(f);
    printf("Dados carregados de %s (%d registros)\n", ARQ_CLIENTES, *n);
}

int main() {
    Cliente clientes[MAX_CLIENTES];
    int n = 0;

    int op;
    do {
        op = menu();
        switch (op) {
            case 1: cadastrarCliente(clientes, &n); break;
            case 2: listarClientes(clientes, n); break;
            case 3: buscarPorId(clientes, n); break;
            case 4: editarCliente(clientes, n); break;
            case 5: removerCliente(clientes, n); break;
            case 6: salvarArquivo(clientes, n); break;
            case 7: carregarArquivo(clientes, &n); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (op != 0);

    return 0;
}
