#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef struct {
    Lexer lexer;
    Token token;
} Parser;

void parser_iniciar(Parser *p, FILE *arquivo);
void parse(Parser *p);

#endif