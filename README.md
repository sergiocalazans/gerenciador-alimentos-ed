# Data Food Management System

Um sistema eficiente de gerenciamento e indexação de dados nutricionais desenvolvido em **C**. O projeto utiliza uma arquitetura de processamento em duas etapas: a conversão de dados brutos (CSV) para binário e a manipulação avançada desses dados em memória através de listas ligadas e árvores binárias.

## 🏗️ Estrutura do Projeto

O sistema é dividido em módulos para garantir a organização e separação de responsabilidades:

- **`alimentos.h`**: Cabeçalho contendo as definições de estruturas (`Alimento`, `NoCategoria`, `NoArvore`), tipos enumerados e protótipos de funções.
- **`alimentos.c`**: Implementação da lógica de I/O, manipulação de árvores binárias, busca por faixas e gerenciamento de memória.
- **`P1.c`**: Programa de pré-processamento que converte arquivos CSV para o formato binário proprietário.
- **`P2.c`**: Programa principal que fornece a interface de menu e as funcionalidades de análise de dados.

## 🚀 Fluxo de Funcionamento

O projeto opera em um pipeline de dados de duas fases:

### 1. Preparação de Dados (P1)

O programa **P1** lê um arquivo no formato CSV (ex: `taco_4_edicao.csv`), realiza o _parsing_ dos campos e gera um arquivo binário (`dados.bin`).

- Filtra campos específicos: Número, Descrição, Energia (kcal) e Proteína (g).
- Mapeia descrições de texto para categorias formais via `enum`.

### 2. Gerenciamento e Consulta (P2)

O programa **P2** carrega o arquivo binário e organiza os dados em memória utilizando:

- **Lista Ligada Alfabética**: Categorias e alimentos são mantidos em ordem crescente de nome.
- **Árvores Binárias de Busca (BST)**: Cada categoria possui duas árvores de indexação (uma para Energia e outra para Proteína), permitindo buscas e ordenações em tempo logarítmico.

## 🛠️ Funcionalidades do Menu (P2)

O sistema permite ao usuário:

1.  Listar todas as categorias cadastradas.
2.  Visualizar alimentos de uma categoria em ordem alfabética.
3.  Listar alimentos por ordem decrescente de Energia ou Proteína (via percurso em árvore).
4.  Consultar alimentos dentro de uma faixa de valores (ex: entre 10 e 50g de proteína).
5.  Remover alimentos ou categorias inteiras com atualização automática dos índices.
6.  Salvar alterações de volta no arquivo binário ao sair.

## 💻 Compilação e Execução

Para compilar e rodar o projeto completo, siga os passos abaixo:

**Passo 1: Gerar o binário (P1)**

```bash
gcc -o p1 P1.c alimentos.c
./p1
```

**Passo 2: Executar o sistema (P2)**

```bash
gcc -o p2 P2.c alimentos.c
./p2
```

## 📊 Especificações Técnicas

| Estrutura                | Uso                     | Algoritmo                        |
| :----------------------- | :---------------------- | :------------------------------- |
| **Lista Ligada**         | Armazenamento principal | Inserção ordenada                |
| **Árvore Binária (BST)** | Indexação de Nutrientes | Percurso in-order para ordenação |
| **Arquivo Binário**      | Persistência de Dados   | Acesso via `struct` direta       |

## Desenvolvimento

Esse projeto foi desenvolvido durante a disciplina 'Programação Imperativa', cursada no 2° Período do curso de Bacharelado em Ciência da Computação na Pontifícia Universidade Católica do Paraná (PUCPR).

Feito por: Sérgio Calazans

---
