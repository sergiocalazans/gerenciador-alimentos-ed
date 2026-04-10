// Arquivo: P1.c
// Descrição: Programa principal da Etapa B. Lê um arquivo CSV de alimentos
//            e armazena seus dados em um arquivo binário (dados.bin).

#include "alimentos.h" // Inclui protótipos e definições

// Requisito 3.a: A função main do programa P1 fica no arquivo P1.c
int main()
{
  const char *ARQUIVO_CSV = "alimentos.csv";
  const char *ARQUIVO_BIN = "dados.bin";

  printf("Lendo dados de '%s' e escrevendo em formato binario para '%s'.\n", ARQUIVO_CSV, ARQUIVO_BIN);

  lerDadosDoCSVECriarBinario(ARQUIVO_CSV, ARQUIVO_BIN);

  printf("Processo concluido com sucesso!\n");

  // Requisito 10: Uma função deve ter, preferencialmente, apenas um comando return.
  return 0;
}