/*線形分解
 *{af(x)+bg(x)}' → af'(x)+bg'(x)
 *実際は
 *(...)+(...)+... → a[(...)]+b[(...)]+...
 *定数項はc[]
 */

#include <string.h>
#include <ctype.h>
#include "differential.h"

void linear_apart(const char prob1[], char prob2[])
{
	int i = 0;						//prob2に焼き終わった+-までの文字数
	int coef;						//係数の添え字(coefficient:係数)
	int d_num = 0;					//[...]の個数
	int pm_at;						//検索した+-の添え字
	int len_prob1 = strlen(prob1);	//prob1の長さ

	/*式がマイナスで始まっていた時*/
	if(prob1[0] == '-'){
		prob2[0] = '-';
		i++;
	}
	
	while(1){
		if(prob1[i] == '1' && prob1[i+1] == '/')					//prob1が1/(...)の時[1/(...)]とする
			coef = i;
		else
			for(coef = i; isdigit(prob1[coef]); coef++)				//どこまでが係数か調べる
				prob2[coef + 2*d_num] = prob1[coef];				//係数をprob2にコピー

		prob2[2*d_num + coef] = '[';								//微分する式の開始

		pm_at = search_char(prob1, '+', '-', coef);					//'+''-'の検索

		if (pm_at < 0){												//'+''-'がなかった時
			for(i=coef; i < len_prob1; i++)
				prob2[i + 2*d_num + 1] = prob1[i];					//最後までをコピー
		}
		else{														//'+-'があった時
			for(i=coef; i<pm_at; i++)
				prob2[i + 2*d_num + 1] = prob1[i];					//+か-までをコピー
		}
		
		prob2[i + 2*d_num + 1] = ']';								//微分する式の終了
		
		if(i == len_prob1)											//+-がなければ
			break;													//終了

		prob2[i + 2*d_num + 2] = prob1[pm_at];						//+-の代入

		d_num++;
		i++;
	}
	rid_const(prob2);
}

/*定数項を取り除く*/
void rid_const(char prob2[])
{
	int i;														//[の場所
	int j;														//係数の桁数
	
	/*prob2の先頭が[]だったとき*/
	/*エラーなのでprob2を[error]とする*/
	if(prob2[0] == '[' && prob2[1] == ']'){
		init(prob2);
		strcpy(prob2, "[error]");
	}
	else{
		for(i = 0; prob2[i] != '\0'; i++){
			if(prob2[i] == '[' && prob2[i+1] == ']'){			//a[]の時
				if(prob2[i-1] == '+' || prob2[i-1] == '-'){
					init(prob2);
					strcpy(prob2, "[error]");
					return;
				}
				for(j = 0; isdigit(prob2[i-j-1]) != 0; j++){		//どこまでが係数か調べる
					if(j == i-1)									//prob1の添え字i-j-1が負にならないように
						break;
				}
				for(int k = i+2; prob2[k-j-3] != '\0'; k++){	//+-a[]の分だけ文字列をずらす
					prob2[k-j-3] = prob2[k];
				}
				i -= j+3;										//文字列をずらした分補正
			}
		}

		/*最初が定数項だった場合+a[...]...となるので+を消去*/
		if(prob2[0] == '+'){
			for(int k=1; prob2[k-1] != '\0'; k++){
				prob2[k-1] = prob2[k];
			}
		}
	}
}

/*線形分解すべき関数か判断*/
int islinear_apart(const char prob1[])
{
	int pm_at = search_char(prob1, '+', '-', 0);

	/*括弧外に'+'や'-'がある場合は原則実行*/
	if (pm_at >= 0) {
		/*ただし、...^-533などの場合は例外*/
		if (pm_at > 0)
			if (prob1[pm_at - 1] == '^')
				return 0;

		return 1;
	}

	/*prob1の先頭が数字の場合は原則実行*/
	if (isdigit(prob1[0])) {
		/*ただし、1/...の場合は例外*/
		if (prob1[0] == '1' && prob1[1] == '/')
			return 0;
		
		return 1;
	}

	return 0;
}
