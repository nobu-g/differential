/*積の微分を行う*/

#include <string.h>
#include "differential.h"

void product(const char prob[], char prob1[])
{
	int par = 0;
	const int len_prob = strlen(prob);	//probの文字数
	int ast_num = 0;					//()外の'*'の数
	int rev = 0;						//添え字補正
	
	/*()外の*の個数を検索*/
	for(int i = 0; i <= len_prob; i++){
		if(prob[i]=='*'){
			/*'('のほうが多ければpar>0 *//* ')'のほうが多ければpar<0*/
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
		int ast1, ast2;					//j,j+1個目の'*'の添え字
		/* ...*[...]*...
		     ast1  ast2  */
		
		/*j個目の'*'の添え字を得る*/
		if(j == 0)
			ast1 = -1;
		else{
			for(int k=0; k < j; k++){
				ast1 = search_char(prob, '*', ' ', start);
				start = ast1 + 1;
			}
		}
		/*j+1個目の'*'の添え字を得る*/
		if(j == ast_num)
			ast2 = len_prob;
		else
			ast2 = search_char(prob, '*', ' ', start);

		/*積の微分後の文字列をprob1に格納*/
		for(int k=0; k <= ast1; k++){
			prob1[k+rev] = prob[k];
		}
		
		prob1[ast1+1+rev] = '[';
		
		for(int k = ast1+1; k <= ast2-1; k++){
			prob1[k+1+rev] = prob[k];				//'['が入った分プラス1
		}

		prob1[ast2+1+rev] = ']';
		
		for(int k = ast2; prob[k] != '\0'; k++){
			prob1[k+2+rev] = prob[k];				//'[]'が入った分プラス2
		}

		if(j != ast_num)
			prob1[len_prob+2+rev] = '+';
		
		rev = strlen(prob1);						//添え字補正
	}
}
