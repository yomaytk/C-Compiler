/* ~~~~~ parse ~~~~~ */
typedef enum{
	ND_ADD,
	ND_SUB,
	ND_MUL,
	ND_DIV,
	ND_NUM,
} Nodekind;

typedef struct Node Node;

struct Node{
	Nodekind kind;
	Node *lhs;
	Node *rhs;
	int val;
};

Node *expr();
Node *mul();
Node *primary();
void gen(Node *node);

/* ~~~~~ msscc.c ~~~~~ */
// トークンの種類
typedef enum {
	TK_RESERVED, // 記号
	TK_NUM,      // 整数トークン
	TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
	TokenKind kind; // トークンの型
	Token *next;    // 次の入力トークン
	int val;        // kindがTK_NUMの場合、その数値
	char *str;      // トークン文字列
};
bool consume(char op);
int expect_number();
void expect(char op);
int expect_number();