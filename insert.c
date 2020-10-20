#include <stdio.h>
#include <string.h>

#include "main.h"

void inserir(char* registro, short* tam_reg) {
    int espaco_disponivel = 1;  // funcionará como uma true/false para ver se é possível realizar a inserção
    short comprimento_reg = 0;
    char* chave;
    int cabeca = 0;
    int anterior = 0;
    int agora = 0;
    int prox = 0;
    int led;
    char copia_registro[TAM_MAX_REG];

    anterior = -1;
    rewind(arquivo_dat);  // reposiciona o ponteiro de L/E do arquivo no byte 0;
    agora = 0;
    fread(&cabeca, sizeof(int), 1, arquivo_dat);
    led = cabeca;
    prox = cabeca;

    strcpy(copia_registro, registro);
    chave = strtok(copia_registro, DELIM_STR);
    if (cabeca != -1) {
        while (espaco_disponivel && (cabeca != -1)) {
            fseek(arquivo_dat, cabeca, SEEK_SET);                    // posiciona o ponteiro de L/E do arquivo no offset da head
            fread(&comprimento_reg, sizeof(short), 1, arquivo_dat);  // faz a leitura do tamanho disponível
            fseek(arquivo_dat, 1, SEEK_CUR);                         //pular *
            anterior = agora;
            agora = prox;

            if (comprimento_reg >= (*tam_reg) + 2) {
                espaco_disponivel = 0;
                fread(&prox, sizeof(int), 1, arquivo_dat);
            } else {
                fread(&cabeca, sizeof(int), 1, arquivo_dat);  //faz leitura da LED
                prox = cabeca;
            }
        }

        // depois da busca por espaco disponivel, se ele existir, vai ser sobrescrito
        if (!espaco_disponivel) {
            short sobra = comprimento_reg - ((*tam_reg) - 2);
            int posicao_de_escrita = 0;
            char asterisco = '*';

            printf("\n\nInsercao do registro de chave '%s'\n", chave);
            printf("Local: offset = %d\n", cabeca);
            printf("Tamanho livre: %d bytes\n", comprimento_reg);

            if (sobra > 50) {
                fseek(arquivo_dat, agora, SEEK_SET);
                fwrite(tam_reg, sizeof(short), 1, arquivo_dat);  //escreve o tamanho do registro a ser inserido
                fwrite(registro, sizeof(char), *tam_reg, arquivo_dat);
                posicao_de_escrita = ftell(arquivo_dat);
                agora = posicao_de_escrita;

                fwrite(&sobra, sizeof(short), 1, arquivo_dat);
                fwrite(&asterisco, sizeof(char), 1, arquivo_dat);
                fwrite(&led, sizeof(int), 1, arquivo_dat);
                fseek(arquivo_dat, 0, SEEK_SET);
                fwrite(&agora, sizeof(int), 1, arquivo_dat);
                fseek(arquivo_dat, (anterior + 3), SEEK_SET);
                fwrite(&prox, sizeof(int), 1, arquivo_dat);

                cabeca = agora;
                printf("Tamanho da sobra: %d bytes\n", sobra);
                printf("Offset da sobra %d (0x%x)\n\n", cabeca, cabeca);
            } else {
                fseek(arquivo_dat, agora, SEEK_SET);
                fwrite(&comprimento_reg, sizeof(short), 1, arquivo_dat);  //escreve o tamanho do registro a ser inserido
                fwrite(registro, sizeof(char), *tam_reg, arquivo_dat);
                posicao_de_escrita = ftell(arquivo_dat);
                agora = posicao_de_escrita;

                if (anterior == 0) {
                    fseek(arquivo_dat, 0, SEEK_SET);
                } else {
                    fseek(arquivo_dat, (anterior + 3), SEEK_SET);
                }
                fwrite(&prox, sizeof(int), 1, arquivo_dat);
            }
        } else {
            fseek(arquivo_dat, 0, SEEK_END);
            fwrite(tam_reg, sizeof(short), 1, arquivo_dat);  //escreve o tamanho do registro a ser inserido
            fwrite(registro, sizeof(char), *tam_reg, arquivo_dat);
            printf("\n\nInsercao do registro de chave '%s'\n", chave);
            printf("Inserido %d bytes no final\n\n", *tam_reg);
        }
    } else {                              // para escrever no final do arquivo
        fseek(arquivo_dat, 0, SEEK_END);  // reposiciona o ponteiro de L/E do arquivo no final do arquivo

        fwrite(tam_reg, sizeof(short), 1, arquivo_dat);         //escreve o tamanho do registro a ser inserido
        fwrite(registro, sizeof(char), *tam_reg, arquivo_dat);  //escreve o registro inserido
        printf("\n\nInsercao do registro de chave '%s' (%d bytes)\n", chave, *tam_reg);
        printf("Local: ao final do arquivo (LED vazia)");
    }
}