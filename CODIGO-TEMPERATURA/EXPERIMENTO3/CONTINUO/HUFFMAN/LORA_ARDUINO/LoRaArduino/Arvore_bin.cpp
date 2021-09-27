#include "arvore_bin.h"
#include <stdlib.h>

node *cria_no(unsigned char freq, char caracter, node *p, node *left,node *right){
node *aux_no_dado=(node*) malloc(sizeof(node));
if(aux_no_dado==NULL){
   // printf(" \n VETOR NAO ALOCADO value  %x \n",aux_no_dado->right);
    return aux_no_dado;
    }
    else
    {

   // printf(" \n VETOR  ALOCADO value direita value allocado %x %x %d \n",aux_no_dado->right,aux_no_dado,sizeof(node*));
    aux_no_dado->frequecia=freq;
    aux_no_dado->caracter=caracter;

    aux_no_dado->left=left;
    aux_no_dado->right=right;

    return aux_no_dado;

    }

}

void inTree(node *top,int *count){
 if (top != NULL){
    // printf("%d count\n",*count);
     *count=*count+1;
     inTree(top->left,count);
     for(int i=0;i<=*count;i++){
       //printf("\t ",*count);
     }
//     Serial.printf(" %c [%d] ",top->caracter,top->frequecia);
    // printf(" %d \n",top->frequecia);
     inTree(top->right,count);
     *count=*count-1;
 } else{
  //  printf(" \n NULL \n");
 }

}

bool pega_binario(node *raiz,char *binario,int tamanho, char aux,int *tamanho_bits){


    if (!(raiz->left || raiz->right) && raiz->caracter == aux)
    {


        *tamanho_bits=*tamanho_bits+tamanho;
        binario[tamanho] = '\0';
        return true;
    }
    else
    {
        bool encontrado = false;

        // Se existir um nó à esquerda
        if (raiz->left)
        {
            // Adicione '0' no bucket do buffer correspondente ao 'tamanho' nodeAtual
            binario[tamanho] = '0';

            // fazer recursão no nó esquerdo
            encontrado = pega_binario(raiz->left, binario, tamanho + 1,aux,tamanho_bits);
        }

        if (!encontrado && raiz->right)
        {
            binario[tamanho] = '1';
            encontrado = pega_binario(raiz->right, binario, tamanho + 1,aux,tamanho_bits);
        }
        if (!encontrado)
        {
            binario[tamanho] = '\0';

        }
        return encontrado;
    }



}
void FreeHuffmanTree(node *n)
{

    // Caso base da recursão, enquanto o nó não for NULL
     if(n != NULL){
        FreeHuffmanTree(n->left);
        FreeHuffmanTree(n->right);
        free(n);
    }

}
/*

*/
