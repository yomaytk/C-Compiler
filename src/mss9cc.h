#define bool _Bool

/* ~~~~~ parse.c ~~~~~ */
// token type
typedef enum {
	TK_RESERVED, // signal
	TK_NUM,      // integer token
	TK_EOF,      // end token
} TokenKind;

// signal type
typedef enum{
	ND_ADD,		// +
	ND_SUB,		// -
	ND_MUL,		// *
	ND_DIV,		// /
	ND_NUM,		// digit
	ND_EQU,		// ==
	ND_NEQ,		// !=
	ND_RIL,		// <
	ND_RLE,		// <=
	ND_LIL, 	// >
	ND_LLE,		// >=
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
};

Node *expr();
Node *equarity();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
bool consume(char *op);
int expect_number();
void expect(char op);
int expect_number();
void gen(Node *node);
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

/* ~~~~~ main.c ~~~~~ */

void error_at(char *loc, char *fmt, ...);

extern Token *token;
extern char *user_input;

/* ~~~~~ codegen.c ~~~~~ */

void gen(Node *node);

