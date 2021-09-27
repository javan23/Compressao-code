#include <stdio.h>
#include <stdlib.h>
#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "Arvore_bin.h"

/*
Estrutura de dado para os nós da lista
*/
typedef struct no{
node *dado;
struct no *next,*previous;
}no;


/*
Estrutura de dado para a lista
*/
typedef struct lista{
no *head;
no *calda;
void (*imprimi)(void *arg);
int (*compara_nos)(void *arg,void *arg1);
}lista;

void inicializa_lista(lista *lista_1 );
/*
Função que remove o primeiro elemento da lista.
Retorna o próprio nó removido
*/
node* remove_elemento(lista *aux_lista);
/*
Função que insere elementos na lista de forma ordenada.
return :

0 : Falha na inserção
1:  Sucesso
*/
int insere_elemento_lista_ordenado(lista *aux_lista, node *dado);
/*
Função que constroi a lista baseada na tabela de probabilidade
*/
void constroi_lista(unsigned char *tabela,lista *lista_ord);
/*
 * Constroi a árvora binária 
 */
void constroi_arvore(lista *lista_com);
#endif // LISTA_H_INCLUDED
