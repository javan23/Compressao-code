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

int COMPRIME_LZW(lista *dic_list,char *mensagem,int tamanho_mensagem, unsigned char *mensagem_descompress,int *tamanho_comprimido);
int DESCOMPRIME_LZW(unsigned char *mensagem, char *mensagem_descompress,int tamanho_comprimido);