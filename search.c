#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

//A função busca fará a leitura da chave dos registros, retornará 0 se não encontrar o registro, 1 se encontrar
int busca(int chave, int *posicao_de_seek, short *tam_reg, int isRemove) {
    int achou = 0;
    int key;  //será utilizada na conversão da aux para inteiro
    char aux[TAM_MAX_REG];
    char *keyOfBuffer;
    char buffer[TAM_MAX_REG];
    int posicao_de_leitura;

    if (!isRemove) printf("\n\nBusca pelo registro de chave '%d'\n", chave);

    fseek(arquivo_dat, 4, SEEK_SET);  // movendo o ponteiro de L/E em 4 bytes a partir do começo do arquivo p/ pular a LED (não queremos ler ela)
    while ((achou == 0) && (!feof(arquivo_dat))) {
        posicao_de_leitura = ftell(arquivo_dat);             //mostra em qual byte (do arquivo) está sendo realizada a leitura
        fread(tam_reg, sizeof(short), 1, arquivo_dat);       // le o tamanho do registro de dentro do arquivo e armazena o valor na tam_reg
        fread(buffer, sizeof(char), *tam_reg, arquivo_dat);  // le do arquivo arq "tamanho" numero de elementos, e armazena na string buffer
        buffer[*tam_reg] = '\0';

        if (buffer[0] != '*') {
            strcpy(aux, buffer);
            keyOfBuffer = strtok(aux, "|");  // pega apenas o número do registro de cada registro no buffer, pois vai ler até chegar no |
            key = atoi(keyOfBuffer);         // converte o valor de char da aux para int e armazena em key

            if (chave == key) {
                achou = 1;
                if (!isRemove) printf("%s %d bytes", buffer, *tam_reg);
                *posicao_de_seek = posicao_de_leitura;
                return achou;  //posicao_de_seek recebe a posicao do registro buscado
            }
        }
    }

    return achou;
}