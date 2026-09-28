#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void erro(Parser *p);
void proximo(Parser *p);
void consumir(Parser *p, TipoToken tipoEsperado);

void proximo(Parser *p) {
    p->token = lexer_proximo_token(&p->lexer);
    if (p->token.tipo == TOKEN_ERRO) {
        exit(1);
    }
}

void parser_iniciar(Parser *p, FILE *arquivo) {
    lexer_iniciar(&p->lexer, arquivo);
    proximo(p);
}

void erro(Parser *p){
    printf("Erro de sintaxe no token [%s]\n", p->token.lexema);
    exit(1);
}

void consumir(Parser *p, TipoToken tipoEsperado){
    if(p->token.tipo == tipoEsperado){
        proximo(p);
    } else {
        erro(p);
    }
}

static void parseSecaoVar(Parser *p);
static void parseDeclVar(Parser *p);
static void parseTipo(Parser *p);
static void parseBloco(Parser *p);
static void parseListaComandos(Parser *p);
static void parseComando(Parser *p);
static void parseAtribuicao(Parser *p);
static void parseIteracao(Parser *p);
static void parseDecisao(Parser *p);
static void parseEscrita(Parser *p);
static void parseExpressao(Parser *p);
static void parseExprRelacional(Parser *p);
static void parseExprAritmetica(Parser *p);
static void parseTermo(Parser *p);
static void parseFator(Parser *p);

void parse(Parser *p){
    consumir(p, TOKEN_PROGRAM);
    consumir(p, TOKEN_IDENTIFICADOR);
    consumir(p, TOKEN_PONTO_VIRGULA);
    parseSecaoVar(p);
    parseBloco(p);
    consumir(p, TOKEN_PONTO);
    printf("Sintaxe reconhecida com sucesso!\n");
}

void parseSecaoVar(Parser *p){
    if(p->token.tipo == TOKEN_VAR){
        consumir(p, TOKEN_VAR);
        while(p->token.tipo == TOKEN_IDENTIFICADOR){
            parseDeclVar(p);
        }
    }
}

void parseDeclVar(Parser *p){
    consumir(p, TOKEN_IDENTIFICADOR);
    while (p->token.tipo == TOKEN_VIRGULA) {
        consumir(p, TOKEN_VIRGULA);
        consumir(p, TOKEN_IDENTIFICADOR);
    }
    consumir(p, TOKEN_DOIS_PONTOS);
    parseTipo(p);
    consumir(p, TOKEN_PONTO_VIRGULA);
}

void parseTipo(Parser *p){
    if (p->token.tipo == TOKEN_INTEGER ||
        p->token.tipo == TOKEN_REAL ||
        p->token.tipo == TOKEN_CHAR) {
        proximo(p);
    } else {
        erro(p);
    }
}

void parseBloco(Parser *p){
    consumir(p, TOKEN_BEGIN);
    parseListaComandos(p);
    consumir(p, TOKEN_END);
}

void parseListaComandos(Parser *p) {
    while (p->token.tipo == TOKEN_BEGIN ||
           p->token.tipo == TOKEN_IDENTIFICADOR ||
           p->token.tipo == TOKEN_WHILE ||
           p->token.tipo == TOKEN_REPEAT ||
           p->token.tipo == TOKEN_IF ||
           p->token.tipo == TOKEN_WRITE) {
        parseComando(p);
    }
}

void parseComando(Parser *p){
    if(p->token.tipo == TOKEN_BEGIN){
        parseBloco(p);
        consumir(p, TOKEN_PONTO_VIRGULA);
    } else if (p->token.tipo == TOKEN_IDENTIFICADOR) {
        parseAtribuicao(p);
    } else if (p->token.tipo == TOKEN_WHILE || p->token.tipo == TOKEN_REPEAT) {
        parseIteracao(p);
    } else if (p->token.tipo == TOKEN_IF) {
        parseDecisao(p);
    } else if (p->token.tipo == TOKEN_WRITE) {
        parseEscrita(p);
    } else {
        erro(p);
    }
}

void parseAtribuicao(Parser *p){
    consumir(p, TOKEN_IDENTIFICADOR);
    consumir(p, TOKEN_ATRIBUICAO);
    parseExpressao(p);
    consumir(p, TOKEN_PONTO_VIRGULA);
}

void parseIteracao(Parser *p){
    if(p->token.tipo == TOKEN_WHILE){
        consumir(p, TOKEN_WHILE);
        parseExpressao(p);
        consumir(p, TOKEN_DO);
        parseComando(p);
    } else if(p->token.tipo == TOKEN_REPEAT){
        consumir(p, TOKEN_REPEAT);
        parseComando(p);
        consumir(p, TOKEN_UNTIL);
        parseExpressao(p);
        consumir(p, TOKEN_PONTO_VIRGULA);
    }
}

void parseDecisao(Parser *p){
    consumir(p, TOKEN_IF);
    parseExpressao(p);
    consumir(p, TOKEN_THEN);
    parseComando(p);
    if(p->token.tipo == TOKEN_ELSE){
        consumir(p, TOKEN_ELSE);
        parseComando(p);
    }
}

void parseEscrita(Parser *p){
    consumir(p, TOKEN_WRITE);
    consumir(p, TOKEN_ABRE_PAR);
    parseExpressao(p);
    consumir(p, TOKEN_FECHA_PAR);
    consumir(p, TOKEN_PONTO_VIRGULA);
}

void parseExpressao(Parser *p){
    parseExprRelacional(p);
    while(p->token.tipo == TOKEN_OR || p->token.tipo == TOKEN_AND){
        proximo(p);
        parseExprRelacional(p);
    }
}

void parseExprRelacional(Parser *p){
    parseExprAritmetica(p);
    while(p->token.tipo == TOKEN_IGUAL || p->token.tipo == TOKEN_DIFERENTE ||
           p->token.tipo == TOKEN_MENOR || p->token.tipo == TOKEN_MENOR_IGUAL ||
           p->token.tipo == TOKEN_MAIOR || p->token.tipo == TOKEN_MAIOR_IGUAL){
        proximo(p);
        parseExprAritmetica(p);
    }
}

void parseExprAritmetica(Parser *p){
    parseTermo(p);
    while (p->token.tipo == TOKEN_MAIS || p->token.tipo == TOKEN_MENOS) {
        proximo(p);
        parseTermo(p);
    }
}

void parseTermo(Parser *p){
    parseFator(p);
    while(p->token.tipo == TOKEN_MULT || p->token.tipo == TOKEN_DIV_REAL ||
           p->token.tipo == TOKEN_DIV){
        proximo(p);
        parseFator(p);
    }
}

void parseFator(Parser *p){
    if(p->token.tipo == TOKEN_ABRE_PAR){
        consumir(p, TOKEN_ABRE_PAR);
        parseExpressao(p);
        consumir(p, TOKEN_FECHA_PAR);
    } else if(p->token.tipo == TOKEN_NOT){
        consumir(p, TOKEN_NOT);
        parseExpressao(p);
    } else if(p->token.tipo == TOKEN_LIT_INTEIRO ||
               p->token.tipo == TOKEN_LIT_REAL ||
               p->token.tipo == TOKEN_LIT_CHAR ||
               p->token.tipo == TOKEN_IDENTIFICADOR){
        proximo(p);
    } else {
        erro(p);
    }
}