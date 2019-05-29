#include <stdio.h>

/*入力されて式に関するエラー*/
#define ERR1		-1			//全角文字が含まれている
#define ERR2		-2			//")"が不足
#define ERR3		-3			//"("が不足
#define ERR4		-4			//指数が不正
#define ERR5		-5			//底が不正
#define ERR6		-6			//定数^定数
#define ERR7		-7			//(xの式)^(xの式)
#define ERR8		-8			//括弧の位置が不正
#define ERR9		-9			//底が入力されていない
#define ERR10		-10			//底に0や1が使われている
#define ERR11		-11			//底には数字のみを使用してください
#define ERR12		-12			//指数に0が使われている

/*どの基本関数かを示す型*/
typedef enum Func {
	ERR,			//どの基本関数でもない
	SIN,			//sin(...)
	COS,			//cos(...)
	TAN,			//tan(...)
	A_FX,			//exp(a,...)
	LOG,			//log(a,...)
	SQRT,			//(...)の正の平方根
	FX_A,			//(...)^a
	X,				//x
} Func;

int brac_check(char s[]);
void help(void);
int standardize(char prob[]);
int islinear_apart(const char prob1[]);
void linear_apart(const char prob1[], char prob2[]);
void rid_const(char prob2[]);
void rid_brackets(char s[]);
void rid_double_brackets(char s[]);
void search_brackets(const char prob[], char prob1[]);
Func judge_basic_func(const char prob[]);
int judge_basic(const char s[], int f_num);
void product(const char prob[], char prob1[]);
int search_char(const char s[], char ch1, char ch2, int start);
void quotient(const char prob[], char prob1[]);
void init(char s[]);
void brackets_link(char prob[], const char prob2[]);
void exp_convert(char prob[]);
void differentiate(const char prob[], char prob1[], Func func);
void dif_sin (const char prob[], char prob1[], int len_prob);
void dif_cos (const char prob[], char prob1[], int len_prob);
void dif_tan (const char prob[], char prob1[], int len_prob);
void dif_a_fx(const char prob[], char prob1[], int len_prob);
void dif_log (const char prob[], char prob1[], int len_prob);
void dif_sqrt(const char prob[], char prob1[], int len_prob);
void dif_fx_a(const char prob[], char prob1[], int len_prob);
