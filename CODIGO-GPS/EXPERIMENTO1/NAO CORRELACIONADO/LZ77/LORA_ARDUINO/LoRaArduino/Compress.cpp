#include "Compress.h"

extern void atualiza_heap();


void compress_( char *a,unsigned char *compact, int tam_men,int *tam_compactado){
int tam_compac=0;

 char *bf1=NULL,*bf2=NULL,*bh1=NULL,*bh2=NULL; //BUF BUFFER DE PESQUISA BFH BUFF DE BUSCA FUTURA
  atualiza_heap();
   bh1=a;
   char considera=0; // DETERMINA SE CONSIDERA OU NAO O ULTIMO BYTE.
   int qtd_bf=0,qtd_bh=0;
   if(tam_men>buffer_h){
        bh2=a+buffer_h-1;
        qtd_bh=buffer_h;}
    else{
        bh2=a+tam_men-1;
        qtd_bh=tam_men;}

   atualiza_heap();
    while(bh1<=bh2 && bh1<(a+tam_men)){

    if(bf1==NULL && bf2==NULL){

        compact[0]=255;
        compact[1]=255;
        compact[2]=*bh1;
       
        bf1=bh1;
        tam_compac+=3;
        bf2=bh1;
        bh1=bh1+1;
        bh2=bh2+1;
        qtd_bf++;
       atualiza_heap();
    }
    else
    {
        atualiza_heap();
        char  *aux_bf=bf2,*aux_bh=bh1;
        int  offset=0,max_offset=0;
        int match_max=0;
        while(aux_bf>=bf1){ // PROCURANDO NO BUFFER
        offset++;
        
            if(*aux_bh==*aux_bf){ // CASO ONDE JA HÁ A LETRA NO DICIONARIO
                int match=1;
             
                while(((aux_bf+match)<=bh2) && *(aux_bf+match)==*(aux_bh+match) && ((aux_bh+match)<=(bh2))){
              
              // PARA ATENDER A TERCEIRA CONDICAO COLOQUEI <=bh2 antes ele era <bh1
                 atualiza_heap();
                    match++;
                }
                if(match>match_max){ // SENAO FOR DESCONSIDERA
                //    printf("\n ATUALIZANDO MAX \n");
                    match_max=match;
                    
                    max_offset=offset;
                }


            }
                aux_bf=aux_bf-1;
        }


        if(match_max){ // ENTRA AQUI QUANDO ENCONTROU ALGUMA VARIAVEL
            
            if((bh1+match_max)<a+tam_men){ //BUFFAR H AINDA CONTEM ELEMENTO
                  
                    compact[tam_compac++]=max_offset; // SUBSTITUIU PELO SPRINTF PQ ELE SOBREPOEM COM /0
                    compact[tam_compac++]=match_max;
                    compact[tam_compac++]=*(bh1+match_max);
                   
                }
            else{ // ACABOU O BUFFER
                 
                    compact[tam_compac++]=max_offset;
                    compact[tam_compac++]=match_max;
                    compact[tam_compac++]=*(bh1+match_max-1); // VOU TER QUE ADICIONAR UM BIT A MAIS PARA INDICAR O TAMANHO
                    considera=1;
                    // POIS NAO TENHO COMO POR UM BYTE DE ENDFILE PARA ESTE MODO, DE MODO QUE O ULTIMO BYTE VAI DIZER SE CONSIDERAR OU NAO
                    // O ULTIMO BYTE


                }
            bh1=bh1+match_max+1; // MANIPULACAO DO BUFFER SEARCH
            bh2=bh1+buffer_h-1;
            if(bh2>=a+tam_men)
                    bh2=a+tam_men-1;
            int size=bf2-bf1;
            atualiza_heap();
            if((size+match_max+1)<buffer_s){ // BUFFER CODIFICADO AINDA NAO TA CHEIO
                                            // PODE PREENCHER COM UMA SEQUENCIA
                    bf1=bf1;
                    bf2=bf2+match_max+1;
                    qtd_bf=qtd_bf+match_max+1;

                }
            else{
                 //   printf("buffer cheio");
                    bf1=bf1+match_max+1;
                    bf2=bf2+match_max+1;
                }

            }
        else{
                atualiza_heap();
                if((bh1+match_max+1)<a+tam_men){ //BUFFAR H AINDA CONTEM ELEMENTO
               
                     compact[tam_compac++]=255;
                    compact[tam_compac++]=255;
                    compact[tam_compac++]=*(bh1+match_max); // SITUACAO ONDE NAO HA MATCH

                    
                    }
                else{ 
                    compact[tam_compac++]=255;
                    compact[tam_compac++]=255;
                    compact[tam_compac++]=*bh1;

                  
                    }
                bh1=bh1+match_max+1; // MANIPULACAO DO BUFFER SEARCH
                bh2=bh1+buffer_h-1;
                if(bh2>=a+tam_men) // PRA NAO ESTRAPOLAR ESSE BUFFER
                        bh2=a+tam_men-1;
                int size=bf2-bf1;
                atualiza_heap();
                if(size<buffer_s){ // BUFFER CODIFICADO AINDA NAO TA CHEIO
                        bf1=bf1;
                        bf2=bf2+1;
                        qtd_bf=qtd_bf+match_max+1;

                    }
                else{
                        bf1=bf1+1;
                        bf2=bf2+1;
                    }


            }

         }

        }
    compact[tam_compac++]=considera;
    atualiza_heap();
    *tam_compactado=tam_compac;

atualiza_heap();

}
int descompress_( char *decode,int tamanho_arq,unsigned char* codificada){ // DESCOMPRESS LZ77
int tamanho_decode=0;

char aux_;
if(codificada==NULL) {

    printf("\n MENSAGEM VAZIA %d.\n",0);
    return 3;
    
}else{
    aux_=codificada[tamanho_arq-1];
    tamanho_arq=tamanho_arq-1;


    char *pont_atual=NULL;
    int offset=0,match=0;
    char caracter=0;
    for(int i=0;i<tamanho_arq;i+=3){
        offset=codificada[i];
        match=codificada[i+1];
        caracter=codificada[i+2];

    
        if(offset==255){
            decode[tamanho_decode++]=caracter;

        }

        else{
            char *aux=decode+tamanho_decode-1; //(TAMANHO DECODE = POSICAO ATUAL DO VETOR)
            pont_atual=aux-(offset-1);
           // printf("\n ponteiro %c \n",*pont_atual);
            if(match==1){
               decode[tamanho_decode++]=*pont_atual;
           
                decode[tamanho_decode++]=caracter;
             
            }
            else{

                for(int j=0;j<match;j++){
                   
                    decode[tamanho_decode++]=*(pont_atual+j);
                
                    }
               
                decode[tamanho_decode++]=caracter;
             
            }
        }
      }
    }
    if(aux_){ 
        tamanho_decode=tamanho_decode-1; // BYTE QUE IGNORA O ULTIMO CARACTER
             }         
        decode[tamanho_decode]=0;
  
    return 0;

}
