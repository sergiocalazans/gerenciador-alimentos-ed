// Arquivo: alimentos.c
// Descrição: Implementa a lógica de manipulação das estruturas de dados
//            de alimentos, incluindo listas, árvores e I/O de arquivos.

#include "alimentos.h"

// --- Funções Auxiliares Internas ---

// Converte um enum CategoriaID para sua representação em string.
const char *categoriaIdParaString(CategoriaID id)
{
  static const char *nomes[] = {
      "Cereais e derivados",
      "Verduras hortalicas e derivados",
      "Frutas e derivados",
      "Gorduras e oleos",
      "Pescados e frutos do mar",
      "Carnes e derivados",
      "Leite e derivados",
      "Bebidas",
      "Ovos e derivados",
      "Produtos acucarados",
      "Miscelaneas",
      "Outros alimentos industrializados",
      "Alimentos preparados",
      "Leguminosas e derivados",
      "Nozes e sementes"};
  if (id >= 0 && id < NUM_CATEGORIAS)
  {
    return nomes[id];
  }
  return "CATEGORIA INVALIDA";
}

// Converte uma string de categoria para o enum CategoriaID.
CategoriaID stringParaCategoriaId(const char *str)
{
  for (int i = 0; i < NUM_CATEGORIAS; i++)
  {
    if (strcmp(str, categoriaIdParaString((CategoriaID)i)) == 0)
    {
      return (CategoriaID)i; // Encontrou, retorna o ID correto
    }
  }

  // Se o loop terminar, a categoria do CSV não foi encontrada no nosso código.
  fprintf(stderr, "AVISO: Categoria desconhecida no CSV: '%s'\n", str);
  return CEREAIS_E_DERIVADOS; // Retorna um padrão seguro para evitar que o programa quebre
}

// Função recursiva para inserir um nó em uma Árvore Binária de Busca.
NoArvore *inserirNoArvore(NoArvore *raiz, float chave, NoAlimento *alimento_ptr)
{
  if (raiz == NULL)
  {
    raiz = (NoArvore *)malloc(sizeof(NoArvore));
    if (raiz == NULL)
    {
      perror("Falha ao alocar memoria para no de arvore");
      exit(1);
    }
    raiz->chave = chave;
    raiz->alimento_ptr = alimento_ptr;
    raiz->esquerda = raiz->direita = NULL;
  }
  else if (chave <= raiz->chave)
  {
    raiz->esquerda = inserirNoArvore(raiz->esquerda, chave, alimento_ptr);
  }
  else
  {
    raiz->direita = inserirNoArvore(raiz->direita, chave, alimento_ptr);
  }
  return raiz;
}

// Libera a memória de uma árvore recursivamente.
void liberarArvore(NoArvore *raiz)
{
  if (raiz != NULL)
  {
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
  }
}

// Constrói/reconstrói as árvores de energia e proteína para uma dada categoria.
void construirArvoresParaCategoria(NoCategoria *categoria)
{
  liberarArvore(categoria->arvore_energia_root);
  liberarArvore(categoria->arvore_proteina_root);
  categoria->arvore_energia_root = NULL;
  categoria->arvore_proteina_root = NULL;

  NoAlimento *alimento_atual = categoria->alimentos_head;
  while (alimento_atual != NULL)
  {
    categoria->arvore_energia_root = inserirNoArvore(categoria->arvore_energia_root, alimento_atual->dados.energia_kcal, alimento_atual);
    categoria->arvore_proteina_root = inserirNoArvore(categoria->arvore_proteina_root, alimento_atual->dados.proteina_g, alimento_atual);
    alimento_atual = alimento_atual->proximo;
  }
}

// Insere um alimento na estrutura de listas, mantendo a ordem alfabética.
void inserirAlimentoNaEstrutura(NoCategoria **categorias_head, Alimento novo_alimento)
{
  NoCategoria *cat_anterior = NULL;
  NoCategoria *cat_atual = *categorias_head;
  const char *nome_categoria = categoriaIdParaString(novo_alimento.categoria_id);

  while (cat_atual != NULL && strcmp(cat_atual->nome, nome_categoria) < 0)
  {
    cat_anterior = cat_atual;
    cat_atual = cat_atual->proximo;
  }

  if (cat_atual == NULL || strcmp(cat_atual->nome, nome_categoria) != 0)
  {
    NoCategoria *nova_categoria = (NoCategoria *)malloc(sizeof(NoCategoria));
    nova_categoria->id = novo_alimento.categoria_id;
    strcpy(nova_categoria->nome, nome_categoria);
    nova_categoria->alimentos_head = NULL;
    nova_categoria->arvore_energia_root = NULL;
    nova_categoria->arvore_proteina_root = NULL;
    nova_categoria->proximo = cat_atual;
    if (cat_anterior == NULL)
      *categorias_head = nova_categoria;
    else
      cat_anterior->proximo = nova_categoria;
    cat_atual = nova_categoria;
  }

  NoAlimento *novo_no_alimento = (NoAlimento *)malloc(sizeof(NoAlimento));
  novo_no_alimento->dados = novo_alimento;

  NoAlimento *alim_anterior = NULL;
  NoAlimento *alim_atual = cat_atual->alimentos_head;
  while (alim_atual != NULL && strcmp(alim_atual->dados.descricao, novo_alimento.descricao) < 0)
  {
    alim_anterior = alim_atual;
    alim_atual = alim_atual->proximo;
  }

  novo_no_alimento->proximo = alim_atual;
  if (alim_anterior == NULL)
    cat_atual->alimentos_head = novo_no_alimento;
  else
    alim_anterior->proximo = novo_no_alimento;
}

// --- Implementação das Funções da Interface ---

void lerDadosDoCSVECriarBinario(const char *arquivo_csv, const char *arquivo_bin)
{
  FILE *in = fopen(arquivo_csv, "r");
  if (!in)
  {
    perror("Erro ao abrir CSV");
    exit(1);
  }
  FILE *out = fopen(arquivo_bin, "wb");
  if (!out)
  {
    perror("Erro ao criar binario");
    fclose(in);
    exit(1);
  }

  char linha[1024];
  fgets(linha, 1024, in); // Pula cabeçalho

  while (fgets(linha, 1024, in))
  {
    Alimento a;
    char categoria_str[100];

    // Formato: Numero;Descricao;Umidade;Energia;Proteina;Carboidrato;Categoria
    sscanf(linha, "%d;%[^;];%*f;%d;%f;%*f;%[^\n]",
           &a.numero, a.descricao, &a.energia_kcal, &a.proteina_g, categoria_str);

    a.categoria_id = stringParaCategoriaId(categoria_str);
    fwrite(&a, sizeof(Alimento), 1, out);
  }
  fclose(in);
  fclose(out);
}

NoCategoria *carregarDadosDeBinario(const char *nome_arquivo)
{
  FILE *in = fopen(nome_arquivo, "rb");
  if (!in)
  {
    perror("Erro ao abrir binario");
    exit(1);
  }

  NoCategoria *categorias_head = NULL;
  Alimento a;
  while (fread(&a, sizeof(Alimento), 1, in) == 1)
  {
    inserirAlimentoNaEstrutura(&categorias_head, a);
  }
  fclose(in);

  NoCategoria *cat_atual = categorias_head;
  while (cat_atual != NULL)
  {
    construirArvoresParaCategoria(cat_atual);
    cat_atual = cat_atual->proximo;
  }
  return categorias_head;
}

void salvarDadosEmBinario(NoCategoria *categorias_head, const char *nome_arquivo)
{
  FILE *out = fopen(nome_arquivo, "wb");
  if (!out)
  {
    perror("Erro ao salvar binario");
    exit(1);
  }

  NoCategoria *cat_atual = categorias_head;
  while (cat_atual != NULL)
  {
    NoAlimento *alim_atual = cat_atual->alimentos_head;
    while (alim_atual != NULL)
    {
      fwrite(&(alim_atual->dados), sizeof(Alimento), 1, out);
      alim_atual = alim_atual->proximo;
    }
    cat_atual = cat_atual->proximo;
  }
  fclose(out);
}

void listarTodasCategorias(NoCategoria *categorias_head)
{
  printf("\n--- Categorias de Alimentos ---\n");
  NoCategoria *atual = categorias_head;
  int i = 1;
  while (atual != NULL)
  {
    printf("%d. %s\n", i++, atual->nome);
    atual = atual->proximo;
  }
  printf("---------------------------------\n");
}

NoCategoria *selecionarCategoria(NoCategoria *categorias_head)
{
  listarTodasCategorias(categorias_head);
  printf("Digite o numero da categoria: ");

  char buffer[100];
  int escolha = -1;
  if (fgets(buffer, sizeof(buffer), stdin))
  {
    sscanf(buffer, "%d", &escolha);
  }

  NoCategoria *atual = categorias_head;
  int i = 1;
  while (atual != NULL && i < escolha)
  {
    atual = atual->proximo;
    i++;
  }
  if (atual == NULL)
  {
    printf("Opcao invalida.\n");
  }
  return atual;
}

void listarAlimentosDeCategoria(NoCategoria *categoria)
{
  if (!categoria)
    return;
  printf("\n--- Alimentos em '%s' ---\n", categoria->nome);
  printf("| %-4s | %-55s | %-12s | %-12s |\n", "Num", "Descricao", "Energia(Kcal)", "Proteina(g)");
  printf("|------|---------------------------------------------------------|--------------|--------------|\n");
  NoAlimento *atual = categoria->alimentos_head;
  if (atual == NULL)
  {
    printf("| Nenhum alimento encontrado.                                                              |\n");
  }
  else
  {
    while (atual != NULL)
    {
      printf("| %-4d | %-55s | %-12d | %-12.2f |\n",
             atual->dados.numero, atual->dados.descricao,
             atual->dados.energia_kcal, atual->dados.proteina_g);
      atual = atual->proximo;
    }
  }
  printf("+--------------------------------------------------------------------------------------------+\n");
}

void listarAlimentosPorOrdemDecrescente(NoArvore *raiz)
{
  if (raiz != NULL)
  {
    listarAlimentosPorOrdemDecrescente(raiz->direita);
    printf("| %-4d | %-55s | %-12d | %-12.2f |\n",
           raiz->alimento_ptr->dados.numero, raiz->alimento_ptr->dados.descricao,
           raiz->alimento_ptr->dados.energia_kcal, raiz->alimento_ptr->dados.proteina_g);
    listarAlimentosPorOrdemDecrescente(raiz->esquerda);
  }
}

void listarAlimentosPorFaixa(NoArvore *raiz, float min, float max)
{
  if (raiz != NULL)
  {
    if (raiz->chave > min)
    {
      listarAlimentosPorFaixa(raiz->esquerda, min, max);
    }
    if (raiz->chave >= min && raiz->chave <= max)
    {
      printf("| %-4d | %-55s | %-12d | %-12.2f |\n",
             raiz->alimento_ptr->dados.numero, raiz->alimento_ptr->dados.descricao,
             raiz->alimento_ptr->dados.energia_kcal, raiz->alimento_ptr->dados.proteina_g);
    }
    if (raiz->chave < max)
    {
      listarAlimentosPorFaixa(raiz->direita, min, max);
    }
  }
}

void removerCategoria(NoCategoria **categorias_head, NoCategoria *categoria_a_remover)
{
  NoCategoria *anterior = NULL;
  NoCategoria *atual = *categorias_head;

  while (atual != NULL && atual != categoria_a_remover)
  {
    anterior = atual;
    atual = atual->proximo;
  }
  if (atual == NULL)
    return;
  if (anterior == NULL)
    *categorias_head = atual->proximo;
  else
    anterior->proximo = atual->proximo;

  NoAlimento *alim_atual = atual->alimentos_head;
  while (alim_atual != NULL)
  {
    NoAlimento *temp = alim_atual;
    alim_atual = alim_atual->proximo;
    free(temp);
  }
  liberarArvore(atual->arvore_energia_root);
  liberarArvore(atual->arvore_proteina_root);
  free(atual);

  printf("Categoria removida com sucesso.\n");
}

void removerAlimento(NoCategoria *categoria, int numero_alimento)
{
  NoAlimento *anterior = NULL;
  NoAlimento *atual = categoria->alimentos_head;

  while (atual != NULL && atual->dados.numero != numero_alimento)
  {
    anterior = atual;
    atual = atual->proximo;
  }

  if (atual == NULL)
  {
    printf("Alimento com numero %d nao encontrado.\n", numero_alimento);
    return;
  }

  if (anterior == NULL)
    categoria->alimentos_head = atual->proximo;
  else
    anterior->proximo = atual->proximo;

  free(atual);

  construirArvoresParaCategoria(categoria);
  printf("Alimento removido e indices atualizados.\n");
}

void liberarMemoria(NoCategoria *categorias_head)
{
  NoCategoria *cat_atual = categorias_head;
  while (cat_atual != NULL)
  {
    NoCategoria *cat_temp = cat_atual;
    cat_atual = cat_atual->proximo;
    NoAlimento *alim_atual = cat_temp->alimentos_head;
    while (alim_atual != NULL)
    {
      NoAlimento *alim_temp = alim_atual;
      alim_atual = alim_atual->proximo;
      free(alim_temp);
    }
    liberarArvore(cat_temp->arvore_energia_root);
    liberarArvore(cat_temp->arvore_proteina_root);
    free(cat_temp);
  }
}
