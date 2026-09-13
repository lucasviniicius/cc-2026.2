#define IDENT 1
#define NUMERO 2
#define MAIS 3
#define MULT 4
#define POTENCIA 5
#define ABRE_PAR 6
#define FECHAR_PAR 7
#define FIM 8

extern int simbolo_lido;
extern char entrada[256];
extern int posicao;

void nome_token(int token, char saida[]);
int proximo_token();
void obtenha_simbolo();
void erro(const char *mensagem);
void expr();
void termo();
void fator();
void primario();