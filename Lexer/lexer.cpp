/**
 * Este arquivo define a análise léxica para uma linguagem de expressões
 * algébricas. Ela le um arquivo, dividindo-o em trechos menores (lexemas)
 * que são classificados e retornados como tokens. Os tokens são retornados
 * um por vez, a cada chamada da função "next_token".
 *
 * Autor: Pablo Sampaio
 */

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

 // variáveis usadas na análise léxica
FILE* input = NULL;
int nextChar = -1;


// reinicia a análise léxica
void reset_lexer(FILE* in) {
	if (input != NULL) {
		delete input;
	}

	// contem o codigo fonte a ser analisado
	input = in;
	// anda para o primeiro byte da entrada 
	nextChar = getc(input);
}


// função principal, que identifica qual o proximo
// token da entrada para retorna-lo. 
token next_token() {
	token token;
	token_type tipo;

	// testa se chegou ao fim do arquivo
	if (nextChar == -1) {
		token.type = TOKEN_EOF;
		token.lexeme = 0;
		return token;
	}

	// identifica os demais tipos de tokens
	if (nextChar >= '0' && nextChar <= '9') {
		tipo = TOKEN_NUMERO;
	} else if (nextChar == ';') {
		tipo = TOKEN_PT_VIRG;
	} else if (nextChar == '+') {
		tipo = TOKEN_MAIS;
	} else if (nextChar == '*') {
		tipo = TOKEN_VEZES;
	} else if (nextChar == '-') {
		tipo = TOKEN_MENOS;
	} else if (nextChar == '/'){
		tipo = TOKEN_DIVISAO;
	} else if (nextChar == '(') {
		tipo = TOKEN_ABRE_PAR;
	} else if (nextChar == ')') {
		tipo = TOKEN_FECHA_PAR;
	} else {
		printf(" ERRO LEXICO: caracter inesperado - \'%c\'.\n", (char)nextChar);
		system("PAUSE");
		exit(1);
	}

	token.type = tipo;
	token.lexeme = nextChar;

	// prepara para a leitura do próximo token
	nextChar = getc(input);

	return token;
}
