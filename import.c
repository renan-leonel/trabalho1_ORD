#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "read_record.h"

void importacao(char **argv, int LEDHead) {
    FILE *flivros;  //para receber e ler o arquivo livros.txt
    short tam_reg_operacoes = 0;
    int num_reg = 0;
    char buffer_livros[TAM_MAX_REG];

    arquivo_dat = fopen("dados.dat", "wb");  //abertura e criação do arquivo para escrever os livros com seu tamanho de registro
    flivros = fopen(argv[2], "r");           //abertura do arquivo livros.txt para coletar os registros

    printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);

    if (arquivo_dat == NULL) {
        printf("\nErro na abertura do arquivo -- programa finalizado.\n");
        system("pause");
        exit(1);
    }

    tam_reg_operacoes = leia_registro(flivros, buffer_livros, TAM_MAX_REG);  //faz a leitura do registro de livros.txt, retornando seu tamanho em tam_reg e o registro em buffer

    fwrite(&LEDHead, sizeof(int), 1, arquivo_dat);  //escreve o cabeçalho de dados.dat com -1 na LED de espaços vazios

    while (tam_reg_operacoes > 0) {
        fwrite(&tam_reg_operacoes, sizeof(short), 1, arquivo_dat);            //escreve o tamanho do registro a ser inserido
        fwrite(buffer_livros, sizeof(char), tam_reg_operacoes, arquivo_dat);  //escreve o registro inserido

        tam_reg_operacoes = leia_registro(flivros, buffer_livros, TAM_MAX_REG);
        num_reg++;  //conta quantos registros foram coletados
    }

    if (num_reg > 0) {
        printf("Operacao concluida.\nQuantidade de registros coletados e inseridos em dados.dat: %d\n", num_reg);
    } else {
        printf("Erro na operação.\n");
    }

    fclose(flivros);      //fecha o arquivo livros.txt
    fclose(arquivo_dat);  //fecha o arquivo dados.dat
}
