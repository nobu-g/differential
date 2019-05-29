/*微分する部分[...]を抽出*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "differential.h"

/*[]内を抽出し、prob1に格納*/
void search_brackets(const char prob[], char prob1[])
{
	int i, j;

	for(i = 0  ; prob[i] != '[' && prob[i]; i++){}		//'['の検索
	for(j = i+1; prob[j] != ']' && prob[j]; j++){}		//']'の検索

	for(int k=0; k < j-i-1; k++){						//'['と']'の間の文字列のコピー
		prob1[k] = prob[k+i+1];
	}
	
	rid_brackets(prob1);								//[(...)]のとき()を取り除く

	/*prob1の先頭が'+'から始まっていた時、+を除去*/
	if(prob1[0] == '+')
		for(int i=0; i < strlen(prob1); i++)
			prob1[i] = prob1[i+1];
}

/*[]のなかに微分(線形分解)計算後の文字列を代入する*/
void brackets_link(char prob[], const char prob2[])
{
	int len_prob = strlen(prob);
	int len_prob2 = strlen(prob2);
	int brac1;	
	int brac2;
	int i;
	int par = 1;

	/*   brac1   brac2   len_prob	*/
	/*     ↓      ↓      ↓		*/
	/*.....[.......].......\0		*/
	
	for(brac1 = 0; prob[brac1] != '[' && prob[brac1]; brac1++);		//'['の検索
	for(brac2 = 0; prob[brac2] != ']' && prob[brac2]; brac2++);		//']'の検索

	/*brac1が0の時probの添え字が-1になるのを防ぐ*/
	if(brac1 != 0){
		/*([...])の形の時()を取り除く*/
		if(prob[brac1-1] == '(' && prob[brac2+1] == ')'){
			for(int i = brac1; i <= brac2; i++){
				prob[i-1] = prob[i];
			}
			for(int i = brac2+2; i <= len_prob + 1; i++){
				prob[i-2] = prob[i];
			}
			brac1--;					//[の位置の補正
			brac2--;					//]の位置の補正
			len_prob -= 2;				//probの長さは2つ減る	
		}
	}

	/*[(...)]の形の時()を取り除く*/
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

			brac2 -= 2;					//[の位置の補正
			len_prob -= 2;				//probの長さは2つ減る
		}
	}
		
	/*この後の処理でprobの添え字が-1にならないためにbrac1を0から1にする*/
	if(brac1 == 0){
		for(int i=len_prob-1; i >= 0; i--)
			prob[i+1] = prob[i];
		prob[0] = '$';
		brac1++;
		brac2++;
		len_prob++;
	}

	/*[x]または[定数]の時、[]ごと消す*/
	if(prob2[0] == '\0'){
		/*probが...*[x]...だった時*/
		if(prob[brac1-1] == '*'){
			for(int i = brac2+1; prob[i-4] != '\0'; i++){
				prob[i + brac1 - brac2 - 2] = prob[i];
			}
		}
		/*probが...[x]*...だった時*/
		else if(prob[brac2+1] == '*'){
			for(int i = brac2+2; prob[i-4] != '\0'; i++){
				prob[i + brac1 - brac2 - 2] = prob[i];
			}
		}
		/*probが...4[x]...などの時*/
		else if(isdigit(prob[brac1-1]) != 0){
			/*[x]を全部取り除く*/
			for(int i = brac2+1; prob[i-3] != '\0'; i++){
				prob[i + brac1 - brac2 - 1] = prob[i];
			}
		}
		/*probが...+[x]-...などの時*/
		else if(prob[brac1-1] == '+' || prob[brac1-1] == '-' ||
			   prob[brac2+1] == '+' || prob[brac2+1] == '-'   ){
			prob[brac1] = '1';
			for(int i = brac2+1; prob[i-2] != '\0'; i++){
				prob[i + brac1 - brac2] = prob[i];
			}
		}
		/*probが[x]だけ又は[x]/(...)の時*/
		else if(prob[brac1+1] == 'x' || prob[brac2+1] == '/'){
			prob[brac1] = '1';
			for(int i = brac2+1; prob[i-2] != '\0'; i++){
				prob[i + brac1 - brac2] = prob[i];
			}
		}
		/*probが[定数]の時*/
		else{
			init(prob);
			prob[0] = '0';
		}
	}
	else{
		char *s;

		/*[]を()に変える*/
		prob[brac1] = '(';
		prob[brac2] = ')';

		s = (char *)calloc(len_prob-brac2+1, sizeof(char));
		
		/*']'(")")からprobの最後までをsに格納*/
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
