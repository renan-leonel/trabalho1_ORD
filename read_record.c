#include <stdlib.h>

#include "main.h"
// Função bem similar à função input, porém na input era utilizado getchar para ler o caracter, como aqui vamos ler direto de um arquivo, usaremos fgetc
short leia_registro(FILE *arq, char *str, int size) {  // passa o arquivo do qual vai ser lido, a string em que o caracter vai ser copiado, e o tamanho da string
    short i = 0;                                       // i é a quantidade de caractere do registro, será o retorno da função
    char c = fgetc(arq);
    while ((c != EOF) && (c != '\n')) {  // enquanto não chegar no final do arquivo, e enquanto não encontrar um \n, pois precisamos ler registro por registro até o \n
        if (size > 0) {
            str[i] = c;
            i++;
            size--;
        }
        c = fgetc(arq);
    }
    str[i] = '\0';  // coloca o \0 no final do registro escrito
    return i;       // retorna a quantidade de caracteres do registro
}
