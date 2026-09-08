/**
 * Este arquivo define os tipos de tokens da linguagem.
 *
 * Autor: Pablo Sampaio
 */

 // tipos de tokens
enum token_type {
	TOKEN_NUMERO = 0,
	TOKEN_PT_VIRG = 1,
	TOKEN_MAIS = 2,
	TOKEN_VEZES = 3,
	TOKEN_ABRE_PAR = 4,
	TOKEN_FECHA_PAR = 5,
	TOKEN_EOF = 6,
	TOKEN_MENOS = 7,
	TOKEN_DIVISAO = 8
};

// representação do token
// em XPR-0, o lexema pode ter apenas um caractere
struct token {
	token_type type;
	char lexeme;
};