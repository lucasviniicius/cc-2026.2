#include "token.h"

typedef struct {
    FILE *arquivo;
    int c;
    int linha;
    int coluna;
} Lexer;

void lexer_iniciar(Lexer *lexer, FILE *arquivo);
Token lexer_proximo_token(Lexer *lexer);