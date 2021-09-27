#include "Compress.h"

extern void atualiza_heap();

/*
 * Função utilizada para atualizar e carregar a tabela de probabilidade de modo dinâmico.
 * unsigned char *tabela: Tabela de probabilidade ( Quantidade de vezes que o símbolo apareceu)
 * CDF *probabilities : Vetor de CDF 
 * unsigned int *TAM  : Montante de caracteres da mensagem, utilizada para realizar a razão.
 */
void atualiza_cdf(unsigned char *tabela, CDF *probabilities, unsigned int *TAM){

 char vector_string[]={"0123456789-.,\0"};
 int tam=0;

 for(int i=0;i<SIZE_TAB+1;i++){

    if(i==0){
        (probabilities+i)->c=vector_string[i];
        (probabilities+i)->low=0;
        (probabilities+i)->high=tabela[0];
        tam=tam+tabela[0];
    }
    else{
        if(tabela[i]==0)
            tabela[i]=1;
        (probabilities+i)->c=vector_string[i];
        (probabilities+i)->low=(probabilities+i-1)->high;

        (probabilities+i)->high=(probabilities+i)->low+tabela[i];
        tam=tam+tabela[i];


    }

 }
*TAM=tam; // É NECESSÁRIO PARA O SCALE

 }
 int compress(unsigned char *mensagem_comprimida, unsigned int *tamanho,char *input)
{
    int i;
    char c;
    SYMBOL s;
    atualiza_heap();
    //
    CDF prob2[SIZE_TAB+1]; // VETOR DE CDF 
    unsigned int tamanho_aux=0;
    unsigned char tabela_probabilidade[SIZE_TAB+1]={1}; // ATRIBUI A TODOS O VALOR UM PARA GARANTIR A DECODIFICABILIDADE

    if(tabela(input,tabela_probabilidade)) // CALCULA A TABELA DE PROBABILIDADE
        return 1;                         
    for(int i=0;i<15;i++) //
        mensagem_comprimida[i]=tabela_probabilidade[i];
    atualiza_cdf(tabela_probabilidade,prob2,&tamanho_aux);

    //static char *input = "<8>>";
    
    atualiza_heap();
    //puts( "Compressing..." );
    initialize_output_bitstream();
    initialize_arithmetic_encoder();
    for ( i=0 ; ; )
    {
        c = input[ i++ ];
        convert_int_to_symbol( c, &s, prob2,tamanho_aux);
        encode_symbol(  &s );
        if ( c == '\0' )
            break;
            atualiza_heap();
    }
    flush_arithmetic_encoder( );
    atualiza_heap();
    flush_output_bitstream(  mensagem_comprimida,tamanho);
    atualiza_heap();
   return 0;
}
void expand(unsigned char *mensagem_comprimida,unsigned int tamanho, char *descomprimida )
{
  
    SYMBOL s;
    char c;
    int count;

    //--------------------- CRIANDO A TABELA -------------------
    CDF probe_decode[SIZE_TAB+1]; // VETOR DE CDF 
    
   unsigned  int tamanho_aux=0;
    unsigned char tabela_probabilidade_decode[SIZE_TAB+1]={1};
    for(int i=0;i<SIZE_TAB;i++)
        tabela_probabilidade_decode[i]=mensagem_comprimida[i];
    atualiza_cdf(tabela_probabilidade_decode,probe_decode,&tamanho_aux); //CRIA O MODELO DE STRUC ADAPTATIVO IGUAL O OUTRO
 
    initialize_input_bitstream();
    initialize_arithmetic_decoder( mensagem_comprimida,tamanho );
    int i=0;
    for ( ; ; )
    {
        s.scale = tamanho_aux;
        count = get_current_count( &s );
        c = convert_symbol_to_int( count, &s,probe_decode,tamanho_aux );
        if ( c == '\0' )
            break;
        remove_symbol_from_stream(  &s, mensagem_comprimida,tamanho );
       descomprimida[i++]=c;
    }

}

/*
 * This routine is called to convert a character read in from
 * the text input stream to a low, high, range SYMBOL.  This is
 * part of the modeling function.  In this case, all that needs
 * to be done is to find the character in the probabilities table
 * and then retrieve the low and high values for that symbol.
 */
void convert_int_to_symbol( char c, SYMBOL *s, CDF *probabilities, int montante )
{
    int i;

    i=0;
    for ( ; ; )
    {
        if ( c == probabilities[ i ].c )
        {

            s->low_count = probabilities[ i ].low;
            s->high_count = probabilities[ i ].high;
            s->scale = montante;
            return;
        }
        if ( probabilities[i].c == '\0' )
            error_exit( "Trying to encode a char not in the table" );
        i++;
    }
}

/*
 * This modeling function is called to convert a SYMBOL value
 * consisting of a low, high, and range value into a text character
 * that can be sent to a file.  It does this by finding the symbol
 * in the probability table that straddles the current range.
 */
char convert_symbol_to_int( unsigned int count, SYMBOL *s, CDF *probabilities, int montante  )
{
    int i;
 //   printf("MONTANTE -> %d \n",montante);
    i = 0;
    for ( ; ; )
    {
        if ( count >= probabilities[ i ].low &&
             count < probabilities[ i ].high )
        {
            s->low_count = probabilities[ i ].low;
            s->high_count = probabilities[ i ].high;
            s->scale = montante;
            return( probabilities[ i ].c );
        }
        if ( probabilities[ i ].c == '\0' )
            error_exit( "Failure to decode character" );
        i++;
    }
}
void error_exit( char *message )
{
    puts( message );
    exit( -1 );
}
