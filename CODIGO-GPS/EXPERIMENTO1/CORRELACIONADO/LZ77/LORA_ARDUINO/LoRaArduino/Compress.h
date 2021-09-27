#include <SPI.h>
#include <LoRa.h>
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LORA 
#undef WIFI 

#include <stdio.h>
#include <stdlib.h>
#define buffer_s  200
#define buffer_h  54



extern int free_heap;

/*
Função que realiza a compressão do LZ77:
char *mensagem : Mensagem a ser comprimda
unsigned char *mensagem_compress : Mensagem comprmida
int tam_men:  tamanho da mensagem original
int *tam_compactado : tamanho da mensagem comprimida


*/
void compress_( char *mensagem,unsigned char *mensagem_compress, int tam_men,int *tam_compactado);
/*
Função que realiza a descompressão do LZ77:
char *mensagem : Mensagem a ser descomprimda
unsigned char *codificada : Mensagem comprmida
int tam_compactado : tamanho da mensagem comprimida

retorno:

0 : descompressão concluída
3 : falha na descompressão
*/
int descompress_( char *mensagem_compress,int tam_compactado,unsigned char* codificada);
