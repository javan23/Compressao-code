#include "Compress.h"

extern void atualiza_heap();

int COMPRIME_LZ78(lista *dic_list, char *mensagem,int tamanho_mensagem, unsigned char *mensagem_compress,int *tamanho_comprimido){

char *ponteiro_busca=mensagem;

char *mensagem_aux=(char*) calloc(MAX_INPUTDIC,sizeof(char)); // STRING BUSCADA NO DICIONARIO

if(mensagem_aux==NULL)
    return 1;
atualiza_heap();

int count=1; // IDX DO DICIONÁRIO
int idx=0;
int value=0;
int aux_buffer=0;

while(ponteiro_busca< mensagem+tamanho_mensagem){ // ENQUANTO NAO CHEGOU AO FIM DA STRING CONTINUA COMPACTANDO

    memset(mensagem_aux,0,sizeof(char)*MAX_INPUTDIC); // LIMPA A VARIAVEL MENSAGEM
    
    strncpy(mensagem_aux,ponteiro_busca,count); // ATRIBUI O PRIMEIRO SIMBOLO A VARIAVEL
 
    value=255;
    atualiza_heap();
    while(busca_dicionario(dic_list,mensagem_aux,&idx)){  // BUSCA NO DICIONARIO ADICIONANDO OS CARACTERES
       count++;
       value=idx; // ARMAZENA O ULTIMO MAIOR IDX ENCONTRADO NO DICIONARIO
       
       if(ponteiro_busca+count>mensagem+tamanho_mensagem){ // QUER DIZER QUE HÁ COINCIDENCIA NO DICIONARIO ATÉ O FIM DA MENSAGEM
  
            mensagem_compress[aux_buffer+1]= 255; //MANDO O CARACTER 255 PORQUE NAO TERA NA MENSAGEM 255 E ELE NESSE CASO SERVE COMO UM \0 O QUE INDICA QUE É COMO SE O PROXIMO SIMBOLO
            // STRING A SER COMPACTADA FOSSE O /0 JA QUE SEMPRE MANDA O PROXIMO
            mensagem_compress[aux_buffer]= (char) value;  // EXCEDEU O TAMANHO DA MENSAGEM
            aux_buffer=aux_buffer+2;
            *tamanho_comprimido=aux_buffer;
            atualiza_heap();
            strncpy(mensagem_aux,ponteiro_busca,count); // NO LZW NAO PRECISA INSERIR ESSE ULTIMO ELEMENTO
          
            if(busca_dicionario(dic_list,mensagem_aux,&idx)==0)
                insere_elemento_lista_ordenado(dic_list,mensagem_aux,count);
            free(mensagem_aux);
            return 0;

            }

       strncpy(mensagem_aux,ponteiro_busca,count);
    }

    if(value==255){
        atualiza_heap();
        mensagem_compress[aux_buffer+1]= (char) ponteiro_busca[count-1]; // COLOCA O SÍMBOLO SUBSEQUENTE O SÍMBOLO
        mensagem_compress[aux_buffer]= 255;  // NÃO HÁ CORRESPONDENCIA NO DICIONARIO, 0 PARA EVITAR CONFLITO COM O IDX

    }
    else{
      
        atualiza_heap();
        mensagem_compress[aux_buffer+1]= (char) ponteiro_busca[count-1];
        mensagem_compress[aux_buffer]= (char) value;  // HÁ INDEX
    }


    aux_buffer=aux_buffer+2;
    *tamanho_comprimido=aux_buffer;

  

  if(busca_dicionario(dic_list,mensagem_aux,&idx)==0) // APENAS SE NAO FOR REPETIDA INSERE NO DICIONARIO
    insere_elemento_lista_ordenado(dic_list,mensagem_aux,count);

    ponteiro_busca=ponteiro_busca+count; // +COUNT PORQUE DIFERENTE DO LZW, O SÍMBOLO DIFERENTE É ENVIADO

    count=1;
    value=0;

}
atualiza_heap();

free(mensagem_aux);
return 0;
}

int DESCOMPRIME_LZ78(unsigned char *mensagem, char *mensagem_descompress,int tamanho_comprimido){

    //DICIONARIO INICIAL
    lista *dic_list= (lista *) malloc(sizeof(lista));
    if(dic_list==NULL)
      return 1;
    constroi_lista(dic_list);

    int idx=0;

    char *ante= (char*) calloc(MAX_INPUTDIC,sizeof(char));
    
    if(ante==NULL)
        {   
        printf("FALHA NA ALOCAÇÃO");
        return 1;
        }

    for(int i=0;i<tamanho_comprimido;i=i+2){


        char *aux=busca_mensagem(dic_list,(unsigned char) mensagem[i]);


        if(mensagem[i]==255){ // PRIMEIRA APARIÇÃO DA STRING


        sprintf(mensagem_descompress,"%s%c",mensagem_descompress,mensagem[i+1]);

        sprintf(ante,"%c",mensagem[i+1]);// PRECISA SER VETOR SE MANDASSE O MENSAGEM[i+1] DAVA ERRO
        insere_elemento_lista_ordenado(dic_list,ante,1);
        }
        else{
            if(mensagem[i+1]!=255) 
                sprintf(ante,"%s%c",aux,mensagem[i+1]);
            else
                sprintf(ante,"%s",aux); // CASO ESPECIAL ONDE NÃO HÁ SIMBOLOS SUBSEQUENTE

            if(busca_dicionario(dic_list,ante,&idx)==0)
                insere_elemento_lista_ordenado(dic_list,ante,strlen(ante));
            sprintf(mensagem_descompress,"%s%s",mensagem_descompress,ante);

            free(aux);

        }

        memset(ante,0,sizeof(char)*MAX_INPUTDIC);


    }

    delete_list(dic_list);
    free(ante);
return 0;
}
