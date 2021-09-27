
 #define SIZE_TAB 13
short int input_bit( unsigned char *mensagem_comprimida,unsigned int tamanho );
void initialize_output_bitstream( void );
void output_bit(  int bit );
void flush_output_bitstream(  unsigned char *mensagem_comprimida, unsigned int *tamanho );
void initialize_input_bitstream( void );
long bit_ftell_output( );
long bit_ftell_input(  );
