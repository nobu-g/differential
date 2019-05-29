/*���`����
 *{af(x)+bg(x)}' �� af'(x)+bg'(x)
 *���ۂ�
 *(...)+(...)+... �� a[(...)]+b[(...)]+...
 *�萔����c[]
 */

#include <string.h>
#include <ctype.h>
#include "differential.h"

void linear_apart(const char prob1[], char prob2[])
{
	int i = 0;						//prob2�ɏĂ��I�����+-�܂ł̕�����
	int coef;						//�W���̓Y����(coefficient:�W��)
	int d_num = 0;					//[...]�̌�
	int pm_at;						//��������+-�̓Y����
	int len_prob1 = strlen(prob1);	//prob1�̒���

	/*�����}�C�i�X�Ŏn�܂��Ă�����*/
	if(prob1[0] == '-'){
		prob2[0] = '-';
		i++;
	}
	
	while(1){
		if(prob1[i] == '1' && prob1[i+1] == '/')					//prob1��1/(...)�̎�[1/(...)]�Ƃ���
			coef = i;
		else
			for(coef = i; isdigit(prob1[coef]); coef++)				//�ǂ��܂ł��W�������ׂ�
				prob2[coef + 2*d_num] = prob1[coef];				//�W����prob2�ɃR�s�[

		prob2[2*d_num + coef] = '[';								//�������鎮�̊J�n

		pm_at = search_char(prob1, '+', '-', coef);					//'+''-'�̌���

		if (pm_at < 0){												//'+''-'���Ȃ�������
			for(i=coef; i < len_prob1; i++)
				prob2[i + 2*d_num + 1] = prob1[i];					//�Ō�܂ł��R�s�[
		}
		else{														//'+-'����������
			for(i=coef; i<pm_at; i++)
				prob2[i + 2*d_num + 1] = prob1[i];					//+��-�܂ł��R�s�[
		}
		
		prob2[i + 2*d_num + 1] = ']';								//�������鎮�̏I��
		
		if(i == len_prob1)											//+-���Ȃ����
			break;													//�I��

		prob2[i + 2*d_num + 2] = prob1[pm_at];						//+-�̑��

		d_num++;
		i++;
	}
	rid_const(prob2);
}

/*�萔������菜��*/
void rid_const(char prob2[])
{
	int i;														//[�̏ꏊ
	int j;														//�W���̌���
	
	/*prob2�̐擪��[]�������Ƃ�*/
	/*�G���[�Ȃ̂�prob2��[error]�Ƃ���*/
	if(prob2[0] == '[' && prob2[1] == ']'){
		init(prob2);
		strcpy(prob2, "[error]");
	}
	else{
		for(i = 0; prob2[i] != '\0'; i++){
			if(prob2[i] == '[' && prob2[i+1] == ']'){			//a[]�̎�
				if(prob2[i-1] == '+' || prob2[i-1] == '-'){
					init(prob2);
					strcpy(prob2, "[error]");
					return;
				}
				for(j = 0; isdigit(prob2[i-j-1]) != 0; j++){		//�ǂ��܂ł��W�������ׂ�
					if(j == i-1)									//prob1�̓Y����i-j-1�����ɂȂ�Ȃ��悤��
						break;
				}
				for(int k = i+2; prob2[k-j-3] != '\0'; k++){	//+-a[]�̕���������������炷
					prob2[k-j-3] = prob2[k];
				}
				i -= j+3;										//����������炵�����␳
			}
		}

		/*�ŏ����萔���������ꍇ+a[...]...�ƂȂ�̂�+������*/
		if(prob2[0] == '+'){
			for(int k=1; prob2[k-1] != '\0'; k++){
				prob2[k-1] = prob2[k];
			}
		}
	}
}

/*���`�������ׂ��֐������f*/
int islinear_apart(const char prob1[])
{
	int pm_at = search_char(prob1, '+', '-', 0);

	/*���ʊO��'+'��'-'������ꍇ�͌������s*/
	if (pm_at >= 0) {
		/*�������A...^-533�Ȃǂ̏ꍇ�͗�O*/
		if (pm_at > 0)
			if (prob1[pm_at - 1] == '^')
				return 0;

		return 1;
	}

	/*prob1�̐擪�������̏ꍇ�͌������s*/
	if (isdigit(prob1[0])) {
		/*�������A1/...�̏ꍇ�͗�O*/
		if (prob1[0] == '1' && prob1[1] == '/')
			return 0;
		
		return 1;
	}

	return 0;
}
