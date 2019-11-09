#define bool _Bool

/* ~~~~~ parse.c ~~~~~ */
// token type
typedef enum {
	TK_RESERVED, // signal
	TK_IDENT,	// identification
	TK_NUM,      // integer token
	TK_EOF,      // end token
} TokenKind;

// signal type
typedef enum{
	ND_ADD,			// +
	ND_SUB,			// -
	ND_MUL,			// *
	ND_DIV,			// /
	ND_NUM,			// digit
	ND_EQU,			// ==
	ND_NOTEQU,		// !=
	ND_RIGHTINE,	// <
	ND_RINEEQU,		// <=
	ND_LEFTINE, 	// >
	ND_LINEEQU,		// >=
	ND_ASSIGN,		// =
	ND_LVAR,		// local variable
	ND_SEMICORO		// ;
} Nodekind;

typedef struct Token Token;

// token type
struct Token {
	TokenKind kind; // トークンの型
	Token *next;    // 次の入力トークン
	int val;        // kindがTK_NUMの場合、その数値
	char *str;      // トークン文字列
	int len;
};

typedef struct Node Node;

struct Node{
	Nodekind kind;
	Node *lhs;
	Node *rhs;
	int val;
	int offset;
};

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equarity();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
bool consume(char *op);
Token *consume_ident();
int expect_number();
void expect(char op);
int expect_number();
void gen(Node *node);
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

extern Node *code[100];

/* ~~~~~ main.c ~~~~~ */

void error_at(char *loc, char *fmt, ...);
void error(char *str);

extern Token *token;
extern char *user_input;

/* ~~~~~ codegen.c ~~~~~ */

void gen(Node *node);
void gen_lval(Node *node);
