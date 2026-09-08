#include <stdio.h>
#include "analisador.h"

int main(){
    obtenha_simbolo();
    expr();

    if(simbolo_lido == FIM){
        printf("Expressao valida");
    } else {
        erro("simbolo inesperado");
    }

    return 0;
}