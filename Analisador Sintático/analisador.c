#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador.h"

int simbolo_lido;
int posicao = 0;
int tokens[] = {
    MAIS,
    IDENT,
    FIM
};

void nome_token(int token, char saida[]){
    switch (token){
        case IDENT: 
            strcpy(saida, "IDENT");
            break;  
        case MAIS:
            strcpy(saida, "+");
            break;
        case NUMERO:
            strcpy(saida, "NUMERO");
            break;
        case MULT:
            strcpy(saida, "*");
            break;
        case POTENCIA:
            strcpy(saida, "**");
            break;
        case ABRE_PAR:
            strcpy(saida, "(");
            break;
        case FECHAR_PAR:
            strcpy(saida, ")");
            break;
        case FIM:
            strcpy(saida, "FIM");
            break;
        default:
            strcpy(saida, "DESCONHECIDO");
            break;
    }
}

void obtenha_simbolo(){
    simbolo_lido = tokens[posicao];
    posicao++;
}

void erro(const char *mensagem){
    char token[30];
    nome_token(simbolo_lido, token);

    printf("Erro sintático: %s\n", mensagem);
    printf("Símbolo lido: %s\n", token);
    exit(1);
}

void expr(){
    termo();

    if(simbolo_lido == MAIS){
        obtenha_simbolo();
        expr();
    }
}

void termo(){
    fator();

    if(simbolo_lido == MULT){
        obtenha_simbolo();
        termo();
    }
}

void fator(){
    primario();

    if(simbolo_lido == POTENCIA){
        obtenha_simbolo();
        fator();
    }
}

void primario(){
    if(simbolo_lido == IDENT){
        printf("IDENT\n");
        obtenha_simbolo();
    } else if(simbolo_lido == NUMERO){
        printf("NUMERO\n");
        obtenha_simbolo();
    } else if(simbolo_lido == ABRE_PAR){
        printf("(");
        obtenha_simbolo();
        expr();

        if(simbolo_lido != FECHAR_PAR){
            erro("falta ')'");
        } else {
            obtenha_simbolo();
        }
    } else {
        erro("primário inválido");
    }
}
