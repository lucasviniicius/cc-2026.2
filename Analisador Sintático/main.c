#include <stdio.h>
#include "analisador.h"

int main(){
    obtenha_simbolo();
    expr();

    if(simbolo_lido == FIM){
        printf("Expressão válida");
    } else {
        erro("símbolo inesperado");
    }

    return 0;
}