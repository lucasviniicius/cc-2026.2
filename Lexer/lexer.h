
#include <stdio.h>
#include "token.h"

// declara que as funções do lexer estão definidas
// externamente (em outro arquivo).

extern token next_token();
extern void reset_lexer(FILE* in);
