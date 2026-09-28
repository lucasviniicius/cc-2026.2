#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s \n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo: %s\n", argv[1]);
        return 1;
    }

    Parser parser;
    

    parser_iniciar(&parser, arquivo);
    
    parse(&parser);

    fclose(arquivo);
    return 0;
}