 #include "Lista.h"
#define TAB_OFFSET 14
 #define printok    0
int insere_elemento_lista_ordenado(lista *aux_lista, node *dado){

    no *aux_no_dado=(no*) malloc(sizeof(no));

    if(aux_lista->calda==NULL && aux_lista->head==NULL){
        #if printok
        printf("ELEMENTO DA CABEÇA");
        #endif // printok
        aux_no_dado->next=NULL;
        aux_no_dado->previous=NULL;
        aux_no_dado->dado=dado;
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
    no *aux_no2; // auxilia na manipulacao
    while(aux_no != NULL){
    //printf("ENTREI AQUI");
    if(dado->frequecia<=aux_no->dado->frequecia)
    {

        if(aux_no==aux_lista->head && aux_no!=aux_lista->calda ){
            #if printok
            printf("\n -----------CABECA +1 ------------\n");
            #endif
            aux_no2=aux_lista->head;
            aux_lista->head=aux_no_dado;
            aux_no_dado->dado=dado;
            aux_no_dado->previous=NULL;
            aux_no_dado->next=aux_no2;

            aux_no2->previous=aux_no_dado;
            aux_no=NULL;
            #if printok
            printf("\n CABEÇA %x CALDA %x NOVA ELEMENTO %x\n",aux_lista->head,aux_lista->calda,aux_no_dado);
            #endif // printok
             no *aux=aux_lista->head;
            #if printok
            printf("\n");
            while(aux!=NULL){
            aux=aux->next;
            }
            #endif // printok
            return 1;


        }
        else if(aux_no==aux_lista->head && aux_no==aux_lista->calda ){


            aux_no2=aux_lista->head;
            aux_lista->head=aux_no_dado;
            aux_no_dado->dado=dado;
            aux_no_dado->previous=NULL;

            aux_no_dado->next=aux_no2;
            aux_lista->calda->previous=aux_no_dado;
            //aux_no2->previous=aux_no_dado;
            #if printok
            printf("\n CABECA  +0 \n");
            #endif
          
            aux_no=NULL;
            #if printok
            printf("\n CABEÇA %x CALDA %x NOVA ELEMENTO %x\n",aux_lista->head,aux_lista->calda,aux_no_dado);
            #endif // printok
            no *aux=aux_lista->head;
            #if printok
            printf("\n");
            while(aux!=NULL){
            printf(" |%d|(%c)  ",aux->dado->frequecia,aux->dado->caracter);
            
            aux=aux->next;
            }
            #endif
            return 1;
        }


        else{
            #if printok
            printf("\n ADICIONANDO NO MEIO %d \n ",dado->frequecia);
            #endif // printok
         
            aux_no_dado->dado=dado;
            aux_no_dado->next=aux_no;
            aux_no_dado->previous=aux_no->previous;
           
            aux_no->previous->next=aux_no_dado;
          
            aux_no->previous=aux_no_dado;

            
            aux_no=NULL;
            no *aux=aux_lista->head;
            #if printok
            printf("\n");
            while(aux!=NULL){
            printf(" |%d|(%c)  ",aux->dado->frequecia,aux->dado->caracter);
            
            aux=aux->next;
            }
            #endif // printok
            return 1;
        }

    }
    aux_no=aux_no->next;

    }
    aux_no2=aux_lista->calda;
    #if printok
    printf("\n ADICIONANDO NO FIM %d\n", dado->frequecia);
    #endif // printok

    aux_no_dado->dado=dado;
    aux_no_dado->next=NULL;
    aux_no_dado->previous=aux_no2;
    aux_no2->next=aux_no_dado;
    aux_lista->calda=aux_no_dado;
    no *aux=aux_lista->head;
    #if printok
    printf("\n");
    while(aux!=NULL){
    aux=aux->next;
            }
    #endif // printok

    return 1;

}


void inicializa_lista(lista *lista_1 ){
lista_1->head=NULL;
lista_1->calda=NULL;

}


void imprimi_lista(lista *aux){
no *aux_imprimi=aux->head;
    if(aux->head==NULL){
    printf("\n LISTA VAZIA \n");
    return;
    }

    while(aux_imprimi!=NULL){


        aux->imprimi(aux_imprimi->dado);
        aux_imprimi=aux_imprimi->next;
    }
    return;

}

node* remove_elemento_lista(lista *aux_lista){
//no *aux_no=(no*) malloc(sizeof(no*));
    node *aux;
    if(aux_lista==NULL){
    #if printok
        printf("LISTA VAZIA");
        #endif // printok
        return NULL;
    }
    if(aux_lista->calda==NULL && aux_lista->head==NULL){
        printf(" \n LISTA VAZIA \n" );
        return NULL;
    }
    if(aux_lista->calda==aux_lista->head  && (aux_lista->head->next==NULL && aux_lista->head->previous==NULL)){

        aux=aux_lista->head->dado;
        free(aux_lista->calda);
        aux_lista->calda=NULL;
        aux_lista->head=NULL;
        free(aux_lista->head); // APAGO LA NO MAIN
        #if printok
        printf(" \n APAGANDO ULTIMO ELEMENTO LISTA VAZIA \n" );
        #endif // printok
        return aux;
    }
    if((aux_lista->calda==NULL && aux_lista->head!=NULL)||(aux_lista->calda!=NULL && aux_lista->head==NULL)){
        printf("LISTA COM PROBLEMA, FAVOR CRIAR NOVAMENTE");
        return NULL;
    }
    //printf("CHEGUEI AQUi");
    no *aux_manipula=aux_lista->head;
    aux_manipula->next->previous=NULL;
    aux_lista->head=aux_manipula->next;
    aux=aux_manipula->dado;
    free(aux_manipula);
    return aux;


}
void constroi_lista(unsigned char *t,lista *lista_ord){
lista_ord->head=NULL;
lista_ord->calda=NULL;

for(int i=0;i<TAB_OFFSET-1;i++){

     // insere na lista apenas se for um elemento válido, ou seja, haja bytes
      if(t[i]){
            if(i<10){

                insere_elemento_lista_ordenado(lista_ord,cria_no(t[i],i+48,NULL,NULL,NULL));
            }
            else if(i==10){
                insere_elemento_lista_ordenado(lista_ord,cria_no(t[i],45,NULL,NULL,NULL));

            }
            else if(i==11){
                     //  printf("ENTREIIIIIIIIIIIIIIIIIIII %d",i);
                    insere_elemento_lista_ordenado(lista_ord,cria_no(t[i],44,NULL,NULL,NULL));

                }
                else{

                    insere_elemento_lista_ordenado(lista_ord,cria_no(t[i],46,NULL,NULL,NULL));
                }
            }
    }


}
void constroi_arvore(lista *ordena){

    while(ordena->head!=ordena->calda){
        node *esquerda= remove_elemento_lista(ordena);
        node *direita= remove_elemento_lista(ordena);
     
        insere_elemento_lista_ordenado(ordena,cria_no(esquerda->frequecia+direita->frequecia,'*',NULL,esquerda,direita));

    }

}
