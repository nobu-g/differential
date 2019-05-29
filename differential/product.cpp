/*�ς̔������s��*/

#include <string.h>
#include "differential.h"

void product(const char prob[], char prob1[])
{
	int par = 0;
	const int len_prob = strlen(prob);	//prob�̕�����
	int ast_num = 0;					//()�O��'*'�̐�
	int rev = 0;						//�Y�����␳
	
	/*()�O��*�̌�������*/
	for(int i = 0; i <= len_prob; i++){
		if(prob[i]=='*'){
			/*'('�̂ق����������par>0 *//* ')'�̂ق����������par<0*/
			par = 0;
			for(int j=i; j >= 0; j--){
				if(prob[j] == '(')
					par++;
				else if(prob[j] == ')')
					par--;
			}
			if(par==0)
				ast_num++;
		}
	}

	for(int j=0; j < ast_num+1; j++){
		int start = 0;
		int ast1, ast2;					//j,j+1�ڂ�'*'�̓Y����
		/* ...*[...]*...
		     ast1  ast2  */
		
		/*j�ڂ�'*'�̓Y�����𓾂�*/
		if(j == 0)
			ast1 = -1;
		else{
			for(int k=0; k < j; k++){
				ast1 = search_char(prob, '*', ' ', start);
				start = ast1 + 1;
			}
		}
		/*j+1�ڂ�'*'�̓Y�����𓾂�*/
		if(j == ast_num)
			ast2 = len_prob;
		else
			ast2 = search_char(prob, '*', ' ', start);

		/*�ς̔�����̕������prob1�Ɋi�[*/
		for(int k=0; k <= ast1; k++){
			prob1[k+rev] = prob[k];
		}
		
		prob1[ast1+1+rev] = '[';
		
		for(int k = ast1+1; k <= ast2-1; k++){
			prob1[k+1+rev] = prob[k];				//'['�����������v���X1
		}

		prob1[ast2+1+rev] = ']';
		
		for(int k = ast2; prob[k] != '\0'; k++){
			prob1[k+2+rev] = prob[k];				//'[]'�����������v���X2
		}

		if(j != ast_num)
			prob1[len_prob+2+rev] = '+';
		
		rev = strlen(prob1);						//�Y�����␳
	}
}
