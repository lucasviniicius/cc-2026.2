#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador.h"

int simbolo_lido;
char entrada[256];
int posicao = 0;

void nome_token(int token, char saida[]){
    switch (token) {
        case IDENT:      strcpy(saida, "IDENT"); break;  
        case MAIS:       strcpy(saida, "+"); break;
        case NUMERO:     strcpy(saida, "NUMERO"); break;
        case MULT:       strcpy(saida, "*"); break;
        case POTENCIA:   strcpy(saida, "**"); break;
        case ABRE_PAR:   strcpy(saida, "("); break;
        case FECHAR_PAR: strcpy(saida, ")"); break;
        case FIM:        strcpy(saida, "FIM"); break;
        default:         strcpy(saida, "DESCONHECIDO"); break;
    }
}

int proximo_token() {
    while (entrada[posicao] == ' ' || entrada[posicao] == '\t' || 
           entrada[posicao] == '\n' || entrada[posicao] == '\r') {
        posicao++;
    }

    if (entrada[posicao] == '\0') {
        return FIM;
    }

    if (isalpha(entrada[posicao])) {
        while (isalnum(entrada[posicao])) {
            posicao++;
        }
        return IDENT;
    }

    if (isdigit(entrada[posicao])) {
        while (isdigit(entrada[posicao])) {
            posicao++;
        }
        return NUMERO;
    }

    char c = entrada[posicao];
    posicao++;

    switch (c) {
        case '+': return MAIS;
        case '(': return ABRE_PAR;
        case ')': return FECHAR_PAR;
        case '*':
            if (entrada[posicao] == '*') {
                posicao++;
                return POTENCIA;
            }
            return MULT;
        default:
            printf("\n[Erro Léxico] Caractere inválido '%c' na posição %d.\n", c, posicao - 1);
            exit(1);
    }
}

void obtenha_simbolo(){
    simbolo_lido = proximo_token();
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