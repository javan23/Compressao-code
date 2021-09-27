#ifndef ARVORE_BIN_H_INCLUDED
#define ARVORE_BIN_H_INCLUDED
/*
 * Cria a estrutura de dado no
 */
typedef struct node{
    char frequecia;
    char caracter;
    //struct  node *p;
    struct node *left,*right;
}node;
/*
 * Inicializa  e cria um nó, considerando os parâmetros da estrutura.
 * Retorna: 
 * o endereço do nó criado caso tenha sucesso
 * null - falha na alocação
 */
node *cria_no(unsigned char freq, char caracter, node *p, node *left,node *right);

void inTree(node *top, int *count_);

/*
 * Busca o código binário em formato de string para o caracter buscado
 * node *raiz : raiz da árvore de huffman
 * char *binario : coloca o valor binário em formato de char
 * char aux : string buscada
 * int tamanho : tamanho em bytes do binario encontrado 
 * int *tamanho_bits : tamanho em bytes do binario acumulado
 * retorno: 
 * true:  encontrado o caracter
 * false: caracter não encontrado
 */
bool pega_binario(node *raiz,char *binario,int tamanho, char aux,int *tamanho_bits);
/*
 * Deleta a árvore criada
 */
void FreeHuffmanTree(node *n);
#endif // ARVORE_BIN_H_INCLUDED
