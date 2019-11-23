#define bool _Bool

/* ~~~~~ tokenize.c ~~~~~ */

// token type
typedef enum {
	TK_RESERVED, // signal
	TK_IDENT,	// identification
	TK_NUM,      // integer token
	TK_RETURN,	// return token
	TK_IF,
	TK_ELSE,	
	TK_WHILE,
	TK_FOR,
	TK_EOF,      // end token
} TokenKind;

typedef struct Token Token;

// token type
struct Token {
	TokenKind kind; // トークンの型
	Token *next;    // 次の入力トークン
	int val;        // kindがTK_NUMの場合、その数値
	char *str;      // トークン文字列
	int len;
};

Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

typedef struct LVar LVar;

struct LVar {
	LVar *next;
	char *name;
	int len;
	int offset;
};

extern LVar *locals_s;
extern LVar *locals_e;

int is_alnum(char c);
void tokenize_debug(Token *tok);

/* ~~~~~ parse.c ~~~~~ */

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
	ND_SEMICORO,	// ;
	ND_RETURN,		// return
	ND_IF,			// if
	ND_ELSE,		// else
	ND_WHILE,
	ND_FOR,
	ND_FOREXPR,		// condition of for
	ND_BLOCK,
	ND_APP,
	ND_FUN,
} Nodekind;

typedef struct Node Node;

struct Node{
	Nodekind kind;
	Node *lhs;
	Node *mhs;
	Node *rhs;
	Node *vector;
	int val;
	int offset;
	char token[100];
	int labelcnt[10];
	// LVar *locals_s;
	// LVar *locals_e;
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
bool consume_tokenstay(char *op);
Token *consume_ident();
int expect_number();
void expect(char op);
int expect_number();
void gen(Node *node);
bool at_eof();

extern Node *code[100];

LVar *find_lvar(Token *tok, Node *node);
char* syntax_debug(Node *code);

extern Node *cur_node;
extern Node *tmp_node;

/* ~~~~~ main.c ~~~~~ */

void error_at(char *loc, char *fmt, ...);
void error(char *str);

extern Token *token;
extern char *user_input;
extern int main_flag;

/* ~~~~~ codegen.c ~~~~~ */

void gen(Node *node);
void gen_lval(Node *node);
void rsp_16n(Node *node);

extern int label_cnt;
