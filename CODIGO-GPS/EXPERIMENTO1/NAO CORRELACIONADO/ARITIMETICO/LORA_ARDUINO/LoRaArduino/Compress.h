
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "coder.h"
#include "bitio.h"
#include "tabela_mani.h"
/*
 * Estrutura de dados que salva a função de densidade acumulada 
 */
typedef struct CDF{
          char c;
          unsigned short int low;
          unsigned short int high;
       } CDF;

extern int free_heap;
/*
 * Função que realiza a compressão utilizando a codificação aritmética
 * unsigned char *mensagem_comprimida : Mensagem comprimida
 * unsigned int *tamanho : Tamanho da mensagem comprimida
 * char *input : Mensagem a ser comprimida
 * return :
 * 0 : Compressão realizada
 * 1 : Falha na compressão
 */
int compress( unsigned char *mensagem_comprimida, unsigned int *tamanho ,char *input);
/*
 * Função que realiza a descompressão utilizando a codificação aritmética
 * unsigned char *mensagem_comprimida : Mensagem comprimida
 * unsigned int *tamanho : Tamanho da mensagem comprimida
 * char *descompress_mensagem : Mensagem descomprimida
 */
void expand( unsigned char *mensagem_comprimida,unsigned int tamanho,  char *descompress_mensagem );
/*
 * This routine is called to convert a character read in from
 * the text input stream to a low, high, range SYMBOL.  This is
 * part of the modeling function.  In this case, all that needs
 * to be done is to find the character in the probabilities table
 * and then retrieve the low and high values for that symbol.
 */
void convert_int_to_symbol( char c, SYMBOL *s, CDF *probabilities, int montante );
/*
 * This modeling function is called to convert a SYMBOL value
 * consisting of a low, high, and range value into a text character
 * that can be sent to a file.  It does this by finding the symbol
 * in the probability table that straddles the current range.
 */
char convert_symbol_to_int( unsigned int count, SYMBOL *s ,CDF *probabilities, int montante );
void error_exit( char *message );
