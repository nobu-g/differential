/*商の微分*/

#include <string.h>
#include <stdlib.h>
#include "differential.h"

void quotient(const char prob[], char prob1[])
{
	int slash = search_char(prob, '/', ' ', 0);
	int len_prob = strlen(prob);
	int len_s1;
	char *s1;
	char *s2;

	/*1/f(x)の微分*/
	/*-[...]/((...)^2)を作る*/
	/*probが/(...)の時*/
	if(slash == 0){
		strcpy(prob1, "-[");
		for(int i=1; i < len_prob; i++)
			prob1[i+1] = prob[i];
		strcat(prob1, "]/((");
		for(int i=1; i < len_prob; i++)
			prob1[i+len_prob+4] = prob[i];
		strcat(prob1, ")^2)");
	}
	/*probが1/(...)の時*/
	else if(prob[0] == '1' && slash == 1){
		strcpy(prob1, "-[");
		for(int i=2; i < len_prob; i++)
			prob1[i] = prob[i];
		strcat(prob1, "]/((");
		for(int i=2; i < len_prob; i++)
			prob1[i+len_prob+2] = prob[i];
		strcat(prob1, ")^2)");
	}

	/*g(x)/f(x)の微分*/
	/*([g(x)]*f(x)-g(x)*[f(x)])/(f(x)^2)を作る*/
	else if(slash > 0){
		s1 = (char *)calloc(slash+1, sizeof(char));
		s2 = (char *)calloc(len_prob - slash, sizeof(char));

		strncpy(s1, prob, slash);					//s1にg(x)をコピー
		s1[slash] = '\0';							//NULL文字の付加

		len_s1 = strlen(s1);

		for(int k=len_s1; k < len_prob; k++){		//s2にf(x)をコピー
			s2[k - len_s1] = prob[k+1];
		}
		s2[len_prob - len_s1 - 1] = '\0';			//NULL文字の付加

		/*prob1に代入*/
		strcpy(prob1, "([");
		strcat(prob1, s1);
		strcat(prob1, "]*");
		strcat(prob1, s2);
		strcat(prob1, "-");
		strcat(prob1, s1);
		strcat(prob1, "*[");
		strcat(prob1, s2);
		strcat(prob1, "])/(");
		strcat(prob1, s2);
		strcat(prob1, "^2)");

		free(s1);
		free(s2);
	}
}
