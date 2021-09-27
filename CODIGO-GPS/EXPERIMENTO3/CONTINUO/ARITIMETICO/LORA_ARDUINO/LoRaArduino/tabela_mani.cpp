#define tipoS 66
#include "tabela_mani.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int tabela(char *v,unsigned char *t){
/*
0-0
1-1
2-2
3-3
4-4
5-5
6-6
7-7
8-8
9-9
10- - 45 = -3
11 -< 60 = 12
12 -> 62 = 14
13 -& 38 = -10
14 -. 46 = -1

*/
char *token;
token = strtok(v, "#");
v=token;
int aux2=0;
//printf("%s \n",v);
int size=strlen(v);
//printf("tamanho %d", size);
//printf("POSI 0 %d \n",t[0]);
for(int i=0;i<size;i++){
    //printf("\n int %d %c \n",v[i],v[i]);
    int aux= v[i]-48; //ASCII PARA BINARIO NORMAL
   // printf("\n %d \n",v[i]);
    if(aux<=9 && aux>=0){
        t[aux]+=1;
        if(t[aux]>254){
            printf("\n TAMANHO MAX %d %d\n",aux,t[aux]);
            return 1;
            }

        }
    else{
        if(aux==-3){
            t[10]+=1;
            if(t[10]>254){
            printf("\n TAMANHO MAX %d \n",t[10]);
            return 1;
            }

            }
        else if(aux==-4){
            t[11]+=1;
            if(t[11]>254){
            printf("\n TAMANHO MAX %d \n",t[11]);
            return 1;
            }
            aux2++;
            }
       
        else{

            t[12]+=1;
            if(t[12]>254){
            printf("\n TAMANHO ELSE MAX %d \n",t[12]);
            return 1;
            }

        }
    }
}
return 0;
}
void print_tab(unsigned char *v){

for(int i=0;i<10;i++)
    printf("\n %d %d \n",i,v[i]);
    printf("\n %c %d \n",'-',v[10]);
    printf("\n < %d \n",v[11],tipoS);
    printf("\n > %d \n",v[12]);
    printf("\n & %d \n",v[13]);
    printf("\n . %d \n",v[14]);

}
