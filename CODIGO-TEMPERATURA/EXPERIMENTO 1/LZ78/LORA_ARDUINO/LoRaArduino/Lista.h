extern int free_heap;

#include <stdio.h>
#include <stdlib.h>

/*
Estrutura de dado para os nós da lista
*/
typedef struct no{
struct no *next,*previous;
char *dado;
char len;
unsigned char idx; // MUDAR SO PARA TESTAR O TAMANHO
//unsigned int idx; // MUDAR SO PARA TESTAR O TAMANHO
}no;


/*
Estrutura de dado para a lista
*/
typedef struct lista{
no *head;
no *calda;
int size_dic;
}lista;
/*
Função que remove o primeiro elemento da lista.
return : 
0 : lista vazia ou problema
1 : elemento removido
*/
int remove_termo(lista *aux_lista);
/*
Função que deleta a lista.
*/
void delete_list(lista *aux_lista);
/*
Função que insere elementos na lista de forma ordenada.
return :

1: elemento inserido ou lista cheia
0: falha na inserção
*/
int insere_elemento_lista_ordenado(lista *aux_lista, char *str,char len);
/*
Função que inicializa a lista.
*/
void constroi_lista(lista *lista_ord);

void imprimi_lista(lista *aux);

/*
Função que busca a string no dicionário.
int *idx : idx da string no dicionário.
return :
  0 : caso o valor não seja encontrado.
  1 : caso o valor seja encontrado
*/
int busca_dicionario(lista *dict,char *string,int *idx); //BUSCA A STRING NO DICIONARIO
/*
Função que busca o idx no dicionário e retorna a string correpondente.
Caso idx seja inválido, será retornado null.
*/
char *busca_mensagem(lista *dict,int idx);
