#include <stdlib.h>
#include <string.h>
#include "differential.h"

/*()�O��ch1��ch2�𕶎���s��start���猟�����A�Y������Ԃ�*/
int search_char(const char s[], char ch1, char ch2, int start)
{
	int par;

	/*()�O�̗^����ꂽ����������*/
	while(1){
		if(s[start] == ch1 || s[start] == ch2){
			/*'('�̂ق����������par>0 *//* ')'�̂ق����������par<0*/
			par = 0;
			for(int j=start; j >= 0; j--){
				if(s[j] == '(')
					par++;
				else if(s[j] == ')')
					par--;
			}
			if(par == 0)						//����������()�̊O�Ȃ�
				return start;					//���[�v�𔲂���
		}
		if(s[start] == '\0')					//NULL�����Ȃ�I��
			break;
		start++;
	}
	return -1;									//ch1��ch2���Ȃ�������
}

void init(char s[])
{
	for(int i=0; s[i] != '\0'; i++){
		s[i] = '\0';
	}
}

/*������̗��[���ꂼ��ɑΉ����銇�ʂ�����Ύ�菜��*/
void rid_brackets(char s[])
{
	/*���[�ɑΉ����邩���������邩�ǂ�������*/
	if(s[0] == '('){
		int par = 1;
		int i;

		for(i=1; par != 0; i++){
			if(s[i] == '(')
				par++;
			else if(s[i] == ')')
				par--;
		}
		/*���[�ɑΉ����邩����������ꍇ*/
		if(s[i] == '\0'){
			/*�ꕶ���O�ɋl�߂�*/
			for(int j=1; j < i; j++){
				s[j-1] = s[j];
			}
			/*�]���������𖄂߂�*/
			s[i-2] = '\0';
			s[i-1] = '\0';
		}
	}
}

/*((abc))�ȂǁA���ʂ���d�ɂȂ��Ă������ЂƑg����菜��*/
/*�O�d�ȏ�̊��ʂɂ��Ή�*/
void rid_double_brackets(char s[])
{
	int par;							//���ʂ̎��x���v�Z����ϐ�
	int brac_open;						//�O���̊��ʂ̎n�܂�̓Y����
	int brac_close1, brac_close2;		//�O���A�����̕����ʂ̓Y����
	int len_s = strlen(s);				//s�̒���

	for(brac_open = 0; brac_open < len_s; brac_open++){
		if(s[brac_open] == '(' && s[brac_open+1] == '('){
			/*�O���̕����ʂ�T��*/
			par = 2;
			for(brac_close1=brac_open+2; par > 0 && s[brac_close1]; brac_close1++){
				if(s[brac_close1] == '(')
					par++;
				else if(s[brac_close1] == ')')
					par--;
			}
			brac_close1--;
			/*�����̕����ʂ�T��*/
			par = 1;
			for(brac_close2=brac_open+2; par > 0 && s[brac_close2]; brac_close2++){
				if(s[brac_close2] == '(')
					par++;
				else if(s[brac_close2] == ')')
					par--;
			}
			brac_close2--;
			
			/*���ʂ��d�Ȃ��Ă�����*/
			/*�����̊��ʂ���菜��*/
			if(brac_close1 == brac_close2+1){
				for(int i=brac_open+2; i <= brac_close1-2; i++)
					s[i-1] = s[i];
				for(int i=brac_close1; i <= len_s + 1; i++)
					s[i-2] = s[i];
				brac_open--;
			}
		}
	}
}

/*���ʂ̐��������Ă��邩�`�F�b�N*/
int brac_check(char s[])
{
	int par = 0;

	for(int i=0; s[i] != '\0'; i++)
		if(s[i] == '(')
			par++;
		else if(s[i] == ')')
			par--;

	return par;
}
