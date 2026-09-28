#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

void avancar(Lexer *lexer){
    if (lexer->c == '\n') {
        lexer->linha++;
        lexer->coluna = 1;
    } else {
        lexer->coluna++;
    }
    lexer->c = fgetc(lexer->arquivo);
}

void lexer_iniciar(Lexer *lexer, FILE *arquivo){
    lexer->arquivo = arquivo;
    lexer->linha = 1;
    lexer->coluna = 0;
    lexer->c = '\0';
    avancar(lexer);
}

TipoToken verificar_palavra_reservada(const char *texto) {
    if (strcmp(texto, "program") == 0) return TOKEN_PROGRAM;
    if (strcmp(texto, "var") == 0) return TOKEN_VAR;
    if (strcmp(texto, "begin") == 0) return TOKEN_BEGIN;
    if (strcmp(texto, "end") == 0) return TOKEN_END;
    if (strcmp(texto, "integer") == 0) return TOKEN_INTEGER;
    if (strcmp(texto, "real") == 0) return TOKEN_REAL;
    if (strcmp(texto, "char") == 0) return TOKEN_CHAR;
    if (strcmp(texto, "if") == 0) return TOKEN_IF;
    if (strcmp(texto, "then") == 0) return TOKEN_THEN;
    if (strcmp(texto, "else") == 0) return TOKEN_ELSE;
    if (strcmp(texto, "while") == 0) return TOKEN_WHILE;
    if (strcmp(texto, "do") == 0) return TOKEN_DO;
    if (strcmp(texto, "repeat") == 0) return TOKEN_REPEAT;
    if (strcmp(texto, "until") == 0) return TOKEN_UNTIL;
    if (strcmp(texto, "write") == 0) return TOKEN_WRITE;
    if (strcmp(texto, "div") == 0) return TOKEN_DIV;
    if (strcmp(texto, "and") == 0) return TOKEN_AND;
    if (strcmp(texto, "or") == 0) return TOKEN_OR;
    if (strcmp(texto, "not") == 0) return TOKEN_NOT;

    return TOKEN_IDENTIFICADOR;
}

Token lexer_proximo_token(Lexer *lexer){
    Token token;
    token.lexema[0] = '\0';

    
    while (lexer->c != EOF) {
        if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r') {
            avancar(lexer);
            continue;
        }

        if (lexer->c == '/') {
            int proximo = fgetc(lexer->arquivo);
            if (proximo == '/') {
                avancar(lexer);
                avancar(lexer);
                while (lexer->c != '\n' && lexer->c != EOF) {
                    avancar(lexer);
                }
                continue;
            } else {
                ungetc(proximo, lexer->arquivo);
                break;
            }
        }
        break;
    }

    token.linha = lexer->linha;
    token.coluna = lexer->coluna;

    if (lexer->c == EOF) {
        token.tipo = TOKEN_EOF;
        strcpy(token.lexema, "EOF");
        return token;
    }

    if (isalpha(lexer->c) || lexer->c == '_') {
        int i = 0;
        while ((isalpha(lexer->c) || isdigit(lexer->c) || lexer->c == '_') && lexer->c != EOF) {
            if (i < 255) token.lexema[i++] = lexer->c;
            avancar(lexer);
        }
        token.lexema[i] = '\0';
        token.tipo = verificar_palavra_reservada(token.lexema);
        return token;
    }

    if (isdigit(lexer->c)) {
        int i = 0;
        int real = 0;

        while (isdigit(lexer->c) && lexer->c != EOF) {
            if (i < 255) token.lexema[i++] = lexer->c;
            avancar(lexer);
        }
    
        if (lexer->c == '.') {
            int proximo = fgetc(lexer->arquivo);
            if (isdigit(proximo)) {
                real = 1;
                if (i < 255) token.lexema[i++] = '.';
                if (i < 255) token.lexema[i++] = proximo;
                avancar(lexer);

                while (isdigit(lexer->c) && lexer->c != EOF) {
                    if (i < 255) token.lexema[i++] = lexer->c;
                    avancar(lexer);
                }
            } else {
                ungetc(proximo, lexer->arquivo);
            }
        }

        token.lexema[i] = '\0';
        token.tipo = real ? TOKEN_LIT_REAL : TOKEN_LIT_INTEIRO;
        return token;   
    }

    if (lexer->c == '\'') {
        int i = 0;
        token.lexema[i++] = '\'';
        avancar(lexer);

        if (lexer->c == '\\') {
            token.lexema[i++] = '\\';
            avancar(lexer);
            if (lexer->c == 'n' || lexer->c == 't' || lexer->c == '\'' || lexer->c == '\\') {
                token.lexema[i++] = lexer->c;
                avancar(lexer);
            }
        } else if (lexer->c != '\'' && lexer->c != EOF) {
            token.lexema[i++] = lexer->c;
            avancar(lexer);
        }

        if (lexer->c == '\'') {
            token.lexema[i++] = '\'';
            avancar(lexer);
            token.lexema[i] = '\0';
            token.tipo = TOKEN_LIT_CHAR;
            return token;
        } else {
            printf("Erro léxico no caracter [%c]\n", lexer->c);
            token.tipo = TOKEN_ERRO;
            return token;
        }
    }

    if (lexer->c == ':') {
        avancar(lexer);
        if (lexer->c == '=') {
            strcpy(token.lexema, ":=");
            token.tipo = TOKEN_ATRIBUICAO;
            avancar(lexer);
        } else {
            strcpy(token.lexema, ":");
            token.tipo = TOKEN_DOIS_PONTOS;
        }
        return token;
    }

    if (lexer->c == '<') {
        avancar(lexer);
        if (lexer->c == '=') {
            strcpy(token.lexema, "<=");
            token.tipo = TOKEN_MENOR_IGUAL;
            avancar(lexer);
        } else if (lexer->c == '>') {
            strcpy(token.lexema, "<>");
            token.tipo = TOKEN_DIFERENTE;
            avancar(lexer);
        } else {
            strcpy(token.lexema, "<");
            token.tipo = TOKEN_MENOR;
        }
        return token;
    }

    if (lexer->c == '>') {
        avancar(lexer);
        if (lexer->c == '=') {
            strcpy(token.lexema, ">=");
            token.tipo = TOKEN_MAIOR_IGUAL;
            avancar(lexer);
        } else {
            strcpy(token.lexema, ">");
            token.tipo = TOKEN_MAIOR;
        }
        return token;
    }

    char c_atual = lexer->c;
    token.lexema[0] = c_atual;
    token.lexema[1] = '\0';
    switch (c_atual) {
        case '+':
            token.tipo = TOKEN_MAIS;
            avancar(lexer);
            return token;
        case '-':
            token.tipo = TOKEN_MENOS;
            avancar(lexer);
            return token;
        case '*':
            token.tipo = TOKEN_MULT;
            avancar(lexer);
            return token;
        case '/':
            token.tipo = TOKEN_DIV_REAL;
            avancar(lexer);
            return token;
        case '(':
            token.tipo = TOKEN_ABRE_PAR;
            avancar(lexer);
            return token;
        case ')':
            token.tipo = TOKEN_FECHA_PAR;
            avancar(lexer);
            return token;
        case ',':
            token.tipo = TOKEN_VIRGULA;
            avancar(lexer);
            return token;
        case ';':
            token.tipo = TOKEN_PONTO_VIRGULA;
            avancar(lexer);
            return token;
        case '=':
            token.tipo = TOKEN_IGUAL;
            avancar(lexer);
            return token;
        case '.':
            token.tipo = TOKEN_PONTO;
            avancar(lexer);
            return token;
        default:
            printf("Erro léxico no caracter [%c]\n", c_atual);
            token.tipo = TOKEN_ERRO;
            avancar(lexer);
            return token;
    }
}