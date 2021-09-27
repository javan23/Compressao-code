#include "Lista.h"
#include <iostream>
#include <cstdlib>
#include <SPI.h>
#include <LoRa.h>
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define printok    0

int insere_elemento_lista_ordenado(lista *aux_lista, char *str,char len){
        
    no *aux_no_dado=(no*) malloc(sizeof(no)); // NO PARA O ELEMENTO DA LISTA

    if(aux_no_dado==NULL){
      Serial.println("FALHA NA ALOCAÇÃO INSERE ELEMENTO LISTA NO");
      return 0;
      }
  
    if(aux_lista->size_dic==256){ // LIMITA DICIONARIO À 256 ENTRADAS O LZ78 E 77 QUE PRECISAM TER UM NUMERO RESERVADO PARA CASOS ONDE NÃO EXISTE O DADO NO DICIONARIO
        Serial.println("LISTA CHEIA");
        free(aux_no_dado);
        return 1;
    }
    if(aux_lista->calda==NULL && aux_lista->head==NULL){

        aux_no_dado->next=NULL;
        aux_no_dado->previous=NULL;
        aux_no_dado->dado= (char*) malloc((len+1)*sizeof(char)); // O MAIS UM E PARA ADICIONAR O \0 PARA QUANDO FOR COMPARAR COM CMP
         if(aux_no_dado->dado==NULL){
            printf("FALHA NA ALOCACAO: aux_no_dado->dado");
            free(aux_no_dado);
            return 0;
         }
        strncpy(aux_no_dado->dado,str,len);
        aux_no_dado->dado[len]=0; //ADICIONA O FIM DE LINHA
        aux_no_dado->len = len+1;
        aux_no_dado->idx=0;
        aux_lista->size_dic=1;
        aux_lista->calda=aux_no_dado;
        aux_lista->head=aux_no_dado;
        return 1;
    }
    if((aux_lista->calda==NULL && aux_lista->head!=NULL)||(aux_lista->calda!=NULL && aux_lista->head==NULL)){
        printf("LISTA COM PROBLEMA, FAVOR CRIAR NOVAMENTE");
        free(aux_no_dado);
        return 0;
    }

    no *aux_no=aux_lista->head;

    no *aux_no2=aux_lista->calda;

    aux_no_dado->dado= (char*) malloc((len+1)*sizeof(char));
    if(aux_no_dado->dado==NULL){
      Serial.println("FALHA NA ALOCAÇÃO INSERE ELEMENTO LISTA NO NOME");
      free( aux_no_dado);
      return 0;
     }
    
    strncpy(aux_no_dado->dado,str,len);
    aux_no_dado->dado[len]=0;
    aux_no_dado->len = len+1;
    aux_no_dado->idx=aux_lista->size_dic;
    aux_lista->size_dic+=1;

    aux_no_dado->next=NULL;
    aux_no_dado->previous=aux_no2;
    aux_no2->next=aux_no_dado;
    aux_lista->calda=aux_no_dado;


    return 1;

}





void imprimi_lista(lista *aux){
no *aux_imprimi=aux->head;
    if(aux->head==NULL){
    printf("\n LISTA VAZIA \n");
    return;
    }

    while(aux_imprimi!=NULL){


        printf(" \n string : %s  idx : %d \n",aux_imprimi->dado,aux_imprimi->idx);
        aux_imprimi=aux_imprimi->next;
    }
    return;

}

int remove_termo(lista *aux_lista){
    char *aux;
    if(aux_lista==NULL){
        #if printok
        //Serial.println("LISTA VAZIA");
        #endif // printok
        return 0;
    }
    if(aux_lista->calda==NULL && aux_lista->head==NULL){
    
        return 0;
    }
    if(aux_lista->calda==aux_lista->head  && (aux_lista->head->next==NULL && aux_lista->head->previous==NULL)){
     
        aux=aux_lista->head->dado;
        free(aux); 
        free(aux_lista->calda);
        aux_lista->calda=NULL;
        aux_lista->head=NULL;
        aux_lista->size_dic--;
     
        #if printok
        
        #endif // printok
       
        return 1;
    }
    if((aux_lista->calda==NULL && aux_lista->head!=NULL)||(aux_lista->calda!=NULL && aux_lista->head==NULL)){
        Serial.println("LISTA COM PROBLEMA, FAVOR CRIAR NOVAMENTE");
        return 0;
    }
    //printf("CHEGUEI AQUi");
 
    no *aux_manipula=aux_lista->head;
    aux_manipula->next->previous=NULL;
   
    aux_lista->head=aux_manipula->next;
    aux=aux_manipula->dado;
    aux_lista->size_dic-=1;

    free(aux);
 
    free(aux_manipula);
    return 1;


}
void constroi_lista(lista *lista_ord){
    lista_ord->head=NULL;
    lista_ord->calda=NULL;
    lista_ord->size_dic=0;

}


// BUSCA CHAVE NO DICIONARIO E RETORNA 1 CASO ENCONTRE E ZERO QUANDO NAO ENCONTRA
int busca_dicionario(lista *dict,char *string,int *idx){

    no *aux_busca=dict->head;
    if(dict->head==NULL){
        *idx=-1000;
        return 0;
    }
    if(strcmp(aux_busca->dado,string)==0){
        *idx=0;
        return 1;
    }
    while(aux_busca!=NULL){

        if(strcmp(aux_busca->dado,string)==0){
            *idx=aux_busca->idx;

            return 1;
        }else{
            aux_busca=aux_busca->next;
        }
    }
  
    return 0;

}


char *busca_mensagem(lista *dict,int idx){
//   
    no *aux_busca=dict->head;

    if(dict->head==NULL){
        idx=-1000;
        return NULL;
    }
    while(aux_busca!=NULL){

        if(aux_busca->idx==idx){
            char *mensagem_aux=(char*) calloc(aux_busca->len,sizeof(char));
            sprintf(mensagem_aux,"%s",aux_busca->dado);
         
            return mensagem_aux;
           
        }else{
            aux_busca=aux_busca->next;
        }
    }

    return NULL;

}
void delete_list(lista *aux_lista){

while(remove_termo(aux_lista));
free(aux_lista);
}
