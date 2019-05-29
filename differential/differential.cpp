#include <string.h>
#include <ctype.h>
#include "differential.h"

int main(void)
{
	char prob[500] = {};	//入力された文字列全体を格納
	char prob1[400] = {};	//[]内から抽出された文字列を格納
	char prob2[400] = {};	//prob1を微分した結果を格納
	Func func;				//どの基本関数なのか
	int fail;				//ミスがあれば、1
	int fin;				//完全に微分し終われば0

	while(1){
		/*初期化*/
		fin = 1;
		fail = 0;
		init(prob);

		printf("微分する数式を入力してください。(hを入力でヘルプを表示)\n");
		scanf("%400[^\n]", prob);
		scanf("%*[^\n]");			//入力バッファに余った文字を取り出しておく
		scanf("%*c");				//改行文字を取り出しておく
		
		/*hが入力された時ヘルプを表示*/
		if(prob[0] == 'h' && prob[1] == '\0'){
			help();
			continue;
		}

		/*入力された式のエラーチェック*/
		switch (standardize(prob)) {
		case 0:		break;					//エラーなし
		case ERR1:	printf("エラー:全角文字が含まれています。\n\n");			continue;
		case ERR2:	printf("エラー:\")\"が足りません。\n\n");					continue;
		case ERR3:	printf("エラー:\"(\"が足りません。\n\n");					continue;
		case ERR4:	printf("エラー:指数が不正です。\n\n");						continue;
		case ERR5:	printf("エラー:底が不正です。\n\n");						continue;
		case ERR6:	printf("エラー:定数同士は計算してください。\n\n");			continue;
		case ERR7:	printf("エラー:対数微分法はできません。\n\n");				continue;
		case ERR8:	printf("エラー:括弧の位置が不正です。\n\n");				continue;
		case ERR9:	printf("エラー:底を入力してください。\n\n");				continue;
		case ERR10:	printf("エラー:底に0や1は使えません。\n\n");				continue;
		case ERR11:	printf("エラー:底には数字のみを使用してください。\n\n");	continue;
		case ERR12:	printf("エラー:指数に0は指定できません。\n\n");				continue;
		default:	printf("!想定外のエラー!\n\n");								continue;
		}

		printf("%s\n", prob);

		/*微分演算子[]を文字列の最初と最後につける*/
		prob[strlen(prob)+1] = ']';
		for(int i = strlen(prob)-1; i >= 0; i--)
			prob[i+1] = prob[i];
		prob[0] = '[';

		while(fin){
			printf("%s\n", prob);
			init(prob1);
			init(prob2);
			
			/*[]内を抽出し、prob1に格納*/
			search_brackets(prob, prob1);

			/*線形分解できるならする*/
			if(islinear_apart(prob1)){
				linear_apart(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			/*積の微分ができるならする*/
			else if(search_char(prob1, '*', ' ', 0) >= 0){
				product(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			/*商の微分ができるならする*/
			else if(search_char(prob1, '/', ' ', 0) >= 0){
				quotient(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			
			/*微分する式が定数だった時(probが0だけの時)*/
			if(prob[0] == '0' && prob[1] == '\0')
				break;
			
			func = judge_basic_func(prob1);			//どの基本関数か判断

			if(func == ERR){						//基本関数でないなら入力ミス
				fail = 1;
				break;
			}
			else{
				differentiate(prob1, prob2, func);	//基本関数を微分
				brackets_link(prob, prob2);
			}

			/*完全に微分し終わったか判断*/
			for(int i=0; prob[i] != '[' && prob[i] != ']'; i++)
				if(prob[i] == '\0'){
					fin = 0;
					break;
				}
		}
		
		exp_convert(prob);							//exp(○,□) → ○^□
		
		rid_double_brackets(prob);					//二重カッコを取る
		rid_brackets(prob);							//全体のカッコを取る
		
		if(fail == 1)
			printf("微分に失敗しました。\n\n");		//エラー
		else
			printf("\nこれを微分すると、\n%s\nです。\n\n", prob);
	}
	return 0;
}
