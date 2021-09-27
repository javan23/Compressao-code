#include "Compress.h"
#define TAB_OFFSET 16
void compress_dado(char *string_descom,unsigned int *tamanho_dado_compactado,
unsigned char *bits, unsigned char *aux_cod,unsigned char *mensagem_code, node *arvore)
{

   int soma=0;
   int i=0;
   char b[50]={0};
   while(string_descom[i]){
            int tamanho=0;

            pega_binario(arvore,b,tamanho,string_descom[i],&soma);
          
            i++;
            for(char *c=b;*c;c++){
                    if(*c=='1'){

                        *aux_cod= *aux_cod | (1 << *bits % 8); // MSB
                    }

                    *bits=*bits-1;// VOU DECREMENTANDO QUANDO CHEGA A NEGATIVO VAI PRA -1
                   
                    if(*bits==255){
                      
                        *bits=7;
                        mensagem_code[*tamanho_dado_compactado+TAB_OFFSET]=*aux_cod;
                        *tamanho_dado_compactado=*tamanho_dado_compactado+1;
                        *aux_cod=0;

                    }

            }


        }

}
int descompacta(char **mensagem_decode,unsigned char *mensagem_comprimida , int tamanho_arq,int qtd_msn){
    char tabela_compac[TAB_OFFSET-1]={0}; // TABELA


         for(int i =0;i<TAB_OFFSET-1;i++){ // SALVANDO TABELA
            tabela_compac[i]=mensagem_comprimida[i];

         }
            ////////////// CONSTROI LISTA
        lista ordena_comp;
        ordena_comp.head=NULL;
        ordena_comp.calda=NULL;

        for(int i=0;i<TAB_OFFSET-1;i++){
         // insere na lista apenas se for um elemento válido, ou seja, haja bytes
        if(tabela_compac[i]){

                if(i<10){

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],i+48,NULL,NULL,NULL));
                }
                else if(i==10){

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],45,NULL,NULL,NULL));

                }
                else if(i==11){

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],60,NULL,NULL,NULL));

                }
                else if(i==12){

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],62,NULL,NULL,NULL));

                }
                else if(i==13){

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],38,NULL,NULL,NULL));

                }
                else{

                    insere_elemento_lista_ordenado(&ordena_comp,cria_no(tabela_compac[i],46,NULL,NULL,NULL));
                }
            }
        }

        // LISTA ORDENADA DA TABELA

        no *aux=ordena_comp.head;
        constroi_arvore(&ordena_comp);

        unsigned int tamanho_bits_comp= tamanho_arq*8;


        if(mensagem_comprimida[TAB_OFFSET-1]!=0) // POSICAO QUE CONTEM O TAMANHO DE BITS USADOS NO ULTIMO BYTE
            tamanho_bits_comp=tamanho_bits_comp-(8-mensagem_comprimida[TAB_OFFSET-1]);
       

        int count_=0;
        unsigned char bit_atual=7;
        char  aux_char=0,aux_2=0;
        int pos=TAB_OFFSET*8;
        char *mensagem_descom= (char*) calloc(35*qtd_msn,sizeof(char));
        if(mensagem_descom == NULL) {
        return 2;
        }

        while(pos<tamanho_bits_comp){
            node *atual= ordena_comp.head->dado; // RAIZ DA ARVORE
            while(atual->left || atual->right)
                {

                    if(pos%8==0){
               
                        aux_char=mensagem_comprimida[pos/8]; // ATUALIZA TODA VEZ QUE COMPLETA UM BYTE
                   
                       }
                   
                    aux_2= aux_char & (1<< bit_atual); // AND DO BIT ATUAL
                  
                    bit_atual--;
                    if(bit_atual==255)
                        bit_atual=7; // COMECA O NOVO BYTE

                    if(aux_2){
                        atual=atual->right;
                     
                        }
                    else{

                        atual=atual->left;
                
                        }
                    pos++;
                }
     

  
            mensagem_descom[count_]=atual->caracter;
            count_++;


        }

        mensagem_descom[count_]='\0';

        FreeHuffmanTree(ordena_comp.head->dado); // APAGA ARVORE OS NOS DA ARVORE
        free(ordena_comp.head); // APAGA ESSE ELEMENTO
        
        *mensagem_decode=mensagem_descom;
        
    return 1;
}
