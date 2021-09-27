extern int free_heap;

// ELEMENTO DA LISTA
typedef struct no{
struct no *next,*previous;
char *dado;
char len;
unsigned char idx; // MUDAR SO PARA TESTAR O TAMANHO
//unsigned int idx; // MUDAR SO PARA TESTAR O TAMANHO se mudar pra int dai da pra ter um dicionario de 2 ^32
}no;


typedef struct lista{
no *head;
no *calda;
int size_dic;
}lista;

int remove_termo(lista *aux_lista);
void delete_list(lista *aux_lista);
int insere_elemento_lista_ordenado(lista *aux_lista, char *str,char len);
void constroi_lista(lista *lista_ord);

void imprimi_lista(lista *aux);

int busca_dicionario(lista *dict,char *string,int *idx); //BUSCA A STRING NO DICIONARIO

char *busca_mensagem(lista *dict,int idx);
