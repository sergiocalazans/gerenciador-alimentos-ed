// Arquivo: dados_alimentos.h
// Descrição: Define as estruturas de dados e os protótipos de funções
//            para o gerenciamento de informações sobre alimentos.

#ifndef ALIMENTOS_H
#define ALIMENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definições de Estruturas e Tipos ---

// Requisito 1: Representação das categorias por meio de um enumerado.
typedef enum {
    CEREAIS_E_DERIVADOS, VERDURAS_HORTALICAS_E_DERIVADOS, FRUTAS_E_DERIVADOS,
    GORDURAS_E_OLEOS, PESCADOS_E_FRUTOS_DO_MAR, CARNES_E_DERIVADOS,
    LEITE_E_DERIVADOS, BEBIDAS, OVOS_E_DERIVADOS,
    PRODUTOS_ACUCARADOS, MISCELANEAS, OUTROS_INDUSTRIALIZADOS,
    ALIMENTOS_PREPARADOS, LEGUMINOSAS_E_DERIVADOS, NOZES_E_SEMENTES,
    NUM_CATEGORIAS // Utilitário para contar o total de categorias
} CategoriaID;

// Estrutura para os dados crus de um alimento.
// É esta estrutura que será salva e lida do arquivo binário.
typedef struct {
    int numero;
    char descricao[200];
    int energia_kcal;
    float proteina_g;
    CategoriaID categoria_id;
} Alimento;

// --- Estruturas para a organização em memória (Listas e Árvores) ---

// Forward declaration para resolver dependência cíclica de ponteiros
struct NoAlimento;

// Requisito C.b: Nó da Árvore Binária de Indexação
typedef struct NoArvore {
    float chave;                    // Valor da energia ou proteína
    struct NoAlimento *alimento_ptr; // Aponta para o nó na lista ligada
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

// Requisito C.a.2: Nó da Lista Ligada de Alimentos
typedef struct NoAlimento {
    Alimento dados;
    struct NoAlimento *proximo;
} NoAlimento;

// Requisito C.a.1: Nó da Lista Ligada de Categorias
typedef struct NoCategoria {
    CategoriaID id;
    char nome[100];
    NoAlimento *alimentos_head;     // Cabeça da lista de alimentos
    NoArvore *arvore_energia_root;  // Raiz da árvore de energia
    NoArvore *arvore_proteina_root; // Raiz da árvore de proteína
    struct NoCategoria *proximo;
} NoCategoria;


// --- Protótipos das Funções ---

// Funções para P1 (Leitura de CSV e criação de binário)
void lerDadosDoCSVECriarBinario(const char* arquivo_csv, const char* arquivo_bin);

// Funções para P2 (Carregamento, manipulação e salvamento)
NoCategoria* carregarDadosDeBinario(const char *nome_arquivo);
void salvarDadosEmBinario(NoCategoria *categorias_head, const char *nome_arquivo);
void liberarMemoria(NoCategoria *categorias_head);

// Funções do Menu Interativo (P2)
void listarTodasCategorias(NoCategoria *categorias_head);
NoCategoria* selecionarCategoria(NoCategoria *categorias_head);
void listarAlimentosDeCategoria(NoCategoria *categoria_selecionada);
void listarAlimentosPorOrdemDecrescente(NoArvore *raiz_arvore);
void listarAlimentosPorFaixa(NoArvore *raiz_arvore, float min, float max);
void removerCategoria(NoCategoria **categorias_head, NoCategoria* categoria_a_remover);
void removerAlimento(NoCategoria *categoria, int numero_alimento);

#endif