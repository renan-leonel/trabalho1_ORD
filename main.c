/*Coleta de livros.txt e alocando em dados.dat, colocando o tamanho na frente
  de cada registro, salvando tudo em binário.*/

#include "main.h"

#include <stdlib.h>
#include <string.h>

#include "import.h"
#include "insert.h"
#include "read_record.h"
#include "remove.h"
#include "search.h"

int buscarChave(char *str, int tamanho_chave) {
    char chave_da_operacao[TAM_MAX_REG];
    int chave, i;

    for (i = 2; i < tamanho_chave; i++) {  // começa em dois para pular a letra de comando e o espaço
                                           //  buffer2[i - 2] = str[i];
        chave_da_operacao[i - 2] = str[i];
    }
    chave_da_operacao[i - 2] = '\0';

    return chave = atoi(chave_da_operacao);
}

void buscarRegistro(char *str, int tamanho_chave) {
    static char reg_da_operacao[TAM_MAX_REG];
    int i;

    for (i = 2; i < tamanho_chave; i++) {
        str[i - 2] = str[i];
    }
    str[i - 2] = '\0';
}

int main(int argc, char **argv) {
    int LEDHead = -1;
    short tam_reg_operacoes = 0;

    //VARIAVEIS PARA AS OPERACOES
    FILE *foperacoes;                    //receberá o arquivo operacoes.txt para leitura
    char buffer_operacoes[TAM_MAX_REG];  //será usado para armazenar os registros de operacoes.txt
    int controle;                        //variável que receberá o número do caso para cada operação

    //Para verificar a abertura do programa:
    if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-i") == 0) {  //acessando a funcionalidade de implementação

        importacao(argv, LEDHead);

    } else if (strcmp(argv[1], "-e") == 0) {
        short tam_reg;
        int chave = 0;
        int posicao_de_seek = 0;
        char reg[TAM_MAX_REG];

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);

        if ((arquivo_dat = fopen("dados.dat", "r+b")) == NULL) {
            printf("Arquivo dados.dat não existe, programa finalizado.\n");
            system("pause");
            exit(1);
        }

        foperacoes = fopen("operacoes.txt", "r");

        tam_reg_operacoes = leia_registro(foperacoes, buffer_operacoes, TAM_MAX_REG);  //armazena o tamanho do primeiro registro de operacoes.txt
        while (tam_reg_operacoes > 0) {
            switch (buffer_operacoes[0]) {
                case BUSCA:
                    chave = buscarChave(buffer_operacoes, tam_reg_operacoes);
                    if (!(busca(chave, &posicao_de_seek, &tam_reg, 0))) printf("Erro: registro nao encontrado!");
                    break;
                case REMOCAO:
                    chave = buscarChave(buffer_operacoes, tam_reg_operacoes);
                    printf("\n\nRemocao do registro de chave %d\n", chave);

                    if (busca(chave, &posicao_de_seek, &tam_reg, 1)) {  //verificando se o registro existe, se existir então procedemos com a remoção
                        remocao(posicao_de_seek, &LEDHead);
                        printf("Registro removido! (%d bytes)\nPosicao: offset = %d bytes", tam_reg, posicao_de_seek);
                    } else {
                        printf("Registro nao encontrado!");
                    }
                    break;
                case INSERCAO:
                    buscarRegistro(buffer_operacoes, tam_reg_operacoes);
                    tam_reg = tam_reg_operacoes - 2;
                    inserir(buffer_operacoes, &tam_reg);
                    break;
                default:
                    printf("Erro na leitura das operações do modo exportação.");
                    break;
            }

            tam_reg_operacoes = leia_registro(foperacoes, buffer_operacoes, TAM_MAX_REG);
        }
    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}