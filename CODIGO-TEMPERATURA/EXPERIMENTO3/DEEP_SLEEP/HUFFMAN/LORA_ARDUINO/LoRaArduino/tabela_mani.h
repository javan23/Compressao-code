

extern int free_heap;

/*
 * Função que cria a tabela de probabilidade
 * char *v : mensagem a ser comprimida
 * unsigned char *t : vetor que armazenará as probabilidade do símbolos
 * return : 
 * 0 : Não houve estouro
 * 1 : Houve estouro
 */
int tabela(char *v,unsigned char *t);
void print_tab(unsigned char *v);
