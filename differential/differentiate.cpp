/*合成関数の微分*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "differential.h"

void differentiate(const char prob[], char prob1[], Func func)
{
	int len_prob = strlen(prob);

	switch(func){
	case SIN :
		dif_sin (prob, prob1, len_prob);
		break;
	case COS :
		dif_cos (prob, prob1, len_prob);
		break;
	case TAN :
		dif_tan (prob, prob1, len_prob);
		break;
	case A_FX:
		dif_a_fx(prob, prob1, len_prob);
		break;
	case LOG :
		dif_log (prob, prob1, len_prob);
		break;
	case SQRT:
		dif_sqrt(prob, prob1, len_prob);
		break;
	case FX_A:
		dif_fx_a(prob, prob1, len_prob);
		break;
	case X   :
		/*何もしない*/
		break;
	}
}

/*cos(...)*[...]を作る*/
void dif_sin(const char prob[], char prob1[], int len_prob)
{
	strcpy(prob1, "cos");
	for(int i=3; i < len_prob; i++){
		prob1[i] = prob[i];
	}
	strcat(prob1, "*[");
	for(int i=4; i < len_prob-1; i++){
		prob1[len_prob+i-2] = prob[i];
	}
	strcat(prob1, "]");
}
/*-sin(...)*[...]を作る*/
void dif_cos(const char prob[], char prob1[], int len_prob)
{
	strcpy(prob1, "-sin");
	for(int i=3; i < len_prob; i++){
		prob1[i+1] = prob[i];
	}
	strcat(prob1, "*[");
	for(int i=4; i < len_prob-1; i++){
		prob1[len_prob+i-1] = prob[i];
	}
	strcat(prob1, "]");
}
/*[...]/((cos(...))^2)を作る*/
void dif_tan(const char prob[], char prob1[], int len_prob)
{
	strcpy(prob1, "[");
	for(int i=4; i < len_prob-1; i++){
		prob1[i-3] = prob[i];
	}
	strcat(prob1, "]/((cos");
	for(int i=3; i < len_prob; i++){
		prob1[i+len_prob] = prob[i];
	}
	strcat(prob1, ")^2)");
}
/*(a^(...))*log(a)*[...]を作る(a<0のときはaに括弧をつける)*/
void dif_a_fx(const char prob[], char prob1[], int len_prob)
{
	int com_at = 4;
	int par;

	/*コンマの場所を検索(com_atが添え字に)
	見つからなかったときはcom_atを-1にして終了*/
	while (1) {
		if (prob[com_at] == ',') {
			par = 0;
			for (int i = com_at; i > 0; i--) {
				if (prob[i] == '(')
					par++;
				else if (prob[i] == ')')
					par--;
			}
			if (par == 1)
				break;
		}
		if (com_at == len_prob - 1) {
			com_at = -1;
			break;
		}
		com_at++;
	}

	/*e^(...)の時*/
	/*(e^(...))*[...]を作る*/
	if(com_at == -1){
		strcpy(prob1, "(e^");
		for(int i=3; i < len_prob; i++)
			prob1[i] = prob[i];
		strcat(prob1, ")*[");
		for(int i=4; i+1 < len_prob; i++)
			prob1[i+len_prob-1] = prob[i];
		prob1[len_prob*2-2] = ']';
	}
	/*a<0の時*/
	/*((a)^(...))*log(a)*[...]を作る*/
	else if (prob[4] == '-') {
		strcpy(prob1, "((");
		for (int i = 4; i < com_at; i++)
			prob1[i - 2] = prob[i];
		strcat(prob1, ")^(");
		for (int i = com_at + 1; i < len_prob - 1; i++)
			prob1[i] = prob[i];
		strcat(prob1, "))*log(");
		for (int i = 4; i < com_at; i++)
			prob1[i + len_prob + 2] = prob[i];
		strcat(prob1, ")*[");
		for (int i = com_at + 1; i < len_prob - 1; i++)
			prob1[i + len_prob + 4] = prob[i];
		prob1[len_prob * 2 + 3] = ']';
	}
	/*(a^(...))*log(a)*[...]を作る*/
	else{
		prob1[0] = '(';
		for(int i=4; i < com_at; i++)
			prob1[i-3] = prob[i];
		strcat(prob1, "^(");
		for(int i=com_at+1; i < len_prob-1; i++)
			prob1[i-2] = prob[i];
		strcat(prob1, "))*log(");
		for(int i=4; i < com_at; i++)
			prob1[i+len_prob] = prob[i];
		strcat(prob1, ")*[");
		for(int i=com_at+1; i < len_prob-1; i++)
			prob1[i+len_prob+2] = prob[i];
		prob1[len_prob*2+1] = ']';
	}
}
/*[...]/((...)*log(a))を作る*/
void dif_log(const char prob[], char prob1[], int len_prob)
{
	int com_at = 4;
	int par;

	/*コンマの場所を検索(com_atが添え字に)
	見つからなかったときはcom_atを-1にして終了*/
	while (1) {
		if (prob[com_at] == ',') {
			par = 0;
			for (int i = com_at; i > 0; i--) {
				if (prob[i] == '(')
					par++;
				else if (prob[i] == ')')
					par--;
			}
			if (par == 1)
				break;
		}
		if (com_at == len_prob - 1) {
			com_at = -1;
			break;
		}
		com_at++;
	}

	/*自然対数の時*/
	/*[...]/(...)を作る*/
	if(com_at == -1){
		strcpy(prob1, "[");
		for(int i=4; i < len_prob-1; i++){
		prob1[i-3] = prob[i];
		}
		strcat(prob1, "]/");
		for(int i=3; i < len_prob; i++){
			prob1[i+len_prob-5] = prob[i];
		}
	}
	/*[...]/((...)*log(a))を作る*/
	else{
		strcpy(prob1, "[");
		for(int i = com_at+1; i < len_prob-1; i++){
			prob1[i-com_at] = prob[i];
		}
		strcat(prob1, "]/((");
		for(int i = com_at+1; i < len_prob-1; i++){
			prob1[i+len_prob-2*com_at+2] = prob[i];
		}
		strcat(prob1, ")*log(");
		for(int i=4; i < com_at; i++){
			prob1[i+2*len_prob-2*com_at+3] = prob[i];
		}
		strcat(prob1, "))");
	}
}
/*[...]/(2sqrt(...))を作る*/
void dif_sqrt(const char prob[], char prob1[], int len_prob)
{
	strcpy(prob1, "[");
	for(int i=5; i < len_prob-1; i++){
		prob1[i-4] = prob[i];
	}
	strcat(prob1, "]/(2");
	strcat(prob1, prob);
	strcat(prob1, ")");
}
/*(a(...)^(a-1))*[...]を作る*/
void dif_fx_a(const char prob[], char prob1[], int len_prob)
{
	int caret = search_char(prob, '^', ' ', 0);	//^の添え字
	int digit;				//aの桁数
	int head;				//指数を走査するときの添え字
	int len;				//prob1の文字数

	/*(...)^1の時、[(...)]を作る*/
	if (prob[caret + 1] == '1' && prob[caret + 2] == '\0') {
		prob1[0] = '[';
		for (int i = 0; i < caret; i++)
			prob1[i + 1] = prob[i];
		prob1[caret + 1] = ']';
	}
	else {
		prob1[0] = '(';
		/*(...)^-1の時*/
		if (prob[caret + 1] == '-' && prob[caret + 2] == '1' && prob[caret + 3] == '\0') {
			prob1[1] = '-';

			/*()内をコピー*/
			for (int i = 0; i < caret; i++)
				prob1[i + 2] = prob[i];

			strcat(prob1, "^-2");
		}
		else {
			/*指数をコピーして係数に*/
			for (digit = caret + 1; digit < len_prob; digit++) {
				prob1[digit - caret] = prob[digit];
			}
			digit -= caret + 1;

			/*()内をコピー*/
			for (int i = 0; i < caret; i++)
				prob1[i + digit + 1] = prob[i];

			/*(...)^2でない時*/
			/*指数部分a-1を作る*/
			if (prob[caret + 1] != '2' || prob[caret + 2] != '\0') {
				prob1[caret + digit + 1] = '^';

				/*指数が負の数の時*/
				if (prob[caret + 1] == '-') {
					prob1[caret + digit + 2] = '-';				//負の数から1を引いても負の数になる
					
					for (head = caret + digit; prob[head] == '9'; head--);

					/*指数が-99999などだった時*/
					if (prob[head] == '-') {
						prob1[caret + digit + 3] = '1';
						for (int i = caret + digit + 4; i < caret + 2*digit + 3; i++)
							prob1[i] = '0';
					}
					/*指数が-345999や855などだった時*/
					else {
						for (int i = caret + 2; i < head; i++)
							prob1[i + digit + 1] = prob[i];
						prob1[digit + head + 1] = prob[head] + 1;
						for (int i = digit + head + 2; i <= caret + 2 * digit + 1; i++)
							prob1[i] = '0';
					}
				}
				/*指数が正の数の時*/
				else {
					for (head = caret + digit; prob[head] == '0'; head--);

					/*指数が10000などだった時*/
					if (prob[head] == '1' && head == caret + 1) {
						for (int i = caret + digit + 2; i <= caret + 2 * digit; i++)
							prob1[i] = '9';
					}
					else{
					for (int i = caret + 1; i < head; i++)
						prob1[i + digit + 1] = prob[i];
					prob1[digit + head + 1] = prob[head] - 1;
					for (int i = digit + head + 2; i <= caret + 2 * digit + 1; i++)
						prob1[i] = '9';
					}
				}
			}
		}

		strcat(prob1, ")*[");
		len = strlen(prob1);
		for (int i = 0; i < caret; i++) {
			prob1[i + len] = prob[i];
		}
		prob1[caret + len] = ']';
	}
}


//ボツのプログラム
///*(a(...)^(a-1))*[...]を作る*/
//void dif_fx_a(const char prob[], char prob1[], int len_prob)
//{
//	int caret = search_char(prob, '^', ' ', 0);	//^の添え字
//	int digit;				//aの桁数
//	int len;				//prob1の文字数
//	char nstr1[11] = {};	//数字aを格納
//	char nstr2[11] = {};	//数字a-1を格納
//
//	prob1[0] = '(';
//	for (digit = caret + 1; digit < len_prob; digit++) {
//		prob1[digit - caret] = prob[digit];
//	}
//	digit -= caret + 1;
//	/*()内をコピー*/
//	for (int i = 0; i < caret; i++) {
//		prob1[i + digit + 1] = prob[i];
//	}
//
//	/*(...)^2でない時*/
//	if (prob[caret + 1] != '2' || prob[caret + 2] != '\0') {
//		strcat(prob1, "^");
//
//		/*数値を経由することでa-1の文字列を作る*/
//		for (int i = caret + 1; i < len_prob; i++) {
//			nstr1[i - caret - 1] = prob[i];
//		}
//		sprintf(nstr2, "%d", atoi(nstr1) - 1);	//文字列→数値→文字列
//		strcat(prob1, nstr2);
//	}
//
//	strcat(prob1, ")*[");
//	len = strlen(prob1);
//	for (int i = 0; i < caret; i++) {
//		prob1[i + len] = prob[i];
//	}
//	strcat(prob1, "]");
//}
