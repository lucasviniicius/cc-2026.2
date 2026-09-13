#include <stdio.h>
#include "analisador.h"

int main(){
    printf("Digite a expressão: ");
    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return 0;
    }

    posicao = 0;
    obtenha_simbolo();

    expr();

    if(simbolo_lido == FIM){
        printf("Expressao valida");
    } else {
        erro("simbolo inesperado");
    }

    return 0;
}