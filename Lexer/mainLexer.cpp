/**
 * Este arquivo serve para testar o analisador léxico.
 * Para isso, é preciso acrescentá-lo no projeto, removendo
 * também o arquivo "mainParser.cpp".
 */

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"


int main()
{
	token token;

	printf("\n\n\n");
	printf(" == TESTE DO LEXER ==\n\n");
	printf(" Digite alguma expressão terminada em \";\" e tecle ENTER:\n\n\n ");

	// passa a entrada padrão para o lexer
	reset_lexer(stdin);

	do {
		token = next_token();
		printf("\t[tipo_%d, \"%c\"]\n", token.type, token.lexeme);

	} while (token.type != TOKEN_PT_VIRG);

	printf("\n == FIM ==\n");

	system("PAUSE");
	return 1;
}
