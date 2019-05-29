#include <stdlib.h>
#include <string.h>
#include "differential.h"

/*()外のch1とch2を文字列sのstartから検索し、添え字を返す*/
int search_char(const char s[], char ch1, char ch2, int start)
{
	int par;

	/*()外の与えられた文字を検索*/
	while(1){
		if(s[start] == ch1 || s[start] == ch2){
			/*'('のほうが多ければpar>0 *//* ')'のほうが多ければpar<0*/
			par = 0;
			for(int j=start; j >= 0; j--){
				if(s[j] == '(')
					par++;
				else if(s[j] == ')')
					par--;
			}
			if(par == 0)						//もし文字が()の外なら
				return start;					//ループを抜ける
		}
		if(s[start] == '\0')					//NULL文字なら終了
			break;
		start++;
	}
	return -1;									//ch1もch2もなかった時
}

void init(char s[])
{
	for(int i=0; s[i] != '\0'; i++){
		s[i] = '\0';
	}
}

/*文字列の両端それぞれに対応する括弧があれば取り除く*/
void rid_brackets(char s[])
{
	/*両端に対応するかっこがあるかどうか判定*/
	if(s[0] == '('){
		int par = 1;
		int i;

		for(i=1; par != 0; i++){
			if(s[i] == '(')
				par++;
			else if(s[i] == ')')
				par--;
		}
		/*両端に対応するかっこがある場合*/
		if(s[i] == '\0'){
			/*一文字前に詰める*/
			for(int j=1; j < i; j++){
				s[j-1] = s[j];
			}
			/*余った部分を埋める*/
			s[i-2] = '\0';
			s[i-1] = '\0';
		}
	}
}

/*((abc))など、括弧が二重になっていた時ひと組を取り除く*/
/*三重以上の括弧にも対応*/
void rid_double_brackets(char s[])
{
	int par;							//括弧の収支を計算する変数
	int brac_open;						//外側の括弧の始まりの添え字
	int brac_close1, brac_close2;		//外側、内側の閉じ括弧の添え字
	int len_s = strlen(s);				//sの長さ

	for(brac_open = 0; brac_open < len_s; brac_open++){
		if(s[brac_open] == '(' && s[brac_open+1] == '('){
			/*外側の閉じ括弧を探す*/
			par = 2;
			for(brac_close1=brac_open+2; par > 0 && s[brac_close1]; brac_close1++){
				if(s[brac_close1] == '(')
					par++;
				else if(s[brac_close1] == ')')
					par--;
			}
			brac_close1--;
			/*内側の閉じ括弧を探す*/
			par = 1;
			for(brac_close2=brac_open+2; par > 0 && s[brac_close2]; brac_close2++){
				if(s[brac_close2] == '(')
					par++;
				else if(s[brac_close2] == ')')
					par--;
			}
			brac_close2--;
			
			/*括弧が重なっていた時*/
			/*内側の括弧を取り除く*/
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

/*括弧の数が合っているかチェック*/
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
