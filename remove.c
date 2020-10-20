#include <string.h>

#include "main.h"

void remocao(int posicao_de_seek, int *LEDHead) {
    char asterisco = '*';

    rewind(arquivo_dat);
    fread(LEDHead, sizeof(int), 1, arquivo_dat);

    fseek(arquivo_dat, (posicao_de_seek + 2), SEEK_SET);
    fwrite(&asterisco, sizeof(char), 1, arquivo_dat);
    fwrite(LEDHead, sizeof(int), 1, arquivo_dat);
    *LEDHead = posicao_de_seek;
    rewind(arquivo_dat);
    fwrite(LEDHead, sizeof(int), 1, arquivo_dat);  //no cabecalho
}