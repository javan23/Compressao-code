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
#define MAX_INPUTDIC 100// TAMANHO MAXIMO DA STRING QUE SERA BUSCADA NO DICIONARIO ( CONSIDERA QUE O TAMANHO MÁXIMO DA STRING DO DICIONÁRIO SERÁ 100)

extern int free_heap; 

/*
Função utilizada para realizar a compressão da mensagem. Sendo
lista *dic_list : dicionário inicializado
char *mensagem : variável contendo a mensagem a ser comprimida
unsigned char *mensagem_descompress : variável contendo a comprimida
int tamanho_mensagem :  tamanho total da mensagem a ser comprimida
int tamanho_comprimido :  tamanho total da mensagem  comprimida
return : 

1 : Falha na compressão
0 : Compressão concluída
*/

int COMPRIME_LZW(lista *dic_list,char *mensagem,int tamanho_mensagem, unsigned char *mensagem_descompress,int *tamanho_comprimido);

/*
Função utilizada para realizar a compressão da mensagem. Sendo
char *mensagem : variável contendo a mensagem comprimida
int tamanho_mensagem :  tamanho total da mensagem a ser comprimida
int tamanho_comprimido :  tamanho total da mensagem  comprimida
return : 

1: Compressão concluída
0: Falha na compressão
*/
int DESCOMPRIME_LZW(unsigned char *mensagem, char *mensagem_descompress,int tamanho_comprimido);
