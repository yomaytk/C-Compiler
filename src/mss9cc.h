#include <stdio.h>
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
	TK_SIZEOF,
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

int is_alnum(char c);
void tokenize_debug(Token *tok);

/* ~~~~~ parse.c ~~~~~ */

// signal type
typedef enum{
	ND_MAIN,
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
	ND_BLOCK,		// {}
	ND_APP,			
	ND_FUN,
	ND_ADDR,		// &p
	ND_DEREF,		// *p	
} Nodekind;

typedef struct Node Node;
typedef struct LVar LVar;
typedef struct Type Type;

typedef enum {
		INT,	// int
		PTR,	// pointer
		ARRAY,	// array
} Ty;

struct Node{
	Nodekind kind;
	Node *lhs;
	Node *mhs;
	Node *rhs;
	Node *vector;
	Node *params;
	int val;
	int offset;
	char token[100];
	int labelcnt[100];
	LVar *locals_s;
	LVar *locals_e;
	int locals_cnt;
	int params_cnt;
	Type *type;
	Node *defnode;
	Node *par;
	int var_size;
};

struct LVar {
	LVar *next;
	char *name;
	int len;
	int offset;
	Node *defnode;
};

struct Type {
	Ty ty;
	Type *ptr_to;
	size_t array_size;
	size_t ptr_size;
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

Node *new_node_num(int val);

extern Node *cur_node;
extern Node *tmp_node;
extern LVar *function_set_s;
extern LVar *function_set_e;


/* ~~~~~ main.c ~~~~~ */

void error_at(char *loc, char *fmt, ...);
void error(char *str);

extern Token *token;
extern char *user_input;
extern int main_flag;

/* ~~~~~ codegen.c ~~~~~ */

void gen(Node *node);
void gen_lval(Node *node);
void rsp_16n(Node *node, int paramscnt);

extern int label_cnt;
