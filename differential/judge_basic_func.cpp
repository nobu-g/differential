/*�^����ꂽ������{�֐�(sin��log�Ȃ�)������
 *�\����Ă��邩�𔻒�
 *��{�֐�������1
 *��{�֐��̐ςȂǁ�0
 */
#include <ctype.h>
#include <string.h>
#include "differential.h"

Func judge_basic_func(const char prob[])
{
	int len_prob = strlen(prob);
	int at;

	switch(prob[0]){
	case 's' :
		/*sin�̎�*/
		if(prob[1]=='i' && prob[2]=='n' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return SIN;
		}
		/*sqrt�̎�*/
		if(prob[1]=='q' && prob[2]=='r' && prob[3]=='t' && prob[4]=='('){
			if(judge_basic(prob, 5) == 1)
				return SQRT;
		}
		break;
	
	case 'c' :
		/*cos�̎�*/
		if(prob[1]=='o' && prob[2]=='s' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return COS;
		}
		break;

	case 't' :
		/*tan�̎�*/
		if(prob[1]=='a' && prob[2]=='n' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return TAN;
		}
		break;

	case 'l' :
		/*log�̎�*/
		if(prob[1]=='o' && prob[2]=='g' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return LOG;
		}
		break;

	case '(' :
		/*(f(x))^n�̎�*/
		at = search_char(prob, '^', ' ', 0);
		if(at > 0)
			return FX_A;
		break;

	case 'x' :
		/*x�̎�*/
		if(prob[1] == '\0')
			return X;
		/*x^a�̎�*/
		if(prob[1] == '^')
			return FX_A;
		break;

	case 'e' :
		/*�w���֐��̎�*/
		if(prob[1]=='x' && prob[2]=='p' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return A_FX;
		}
		break;

	default :
		break;
	}
	return ERR;
}

/*���������������ɂ��邩�`�F�b�N*/
int judge_basic(const char s[], int f_num)
{
	int par = 1;
	int i;

	/*'('�̂ق����������par>0 *//* ')'�̂ق����������par<0*/
	for(i = f_num; par > 0 && s[i]; i++){
		if(s[i]=='(')
			par++;
		else if(s[i]==')')
			par--;
	}
	if(s[i]=='\0')			//���������̎��̕�����'\0'�Ȃ�
		return 1;			//1��Ԃ�
	else
		return 0;
}
