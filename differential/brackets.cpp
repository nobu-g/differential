/*�������镔��[...]�𒊏o*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "differential.h"

/*[]���𒊏o���Aprob1�Ɋi�[*/
void search_brackets(const char prob[], char prob1[])
{
	int i, j;

	for(i = 0  ; prob[i] != '[' && prob[i]; i++){}		//'['�̌���
	for(j = i+1; prob[j] != ']' && prob[j]; j++){}		//']'�̌���

	for(int k=0; k < j-i-1; k++){						//'['��']'�̊Ԃ̕�����̃R�s�[
		prob1[k] = prob[k+i+1];
	}
	
	rid_brackets(prob1);								//[(...)]�̂Ƃ�()����菜��

	/*prob1�̐擪��'+'����n�܂��Ă������A+������*/
	if(prob1[0] == '+')
		for(int i=0; i < strlen(prob1); i++)
			prob1[i] = prob1[i+1];
}

/*[]�̂Ȃ��ɔ���(���`����)�v�Z��̕������������*/
void brackets_link(char prob[], const char prob2[])
{
	int len_prob = strlen(prob);
	int len_prob2 = strlen(prob2);
	int brac1;	
	int brac2;
	int i;
	int par = 1;

	/*   brac1   brac2   len_prob	*/
	/*     ��      ��      ��		*/
	/*.....[.......].......\0		*/
	
	for(brac1 = 0; prob[brac1] != '[' && prob[brac1]; brac1++);		//'['�̌���
	for(brac2 = 0; prob[brac2] != ']' && prob[brac2]; brac2++);		//']'�̌���

	/*brac1��0�̎�prob�̓Y������-1�ɂȂ�̂�h��*/
	if(brac1 != 0){
		/*([...])�̌`�̎�()����菜��*/
		if(prob[brac1-1] == '(' && prob[brac2+1] == ')'){
			for(int i = brac1; i <= brac2; i++){
				prob[i-1] = prob[i];
			}
			for(int i = brac2+2; i <= len_prob + 1; i++){
				prob[i-2] = prob[i];
			}
			brac1--;					//[�̈ʒu�̕␳
			brac2--;					//]�̈ʒu�̕␳
			len_prob -= 2;				//prob�̒�����2����	
		}
	}

	/*[(...)]�̌`�̎�()����菜��*/
	if (prob[brac1 + 1] == '(' && prob[brac2 - 1] == ')') {
		for (i = brac1 + 2; par > 0 && prob[i]; i++) {
			if (prob[i] == '(')
				par++;
			else if (prob[i] == ')')
				par--;
		}
		if (i == brac2) {
			for (int i = brac1 + 2; i < brac2 - 1; i++)
				prob[i - 1] = prob[i];
			for (int i = brac2; i <= len_prob + 1; i++) 
				prob[i - 2] = prob[i];

			brac2 -= 2;					//[�̈ʒu�̕␳
			len_prob -= 2;				//prob�̒�����2����
		}
	}
		
	/*���̌�̏�����prob�̓Y������-1�ɂȂ�Ȃ����߂�brac1��0����1�ɂ���*/
	if(brac1 == 0){
		for(int i=len_prob-1; i >= 0; i--)
			prob[i+1] = prob[i];
		prob[0] = '$';
		brac1++;
		brac2++;
		len_prob++;
	}

	/*[x]�܂���[�萔]�̎��A[]���Ə���*/
	if(prob2[0] == '\0'){
		/*prob��...*[x]...��������*/
		if(prob[brac1-1] == '*'){
			for(int i = brac2+1; prob[i-4] != '\0'; i++){
				prob[i + brac1 - brac2 - 2] = prob[i];
			}
		}
		/*prob��...[x]*...��������*/
		else if(prob[brac2+1] == '*'){
			for(int i = brac2+2; prob[i-4] != '\0'; i++){
				prob[i + brac1 - brac2 - 2] = prob[i];
			}
		}
		/*prob��...4[x]...�Ȃǂ̎�*/
		else if(isdigit(prob[brac1-1]) != 0){
			/*[x]��S����菜��*/
			for(int i = brac2+1; prob[i-3] != '\0'; i++){
				prob[i + brac1 - brac2 - 1] = prob[i];
			}
		}
		/*prob��...+[x]-...�Ȃǂ̎�*/
		else if(prob[brac1-1] == '+' || prob[brac1-1] == '-' ||
			   prob[brac2+1] == '+' || prob[brac2+1] == '-'   ){
			prob[brac1] = '1';
			for(int i = brac2+1; prob[i-2] != '\0'; i++){
				prob[i + brac1 - brac2] = prob[i];
			}
		}
		/*prob��[x]��������[x]/(...)�̎�*/
		else if(prob[brac1+1] == 'x' || prob[brac2+1] == '/'){
			prob[brac1] = '1';
			for(int i = brac2+1; prob[i-2] != '\0'; i++){
				prob[i + brac1 - brac2] = prob[i];
			}
		}
		/*prob��[�萔]�̎�*/
		else{
			init(prob);
			prob[0] = '0';
		}
	}
	else{
		char *s;

		/*[]��()�ɕς���*/
		prob[brac1] = '(';
		prob[brac2] = ')';

		s = (char *)calloc(len_prob-brac2+1, sizeof(char));
		
		/*']'(")")����prob�̍Ō�܂ł�s�Ɋi�[*/
		for(int i = brac2; i <= len_prob; i++){
			s[i-brac2] = prob[i];
		}

		for(int i=0; i <= len_prob2;i++){
			prob[i+brac1+1] = prob2[i];
		}
		for(int i=brac1+len_prob2+2; prob[i] != '\0'; i++)
			prob[i] = '\0';

		strcat(prob, s);
		free(s);
	}

	if(prob[0] == '$')
		for(int i=1; prob[i-1] != '\0'; i++)
			prob[i-1] = prob[i];
}
