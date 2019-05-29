/*�����֐��̔���*/

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
		/*�������Ȃ�*/
		break;
	}
}

/*cos(...)*[...]�����*/
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
/*-sin(...)*[...]�����*/
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
/*[...]/((cos(...))^2)�����*/
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
/*(a^(...))*log(a)*[...]�����(a<0�̂Ƃ���a�Ɋ��ʂ�����)*/
void dif_a_fx(const char prob[], char prob1[], int len_prob)
{
	int com_at = 4;
	int par;

	/*�R���}�̏ꏊ������(com_at���Y������)
	������Ȃ������Ƃ���com_at��-1�ɂ��ďI��*/
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

	/*e^(...)�̎�*/
	/*(e^(...))*[...]�����*/
	if(com_at == -1){
		strcpy(prob1, "(e^");
		for(int i=3; i < len_prob; i++)
			prob1[i] = prob[i];
		strcat(prob1, ")*[");
		for(int i=4; i+1 < len_prob; i++)
			prob1[i+len_prob-1] = prob[i];
		prob1[len_prob*2-2] = ']';
	}
	/*a<0�̎�*/
	/*((a)^(...))*log(a)*[...]�����*/
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
	/*(a^(...))*log(a)*[...]�����*/
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
/*[...]/((...)*log(a))�����*/
void dif_log(const char prob[], char prob1[], int len_prob)
{
	int com_at = 4;
	int par;

	/*�R���}�̏ꏊ������(com_at���Y������)
	������Ȃ������Ƃ���com_at��-1�ɂ��ďI��*/
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

	/*���R�ΐ��̎�*/
	/*[...]/(...)�����*/
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
	/*[...]/((...)*log(a))�����*/
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
/*[...]/(2sqrt(...))�����*/
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
/*(a(...)^(a-1))*[...]�����*/
void dif_fx_a(const char prob[], char prob1[], int len_prob)
{
	int caret = search_char(prob, '^', ' ', 0);	//^�̓Y����
	int digit;				//a�̌���
	int head;				//�w���𑖍�����Ƃ��̓Y����
	int len;				//prob1�̕�����

	/*(...)^1�̎��A[(...)]�����*/
	if (prob[caret + 1] == '1' && prob[caret + 2] == '\0') {
		prob1[0] = '[';
		for (int i = 0; i < caret; i++)
			prob1[i + 1] = prob[i];
		prob1[caret + 1] = ']';
	}
	else {
		prob1[0] = '(';
		/*(...)^-1�̎�*/
		if (prob[caret + 1] == '-' && prob[caret + 2] == '1' && prob[caret + 3] == '\0') {
			prob1[1] = '-';

			/*()�����R�s�[*/
			for (int i = 0; i < caret; i++)
				prob1[i + 2] = prob[i];

			strcat(prob1, "^-2");
		}
		else {
			/*�w�����R�s�[���ČW����*/
			for (digit = caret + 1; digit < len_prob; digit++) {
				prob1[digit - caret] = prob[digit];
			}
			digit -= caret + 1;

			/*()�����R�s�[*/
			for (int i = 0; i < caret; i++)
				prob1[i + digit + 1] = prob[i];

			/*(...)^2�łȂ���*/
			/*�w������a-1�����*/
			if (prob[caret + 1] != '2' || prob[caret + 2] != '\0') {
				prob1[caret + digit + 1] = '^';

				/*�w�������̐��̎�*/
				if (prob[caret + 1] == '-') {
					prob1[caret + digit + 2] = '-';				//���̐�����1�������Ă����̐��ɂȂ�
					
					for (head = caret + digit; prob[head] == '9'; head--);

					/*�w����-99999�Ȃǂ�������*/
					if (prob[head] == '-') {
						prob1[caret + digit + 3] = '1';
						for (int i = caret + digit + 4; i < caret + 2*digit + 3; i++)
							prob1[i] = '0';
					}
					/*�w����-345999��855�Ȃǂ�������*/
					else {
						for (int i = caret + 2; i < head; i++)
							prob1[i + digit + 1] = prob[i];
						prob1[digit + head + 1] = prob[head] + 1;
						for (int i = digit + head + 2; i <= caret + 2 * digit + 1; i++)
							prob1[i] = '0';
					}
				}
				/*�w�������̐��̎�*/
				else {
					for (head = caret + digit; prob[head] == '0'; head--);

					/*�w����10000�Ȃǂ�������*/
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


//�{�c�̃v���O����
///*(a(...)^(a-1))*[...]�����*/
//void dif_fx_a(const char prob[], char prob1[], int len_prob)
//{
//	int caret = search_char(prob, '^', ' ', 0);	//^�̓Y����
//	int digit;				//a�̌���
//	int len;				//prob1�̕�����
//	char nstr1[11] = {};	//����a���i�[
//	char nstr2[11] = {};	//����a-1���i�[
//
//	prob1[0] = '(';
//	for (digit = caret + 1; digit < len_prob; digit++) {
//		prob1[digit - caret] = prob[digit];
//	}
//	digit -= caret + 1;
//	/*()�����R�s�[*/
//	for (int i = 0; i < caret; i++) {
//		prob1[i + digit + 1] = prob[i];
//	}
//
//	/*(...)^2�łȂ���*/
//	if (prob[caret + 1] != '2' || prob[caret + 2] != '\0') {
//		strcat(prob1, "^");
//
//		/*���l���o�R���邱�Ƃ�a-1�̕���������*/
//		for (int i = caret + 1; i < len_prob; i++) {
//			nstr1[i - caret - 1] = prob[i];
//		}
//		sprintf(nstr2, "%d", atoi(nstr1) - 1);	//�����񁨐��l��������
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
