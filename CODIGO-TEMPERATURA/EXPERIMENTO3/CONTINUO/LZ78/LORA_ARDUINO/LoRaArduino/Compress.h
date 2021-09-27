#include <SPI.h>
#include <LoRa.h>
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LORA 
#undef WIFI 
#include "Lista.h"

#define DIC_SIZE 16 //TAMANHO MAXIMO DE SIMBOLO POR ITEM DO DICIONARIO INICIAL 15 +1 DO \0
#define MAX_INPUTDIC 100// TAMANHO MAXIMO DA STRING QUE SERA BUSCADA NO DICIONARIO

extern int free_heap;

/*
 * Função realiza a compressão utilizando o LZ78
 * lista *dic_list : Lista que contém o dicionário
 * char *mensagem : Mensagem a ser comprimida
 * int tamanho_mensagem : Tamanho da mensagem a ser comprimida
 * unsigned char *mensagem_compress : Mensagem comprimida
 * int *tamanho_comprimido : Tamanho da mensagem comprimida
 * return : 
 * 0 : Sucesso na compressão
 * 1 : Falha na compressão
 */
int COMPRIME_LZ78(lista *dic_list,char *mensagem,int tamanho_mensagem, unsigned char *mensagem_compress,int *tamanho_comprimido);

/*
 * unsigned char *mensagem : Mensagem comprimida
 * char *mensagem_descompress : Mensagem descomprimida
 * int tamanho_comprimido : Tamanho comprimido
 * return :
 *  0 : Sucesso
 *  1:  Falha
 */
int DESCOMPRIME_LZ78(unsigned char *mensagem, char *mensagem_descompress,int tamanho_comprimido);
