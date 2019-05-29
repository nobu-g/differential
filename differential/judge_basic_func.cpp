/*与えられた式が基本関数(sinやlogなど)だけで
 *表されているかを判定
 *基本関数だけ→1
 *基本関数の積など→0
 */
#include <ctype.h>
#include <string.h>
#include "differential.h"

Func judge_basic_func(const char prob[])
{
	int len_prob = strlen(prob);
	int at;

	switch(prob[0]){
	case 's' :
		/*sinの時*/
		if(prob[1]=='i' && prob[2]=='n' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return SIN;
		}
		/*sqrtの時*/
		if(prob[1]=='q' && prob[2]=='r' && prob[3]=='t' && prob[4]=='('){
			if(judge_basic(prob, 5) == 1)
				return SQRT;
		}
		break;
	
	case 'c' :
		/*cosの時*/
		if(prob[1]=='o' && prob[2]=='s' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return COS;
		}
		break;

	case 't' :
		/*tanの時*/
		if(prob[1]=='a' && prob[2]=='n' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return TAN;
		}
		break;

	case 'l' :
		/*logの時*/
		if(prob[1]=='o' && prob[2]=='g' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return LOG;
		}
		break;

	case '(' :
		/*(f(x))^nの時*/
		at = search_char(prob, '^', ' ', 0);
		if(at > 0)
			return FX_A;
		break;

	case 'x' :
		/*xの時*/
		if(prob[1] == '\0')
			return X;
		/*x^aの時*/
		if(prob[1] == '^')
			return FX_A;
		break;

	case 'e' :
		/*指数関数の時*/
		if(prob[1]=='x' && prob[2]=='p' && prob[3]=='('){
			if(judge_basic(prob, 4) == 1)
				return A_FX;
		}
		break;

	default :
		break;
	}
	return ERR;
}

/*閉じかっこが文末にあるかチェック*/
int judge_basic(const char s[], int f_num)
{
	int par = 1;
	int i;

	/*'('のほうが多ければpar>0 *//* ')'のほうが多ければpar<0*/
	for(i = f_num; par > 0 && s[i]; i++){
		if(s[i]=='(')
			par++;
		else if(s[i]==')')
			par--;
	}
	if(s[i]=='\0')			//閉じかっこの次の文字が'\0'なら
		return 1;			//1を返す
	else
		return 0;
}
