# CRUD de Clientes (C)

Projeto em C para cadastro e gerenciamento de clientes via menu no terminal.
Inclui operações de criar, listar, buscar, editar e remover, com persistência em arquivo CSV.

## Funcionalidades
- Cadastrar cliente (ID, nome, telefone, e-mail)
- Listar clientes
- Buscar por ID
- Editar dados do cliente
- Remoção lógica (marcar como inativo)
- Salvar e carregar dados do arquivo `clientes.csv`

## Como compilar e executar

### Linux / macOS
```bash
gcc src/main.c -o app
./app
