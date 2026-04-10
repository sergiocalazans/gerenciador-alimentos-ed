// Arquivo: P2.c (VERSÃO CORRIGIDA E ROBUSTA)
// Descrição: Programa principal da Etapa C. Carrega dados de alimentos
//            de um arquivo binário, os organiza em listas e árvores,
//            e fornece uma interface de menu para o usuário.

#include "alimentos.h"

// Exibe o menu principal de opções para o usuário.
void exibirMenu()
{
  printf("\n========= MENU DE GERENCIAMENTO DE ALIMENTOS =========\n");
  printf("1. Listar todas as categorias\n");
  printf("2. Listar alimentos de uma categoria (ordem alfabetica)\n");
  printf("3. Listar alimentos de uma categoria por Energia (decrescente)\n");
  printf("4. Listar alimentos de uma categoria por Proteina (decrescente)\n");
  printf("5. Listar alimentos de uma categoria por faixa de Energia\n");
  printf("6. Listar alimentos de uma categoria por faixa de Proteina\n");
  printf("7. Remover uma categoria\n");
  printf("8. Remover um alimento\n");
  printf("9. Sair e Salvar Alteracoes\n");
  printf("======================================================\n");
  printf("Escolha uma opcao: ");
}

// FUNÇÃO ROBUSTA PARA LER UMA OPÇÃO NUMÉRICA DO USUÁRIO
// Garante que o programa não entre em loop infinito com entradas inválidas.
int obterOpcaoDoUsuario()
{
  char buffer[100];
  int opcao = 0;

  // Lê a linha inteira para o buffer de texto.
  if (fgets(buffer, sizeof(buffer), stdin))
  {
    // Tenta converter o texto lido para um inteiro.
    // Se sscanf retornar 1, a conversão foi bem-sucedida.
    if (sscanf(buffer, "%d", &opcao) != 1)
    {
      opcao = -1; // Retorna um valor inválido se a entrada não for um número
    }
  }
  return opcao;
}

// Requisito 3.b: A função main do programa P2 fica no arquivo P2.c
int main()
{
  const char *ARQUIVO_BIN = "dados.bin";
  NoCategoria *lista_categorias = carregarDadosDeBinario(ARQUIVO_BIN);
  printf("Dados do arquivo '%s' carregados com sucesso.\n", ARQUIVO_BIN);

  int opcao = 0;
  int alteracoes_feitas = 0; // Flag para salvar apenas se houver mudanças

  // Requisito C.c: Loop de menu
  // Requisito 9: O controle do loop é feito pela variável 'opcao', sem break/continue.
  while (opcao != 9)
  {
    exibirMenu();
    opcao = obterOpcaoDoUsuario();

    // Variáveis que podem ser usadas dentro do switch
    NoCategoria *categoria_selecionada = NULL;
    float min, max;
    int num_alimento;

    switch (opcao)
    {
    case 1:
      listarTodasCategorias(lista_categorias);
      break;

    case 2:
      categoria_selecionada = selecionarCategoria(lista_categorias);
      if (categoria_selecionada != NULL)
      {
        listarAlimentosDeCategoria(categoria_selecionada);
      }
      break;

    case 3: // Listar por Energia
    case 4: // Listar por Proteína
      categoria_selecionada = selecionarCategoria(lista_categorias);
      if (categoria_selecionada != NULL)
      {
        const char *criterio = (opcao == 3) ? "Energia" : "Proteina";
        printf("\n--- Alimentos em '%s' (Ordem Decrescente de %s) ---\n", categoria_selecionada->nome, criterio);
        printf("| %-4s | %-55s | %-12s | %-12s |\n", "Num", "Descricao", "Energia(Kcal)", "Proteina(g)");
        printf("|------|---------------------------------------------------------|--------------|--------------|\n");

        if (opcao == 3)
        {
          listarAlimentosPorOrdemDecrescente(categoria_selecionada->arvore_energia_root);
        }
        else
        {
          listarAlimentosPorOrdemDecrescente(categoria_selecionada->arvore_proteina_root);
        }
        printf("+--------------------------------------------------------------------------------------------+\n");
      }
      break;

    case 5: // Faixa de Energia
    case 6: // Faixa de Proteína
      categoria_selecionada = selecionarCategoria(lista_categorias);
      if (categoria_selecionada != NULL)
      {
        printf("Digite o valor minimo: ");
        min = obterOpcaoDoUsuario(); // Reutiliza a função de leitura segura
        printf("Digite o valor maximo: ");
        max = obterOpcaoDoUsuario();

        const char *criterio = (opcao == 5) ? "Energia" : "Proteina";
        printf("\n--- Alimentos em '%s' (Faixa de %s: %.0f a %.0f) ---\n", categoria_selecionada->nome, criterio, min, max);
        printf("| %-4s | %-55s | %-12s | %-12s |\n", "Num", "Descricao", "Energia(Kcal)", "Proteina(g)");
        printf("|------|---------------------------------------------------------|--------------|--------------|\n");

        if (opcao == 5)
        {
          listarAlimentosPorFaixa(categoria_selecionada->arvore_energia_root, min, max);
        }
        else
        {
          listarAlimentosPorFaixa(categoria_selecionada->arvore_proteina_root, min, max);
        }
        printf("+--------------------------------------------------------------------------------------------+\n");
      }
      break;

    case 7:
      printf("Selecione a categoria para REMOVER:\n");
      categoria_selecionada = selecionarCategoria(lista_categorias);
      if (categoria_selecionada != NULL)
      {
        removerCategoria(&lista_categorias, categoria_selecionada);
        alteracoes_feitas = 1;
      }
      break;

    case 8:
      categoria_selecionada = selecionarCategoria(lista_categorias);
      if (categoria_selecionada != NULL)
      {
        listarAlimentosDeCategoria(categoria_selecionada);
        printf("Digite o numero do alimento a ser removido: ");
        num_alimento = obterOpcaoDoUsuario();
        removerAlimento(categoria_selecionada, num_alimento);
        alteracoes_feitas = 1;
      }
      break;

    case 9:
      printf("Encerrando o programa...\n");
      break;

    default:
      printf("\nOpcao invalida! Por favor, digite um numero entre 1 e 9.\n");
      break;
    }
  }

  // C.c.9: Gera uma versão atualizada do arquivo se algo foi removido
  if (alteracoes_feitas)
  {
    salvarDadosEmBinario(lista_categorias, ARQUIVO_BIN);
    printf("Alteracoes foram salvas no arquivo '%s'.\n", ARQUIVO_BIN);
  }

  // Requisito 5: Libera toda a memória alocada
  liberarMemoria(lista_categorias);
  printf("Memoria liberada com sucesso. Programa encerrado.\n");

  return 0;
}
