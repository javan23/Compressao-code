#include "Compress.h"

extern void atualiza_heap();

int COMPRIME_LZW(lista *dic_list,char *mensagem,int tamanho_mensagem, unsigned char *mensagem_descompress,int *tamanho_comprimido){


char *ponteiro_busca=mensagem;

char *mensagem_aux=(char*) calloc(MAX_INPUTDIC,sizeof(char)); // STRING BUSCADA NO DICIONARIO 

if(mensagem_aux==NULL){
    Serial.println("FALHA NA ALOCAÇÃO COMPRIME_LZW");
    return 1;
    }

atualiza_heap();

int count=1;

int idx=0; // IDX DO DICIONÁRIO

int value=0;

int aux_buffer=0;

while(ponteiro_busca< mensagem+tamanho_mensagem){ // ENQUANTO NAO CHEGOU AO FIM DA STRING CONTINUA COMPACTANDO
  
    memset(mensagem_aux,0,sizeof(char)*MAX_INPUTDIC); // LIMPO A VARIAVEL MENSAGEM

    strncpy(mensagem_aux,ponteiro_busca,count); // ATRIBUI O PRIMEIRO SIMBOLO A VARIAVEL
 
    atualiza_heap();
    
    while(busca_dicionario(dic_list,mensagem_aux,&idx)){  // BUSCA NO DICIONARIO ADICIONANDO OS CARACTERES
       count++; // QUANTOS CARACTERES COINCIDIRAM
       value=idx; // ARMAZENA O ULTIMO MAIOR IDX ENCONTRADO NO DICIONARIO
       atualiza_heap();
       if(ponteiro_busca+count>mensagem+tamanho_mensagem){ // QUER DIZER QUE HÁ COINCIDENCIA NO DICIONARIO ATÉ O FIM DA MENSAGEM
            //sprintf(mensagem_descompress,"%s%d",mensagem_descompress,value); // O ÚLTIMO IDX SALVO JÁ SERIA O ÚLTIMO VALIDO PARA O MESMO
            mensagem_descompress[aux_buffer]= (char) value;  
            aux_buffer++;
            *tamanho_comprimido=aux_buffer;
            atualiza_heap();
            free(mensagem_aux);
            return 0;

            }

       strncpy(mensagem_aux,ponteiro_busca,count);
    }

    mensagem_descompress[aux_buffer]= ( unsigned char) value; // SALVA O INDEX COMPRIMIDO AQUI
    aux_buffer++;
    *tamanho_comprimido=aux_buffer;


    atualiza_heap();
    insere_elemento_lista_ordenado(dic_list,mensagem_aux,count);

    ponteiro_busca=ponteiro_busca+count-1; // -1 PORQUE A ULTIMA STRING ENCONTRADA SERIA A MENSAGEM_AUX -1 SIMBOLO

    count=1;
    value=0;

}
atualiza_heap();
free(mensagem_aux);
return 0;
}

int DESCOMPRIME_LZW(unsigned char *mensagem, char *mensagem_descompress,int tamanho_comprimido){
    
  
    //DICIONARIO INICIAL
    lista *dic_list=(lista *) malloc(sizeof(lista));
    if(dic_list==NULL){
      return 0;
    }
    constroi_lista(dic_list);
    char dic[DIC_SIZE]={0}; // DECLARA DICIONÁRIO 
    int idx=0;
   
    sprintf(dic,"%s","0123456789-.,"); // CARREGA DICIONARIO

    for(int i=0;i<DIC_SIZE-1;i++)
        insere_elemento_lista_ordenado(dic_list,dic+i,1); // INSERE DICIONÁRIO INICIAL NA LISTA

    char *ante= (char*) calloc(MAX_INPUTDIC,sizeof(char));
    char *atual= (char*) calloc(MAX_INPUTDIC,sizeof(char));
    char *aux =(char*) busca_mensagem(dic_list,(char) mensagem[0]);
    if(ante==NULL){
      free(dic_list);
      return 0;
       }
    if(atual==NULL){
      free(dic_list);
      return 0;
       }
    if(aux==NULL)
      {
      free(dic_list);
      return 0;
       }
    
    sprintf(ante,"%s",aux);

    sprintf(mensagem_descompress,"%s",ante);

    free(aux);

    for(int i=1;i<tamanho_comprimido;i++){


        char *aux=busca_mensagem(dic_list,(unsigned char) mensagem[i]);
        if(aux==NULL){
            sprintf(ante,"%s%c",ante,ante[0]); // CASO ESPECIAL DO LZW (IDX A DESCOMPRIMIR NÃO CONSTA NO DIC CONSTRUIDO)
            insere_elemento_lista_ordenado(dic_list,ante,strlen(ante));
            sprintf(mensagem_descompress,"%s%s",mensagem_descompress,ante);
            memset(atual,0,sizeof(char)*MAX_INPUTDIC);
            continue;
        }
        else{//printf("\n CONCAT ANTERIOR %s %s ATUAL \n",ante,aux);
        sprintf(atual,"%s%c",ante,aux[0]);
        if(busca_dicionario(dic_list,atual,&idx)==0)
            insere_elemento_lista_ordenado(dic_list,atual,strlen(atual)); // SE ATUAL + ANTERIOR NAO EXISTIR NO DICIONARIO , ADICIONA
        memset(atual,0,sizeof(char)*MAX_INPUTDIC);
        memset(ante,0,sizeof(char)*MAX_INPUTDIC);

        sprintf(ante,"%s",aux);
        sprintf(mensagem_descompress,"%s%s",mensagem_descompress,aux);
        free(aux);
        }
    }
   // imprimi_lista(dic_list);

free(ante);

free(atual);

delete_list(dic_list);

return 1;
}
