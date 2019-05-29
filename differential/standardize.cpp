#include <string.h>
#include <ctype.h>
#include <mbstring.h>
#include "differential.h"

/*入力された数式のエラーチェックをし、処理しやすいように規格化する*/
int standardize(char prob[])
{
	int flagxf, flagxl;			//かたまりがxの式かどうかのフラグ(前半の式と後半の式)
	int block1, block2;			//かたまり(sin(x)やxや(...)など)の前半の始めと後半の終わりを示す
	int par;					//括弧()の収支を計算するための変数

	/*全角文字がないか調べる*/
	for (int i = 0; prob[i]; i++) {
		if (_ismbblead(prob[i])) {
			i++;
			return ERR1;
		}
	}

	/*大文字を小文字にし、スペースを消去する*/
	for(int i=0; prob[i]; i++){
		if(isupper(prob[i]))
			prob[i] = tolower(prob[i]);
		if(isspace(prob[i])){
			for(int j=i+1; prob[j-1] != '\0'; j++)
				prob[j-1] = prob[j];

			i--;
		}
	}

	/*係数などの値が0から始まっていた時、その0を取り除く*/
	for (int i = 0; prob[i]; i++) {
		if (prob[i] == '0' && isdigit(prob[i+1])) {
			if (i == 0)
				for (int j = 1; prob[j - 1]; j++)
					prob[j - 1] = prob[j];
			else if (isdigit(prob[i - 1]) == 0)
				for (int j = i + 1; prob[j - 1]; j++)
					prob[j - 1] = prob[j];
			else
				continue;
			i--;
		}
	}
	
	/*括弧の個数をチェック*/
	if (brac_check(prob) > 0)
		return ERR2;
	else if (brac_check(prob) < 0)
		return ERR3;

	/*二重括弧があれば、そのうち一組を取り除く*/
	rid_double_brackets(prob);

	/*"^"の用いられた指数関数を全てexpの形に変換する*/
	
	/*	<方法>
	 *	1.後ろから、^を探す
	 *	2.見つかったら、^の後のかたまりの最後の添え字を得る
	 *	3.^の前のかたまりの最初の添え字を得る
	 *	4.両方xなし→エラー:数値を計算してください
	 *	5.両方xの式→エラー:対数微分法はできません
	 *	6.前半xの式→expに置き換える
	 *	7.後半xの式→() ^ ()のかたまり全体を()で囲む
	 *	8.ヘッドを前半の式の先頭に持ってくる
	 *	以下繰り返し
	 */

	for (int i = strlen(prob) - 1; i > 0; i--) {
		/*"^"が見つかった時*/
		if (prob[i] == '^') {
			/*後ろのかたまりを探す(その最後の添え字をblock2に)*/

			/*...^(...)の場合*/
			if (prob[i + 1] == '(') {
				/*閉じ括弧を探す*/
				par = 1;
				for (block2 = i + 2; par > 0 && prob[block2]; block2++) {
					if (prob[block2] == '(')
						par++;
					else if (prob[block2] == ')')
						par--;
				}
				if (par > 0)
					return ERR8;		//エラー
				block2--;				//for文でインクリメントしすぎた分を補正
				if (block2 == i + 2)
					return ERR4;		//エラー

				/*かたまりがxの式かどうか調べる*/
				flagxl = 0;
				for (int j = i + 2; j < block2; j++)
					if (prob[j] == 'x') {
						flagxl = 1;
						break;
					}
			}
			/*^+123や、^-45などの場合*/
			else if ((prob[i+1] == '+' || prob[i+1] == '-') && isdigit(prob[i+2])){
				/*数字を最後までたどる*/
				for (block2 = i + 3; isdigit(prob[block2]); block2++);
				block2--;				//for文でインクリメントしすぎた分を補正

				flagxl = 0;
			}
			/*^345などの場合*/
			else if (isdigit(prob[i + 1])) {
				/*数字を最後までたどる*/
				for (block2 = i + 2; isdigit(prob[block2]); block2++);
				block2--;

				flagxl = 0;
			}
			/*^xの場合*/
			else if (prob[i + 1] == 'x') {
				block2 = i + 1;
				flagxl = 1;
			}
			/*^-xの場合*/
			else if (prob[i + 1] == '-' && prob[i + 2] == 'x') {
				block2 = i + 2;
				flagxl = 1;
			}
			/*^sin(...)などの場合*/
			else {
				par = 1;

				/*'('の検索*/
				for (block2 = i + 1; prob[block2] != '(' && prob[block2]; block2++);

				/*')'の検索*/
				for (block2++; par > 0 && prob[block2]; block2++) {
					if (prob[block2] == '(')
						par++;
					else if (prob[block2] == ')')
						par--;
				}
				if (par > 0)			//括弧がなかった場合
					return ERR4;		//エラー:指数が不正です
				block2--;				//forループでインクリメントしすぎた分を補正

				/*かたまりがxの式かどうか調べる*/
				flagxl = 0;
				for (int j = i + 2; j < block2; j++)
					if (prob[j] == 'x') {
						flagxl = 1;
						break;
					}
				if (flagxl == 0)
					return ERR4;
			}

			/*^の前のかたまりを探す(その最初の添え字をblock1に)*/

			/*(...)^...の場合*/
			if (prob[i - 1] == ')') {
				/*括弧の始めを探す*/
				if (i == 1)				//例外処理
					return ERR8;

				par = -1;
				for (block1 = i - 2; par < 0 && block1 >= 0; block1--) {
					if (prob[block1] == '(')
						par++;
					else if (prob[block1] == ')')
						par--;
				}
				if (par < 0)
					return ERR8;		//エラー
				block1++;				//for文でデクリメントしすぎた分を補正

				/*かたまりがxの式かどうか調べる*/
				flagxf = 0;
				for (int j = i - 2; j > block1; j--)
					if (prob[j] == 'x') {
						flagxf = 1;
						break;
					}
			}
			/*356^...などの場合*/
			else if (isdigit(prob[i - 1])) {
				/*数字の始まりを探す*/
				for (block1 = i - 2; block1 >= 0; block1--)
					if (isdigit(prob[block1]) == 0)
						break;
				block1++;

				flagxf = 0;
			}
			else if (prob[i - 1] == 'x') {
				block1 = i - 1;
				flagxf = 1; 
			}
			else
				return ERR5;

			if (flagxf == 0 && flagxl == 0)
				return ERR6;
			else if (flagxf == 1 && flagxl == 1)
				return ERR7;
			else if (flagxf == 1 && flagxl == 0) {
				/*後半が数字のみかチェックし、そうならば括弧や+を取り除く*/
				if (prob[i + 1] == '(') {
					if (prob[i + 2] == '+' && isdigit(prob[i + 3])) {
						/*全て数字かチェック*/
						for (int j = i + 4; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*括弧と+を取り除く*/
						for (int j = i + 3; j < block2; j++)
							prob[j - 2] = prob[j];
						for (int j = block2 + 1; prob[j - 3]; j++)
							prob[j - 3] = prob[j];

						block2 -= 3;					//block2の調整
					}
					else if (prob[i + 2] == '-' && isdigit(prob[i + 3])) {
						/*全て数字かチェック*/
						for (int j = i + 4; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*括弧を取り除く*/
						for (int j = i + 2; j < block2; j++)
							prob[j - 1] = prob[j];
						for (int j = block2 + 1; prob[j - 2]; j++)
							prob[j - 2] = prob[j];

						block2 -= 2;					//block2の調整
					}
					else {
						/*全て数字かチェック*/
						for (int j = i + 2; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*括弧を取り除く*/
						for (int j = i + 2; j < block2; j++)
							prob[j - 1] = prob[j];
						for (int j = block2 + 1; prob[j - 2]; j++)
							prob[j - 2] = prob[j];

						block2 -= 2;					//block2の調整
					}
				}
				/*^345や^-4636や^+88などの時*/
				else {
					if (prob[i + 1] == '+') {
						/*+を取り除く*/
						for (int j = i + 2; prob[j - 1]; j++)
							prob[j - 1] = prob[j];

						block2--;					//block2の調整
					}
				}

				/*^0または^-0の場合エラー*/
				if (prob[i + 1] == '0' || (prob[i + 1] == '-' && prob[i + 2] == '0'))
					return ERR12;

				/*まだ囲まれていないなら...^...のかたまり全体を()で囲む*/
				if (block1 > 0)
					if (prob[block1 - 1] == '(' && prob[block2 + 1] == ')')
						continue;
				for (int i = strlen(prob) - 1; i > block2; i--)
					prob[i + 2] = prob[i];
				prob[block2 + 2] = ')';
				for (int i = block2; i >= block1; i--)
					prob[i + 1] = prob[i];
				prob[block1] = '(';
			}
			else if (flagxf == 0 && flagxl == 1) {
				/*expに置き換える*/

				/* block1   i  block2*/
				/* 　↓    ↓   ↓   */
				/* 　 (-123)^(...)   */
				/*       ↓変換      */
				/*  exp(-123,...)    */

				if (prob[i - 1] == ')' && prob[i + 1] == '(') {
					for (int j = strlen(prob) - 1; j > block2; j--)
						prob[j + 1] = prob[j];
					prob[block2 + 1] = ')';
					for (int j = block2 - 1; j > i + 1; j--)
						prob[j + 1] = prob[j];
					prob[i + 2] = ',';
					for (int j = i - 2; j > block1; j--)
						prob[j + 3] = prob[j];
				}
				else if (prob[i - 1] == ')') {
					for (int j = strlen(prob) - 1; j > block2; j--)
						prob[j + 3] = prob[j];
					prob[block2 + 3] = ')';
					for (int j = block2; j > i; j--)
						prob[j + 2] = prob[j];
					prob[i + 2] = ',';
					for (int j = i - 2; j > block1; j--)
						prob[j + 3] = prob[j];
				}
				else if (prob[i + 1] == '(') {
					for (int j = strlen(prob) - 1; j > block2; j--)
						prob[j + 3] = prob[j];
					prob[block2 + 3] = ')';
					for (int j = block2 + 1; j > i + 1; j--)
						prob[j + 3] = prob[j];
					prob[i + 4] = ',';
					for (int j = i - 1; j >= block1; j--)
						prob[j + 4] = prob[j];
				}
				else {
					for (int j = strlen(prob) - 1; j > block2; j--)
						prob[j + 5] = prob[j];
					prob[block2 + 5] = ')';
					for (int j = block2; j > i; j--)
						prob[j + 4] = prob[j];
					prob[i + 4] = ',';
					for (int j = i - 1; j >= block1; j--)
						prob[j + 4] = prob[j];
				}
				
				prob[block1 + 3] = '(';
				prob[block1 + 2] = 'p';
				prob[block1 + 1] = 'x';
				prob[block1] = 'e';
			}
			/*添え字iの調整*/
			if (flagxf == 0 && flagxl == 1)
				i = block1;
		}
	}
	
	/*コンマを含むexp,logの第一引数が存在するか、存在するなら数字のみであるかをチェックする。*/
	for (int i = 0; prob[i] != '\0'; i++) {
		int comma;							//コンマの添え字
		int digit;							//コンマの前の数字の桁数
 
		if ((prob[i] == 'e' && prob[i + 1] == 'x' && prob[i + 2] == 'p' && prob[i + 3] == '(') ||
			(prob[i] == 'l' && prob[i + 1] == 'o' && prob[i + 2] == 'g' && prob[i + 3] == '(')) {
			par = 1;
			int closebrac;			//exp,logの閉じ括弧の添え字

			/*exp,logの閉じ括弧を探す*/
			for (closebrac = i + 4; par > 0 && prob[closebrac]; closebrac++) {
				if (prob[closebrac] == '(')
					par++;
				else if (prob[closebrac] == ')')
					par--;
			}
			/*エラー:括弧の位置が不正*/
			if (par > 0)
				return ERR8;

			/*exp,logのコンマの位置を(あれば)探す*/
			for (comma = i + 4; comma < closebrac; comma++) {
				if (prob[comma] == ',') {
					/*par=0から、"exp("から","までの括弧の収支を計算*/
					for (int j = i + 4; j < comma; j++) {
						if (prob[j] == '(')
							par++;
						else if (prob[j] == ')')
							par--;
					}

					/*exp,logがコンマを含んでいなかった時*/
					/*※comma<0にはならない*/
					if (par > 0)
						break;
					/*コンマを含んでいた時、第一引数が存在するか、するなら数字のみか調べる*/
					else if (par == 0) {
						if (comma == i + 4)
							return ERR9;

						if (prob[comma - 1] == '0') {
							if (comma == i + 5 || (comma == i + 6 && (prob[comma - 2] == '+' || prob[comma - 2] == '-')))
								return ERR10;
						}
						else if (prob[comma - 1] == '1')
							if (comma == i + 5 || (comma == i + 6 && prob[comma - 2] == '+'))
								return ERR10;

						/*底の桁数を数える*/
						for (digit = comma - 1; isdigit(prob[digit]) && prob[digit]; digit--);
						/*底が数字だけかチェック*/
						if (digit == i + 3 || (digit == i+4 && (prob[digit] == '+' || prob[digit] == '-')))
							break;
						else
							return ERR11;
					}
				}
			}
			/*添え字調節*/
			if (par == 0)
				i = comma;
			else
				i += 3;
		}
	}

	return 0;
}

/*exp(abc,defg)を(abc^(defg))にする*/
void exp_convert(char prob[])
{
	//'\0'で埋めるのを忘れずに

	int com_at;								//コンマの添え字
	int brac_close;							//閉じ括弧の添え字
	int par;								//括弧の収支を計算する変数

	for(int i=0; prob[i] != '\0'; i++){
		if(prob[i] == 'e'){
			if(prob[i+1] == 'x' && prob[i+2] == 'p'&& prob[i+3] =='('){
				/*コンマの場所を検索(com_atが添え字に)
				  見つからなかったときはcom_atを-1にする*/
				if (prob[i + 4] == '+' || prob[i + 4] == '-') {
					for (com_at = i + 5; isdigit(prob[com_at]); com_at++);
					if (com_at == i + 5 || prob[com_at] != ',')
						com_at = -1;
				}
				else {
					for (com_at = i + 4; isdigit(prob[com_at]); com_at++);
					if (com_at == i + 4 || prob[com_at] != ',')
						com_at = -1;
				}

				/*閉じ括弧の場所を探す*/
				par = 1;
				for(brac_close = i+4; par > 0; brac_close++){
					if(prob[brac_close] == '(')
						par++;
					else if(prob[brac_close] == ')')
						par--;
				}
				brac_close--;

				/*exp(abc)などの時*/
				/*(e^(abc))を作る*/
				if(com_at == -1){
					prob[i  ] = '(';
					prob[i+1] = 'e';
					prob[i+2] = '^';
					for(int j=strlen(prob)-1; j > brac_close; j--)
						prob[j+1] = prob[j];
					prob[brac_close+1] = ')';

					i += 3;					//iの修正
				}
				/*底が負の数の時*/
				/*((abc)^(defg))を作る*/
				else if (prob[i + 4] == '-') {
					prob[i] = '(';
					prob[i + 1] = '(';
					for (int j = i + 4; j < com_at; j++)
						prob[j - 2] = prob[j];
					prob[com_at - 2] = ')';
					prob[com_at - 1] = '^';
					prob[com_at] = '(';
					for (int j = com_at + 1; j <= brac_close; j++)
						prob[j] = prob[j];
					for (int j = strlen(prob) - 1; j > brac_close; j--)
						prob[j + 1] = prob[j];
					prob[brac_close + 1] = ')';

					i = com_at;			//iの修正
				}
				/*exp(abc,defg)などの時*/
				/*(abc^(defg))を作る*/
				else{
					prob[i] = '(';
					for(int j=i+4; j < com_at; j++)
						prob[j-3] = prob[j];
					prob[com_at-3] = '^';
					prob[com_at-2] = '(';
					for(int j=com_at+1; j <= brac_close; j++)
						prob[j-2] = prob[j];
					prob[brac_close-1] = ')';
					for(int j=brac_close+1; prob[j-1] != '\0'; j++)
						prob[j-1] = prob[j];

					i = com_at-2;			//iの修正
				}
			}
		}
	}
}


/*ボツになった...^...→exp(...)変換のプログラム*/

///*123^(...) → exp(123,...)*/
//for(int i=strlen(prob)-1; i > 0; i--){
//	if(prob[i] == '^'){
//		for(digit1 = i+1; isdigit(prob[digit1]); digit1++);	//'^'の後の数字の桁数(有無)を調べる
//		digit1--;											//forループでインクリメントしすぎた分を補正

//		/*123^(...)などの時*/
//		if(digit1 == i){
//			
//			/*'^'の前の数字がどこまでか調べる*/
//			for(digit2 = i-1; isdigit(prob[digit2]) && digit2 != 0; digit2--);

//			/*digit2が数字の先頭を指すようにする*/
//			if(digit2 != 0 || isdigit(prob[0]) == 0)
//				digit2++;

//			/*'^'の後のかたまりがどこまでか調べる*/

//			/*123^xなどの時*/
//			if(prob[i+1] == 'x')
//				block = i+1;
//			/*123^(...)や123^sin(...)などの時*/
//			else{
//				par = 1;

//				/*'('の検索*/
//				for(block = i+1; prob[block] != '(' && prob[block]; block++);

//				/*')'の検索*/
//				for(block++; par > 0 && prob[block]; block++){
//					if(prob[block] == '(')
//						par++;
//					else if(prob[block] == ')')
//						par--;
//				}
//				if (par > 0)
//					return -1;

//				block--;		//forループでインクリメントしすぎた分を補正
//			}
//			
//			/*exp(123,(...))を作る*/

//			/* digit2 i  block */
//			/*  　↓ ↓   ↓   */
//			/*  　 123^(...)   */
//			/*      ↓変換     */
//			/*  exp(123,(...)) */

//			for(int j=strlen(prob)-1; j > block; j--)
//				prob[j+5] = prob[j];
//			prob[block+5] = ')';
//			for(int j=block; j > i; j--)
//				prob[j+4] = prob[j];
//			prob[i+4] = ',';
//			for(int j=i-1; j >= digit2; j--)
//				prob[j+4] = prob[j];
//			prob[digit2+3] = '(';
//			prob[digit2+2] = 'p';
//			prob[digit2+1] = 'x';
//			prob[digit2  ] = 'e';
//			
//			i = digit2;				//添え字補正
//		}
//		/*(...)^123などの時*/
//		else{
//			/*'^'の前のかたまりがどこまでか調べる*/

//			/*x^123などの時*/
//			if(prob[i-1] == 'x')
//				block = i-1;
//			/*(...)^123などの時*/
//			else if(prob[i-1] == ')'){
//				par = -1;

//				/*'('の検索*/
//				for(block = i-2; par < 0; block--){
//					if(prob[block] == '(')
//						par++;
//					else if(prob[block] == ')')
//						par--;
//				}
//				block++;
//			}
//			else
//				break;

//			/*45^(...)^123などの時*/
//			if(prob[block-1] == '^'){

//				/*前の'^'の左にある数字の先頭の添え字を調べる*/
//				for(digit3 = block-2; isdigit(prob[digit3]) != 0 && digit3 != 0; digit3--);
//				if(digit3 != 0 || isdigit(prob[0]) == 0)
//					digit3++;

//				/*exp(...,...)の形にする*/

//				/* digit3 block  i  digit1 */
//				/*  ↓     ↓    ↓    ↓   */
//				/*  ４５＾（...）＾１２３   */
//				/*         ↓変換           */
//				/*     exp(45,(...)^123)    */
//				
//				for(int j=strlen(prob)-1; j > digit1; j--)
//					prob[j+5] = prob[j];
//				prob[digit1+5] = ')';
//				for(int j=digit1; j >= block; j--)
//					prob[j+4] = prob[j];
//				prob[block+3] = ',';
//				for(int j=block-2; j >= digit3; j--)
//					prob[j+4] = prob[j];
//				prob[digit3+3] = '(';
//				prob[digit3+2] = 'p';
//				prob[digit3+1] = 'x';
//				prob[digit3  ] = 'e';

//				i = digit3;			//添え字補正
//			}
//		}
//	}
//}
