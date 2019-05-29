#include <stdio.h>

/*���͂���Ď��Ɋւ���G���[*/
#define ERR1		-1			//�S�p�������܂܂�Ă���
#define ERR2		-2			//")"���s��
#define ERR3		-3			//"("���s��
#define ERR4		-4			//�w�����s��
#define ERR5		-5			//�ꂪ�s��
#define ERR6		-6			//�萔^�萔
#define ERR7		-7			//(x�̎�)^(x�̎�)
#define ERR8		-8			//���ʂ̈ʒu���s��
#define ERR9		-9			//�ꂪ���͂���Ă��Ȃ�
#define ERR10		-10			//���0��1���g���Ă���
#define ERR11		-11			//��ɂ͐����݂̂��g�p���Ă�������
#define ERR12		-12			//�w����0���g���Ă���

/*�ǂ̊�{�֐����������^*/
typedef enum Func {
	ERR,			//�ǂ̊�{�֐��ł��Ȃ�
	SIN,			//sin(...)
	COS,			//cos(...)
	TAN,			//tan(...)
	A_FX,			//exp(a,...)
	LOG,			//log(a,...)
	SQRT,			//(...)�̐��̕�����
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
