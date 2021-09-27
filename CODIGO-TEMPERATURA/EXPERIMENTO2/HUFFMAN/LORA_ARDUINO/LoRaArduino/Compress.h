
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAB_OFFSET 16 // Tamanho da tabela
#include "Arvore_bin.h"
#include "tabela_mani.h"
#include "Lista.h"

/*
 * Comprime utilizando a compressão de Huffman
 * char *string_descom : mensagem a ser comprimida
 * unsigned int *tamanho_dado_compactado : tamanho do dado a ser comprimido
 * unsigned char *bits : bits a serem considerados no último byte
 * unsigned char *aux_cod : variável auxiliar utilizada para a necessidade de mais de uma compressão
 * unsigned char *mensagem_code : mensagem codificada
 * node *arvore : Árvore de huffman
 */
void compress_dado(char *string_descom,unsigned int *tamanho_dado_compactado,
unsigned char *bits, unsigned char *aux_cod,unsigned char *mensagem_code,  node *arvore);


/* Descomprime utilizando a compressão de Huffman
 *  char **mensagem_decode : mensagem a ser decodificada
 *  unsigned char *mensagem_comprimida : mensagem comprimida
 *  int tamanho_arq : tamanho da mensagem comprimida
 *  int qtd_msn : define o tamanho para a destring descomprimida
 *  return:
 *  
 *  2 : falha
 *  1 : sucesso na descompressão
 */
int descompacta(char **mensagem_decode,unsigned char *mensagem_comprimida , int tamanho_arq,int qtd_msn);
