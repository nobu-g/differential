/*���̔���*/

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

	/*1/f(x)�̔���*/
	/*-[...]/((...)^2)�����*/
	/*prob��/(...)�̎�*/
	if(slash == 0){
		strcpy(prob1, "-[");
		for(int i=1; i < len_prob; i++)
			prob1[i+1] = prob[i];
		strcat(prob1, "]/((");
		for(int i=1; i < len_prob; i++)
			prob1[i+len_prob+4] = prob[i];
		strcat(prob1, ")^2)");
	}
	/*prob��1/(...)�̎�*/
	else if(prob[0] == '1' && slash == 1){
		strcpy(prob1, "-[");
		for(int i=2; i < len_prob; i++)
			prob1[i] = prob[i];
		strcat(prob1, "]/((");
		for(int i=2; i < len_prob; i++)
			prob1[i+len_prob+2] = prob[i];
		strcat(prob1, ")^2)");
	}

	/*g(x)/f(x)�̔���*/
	/*([g(x)]*f(x)-g(x)*[f(x)])/(f(x)^2)�����*/
	else if(slash > 0){
		s1 = (char *)calloc(slash+1, sizeof(char));
		s2 = (char *)calloc(len_prob - slash, sizeof(char));

		strncpy(s1, prob, slash);					//s1��g(x)���R�s�[
		s1[slash] = '\0';							//NULL�����̕t��

		len_s1 = strlen(s1);

		for(int k=len_s1; k < len_prob; k++){		//s2��f(x)���R�s�[
			s2[k - len_s1] = prob[k+1];
		}
		s2[len_prob - len_s1 - 1] = '\0';			//NULL�����̕t��

		/*prob1�ɑ��*/
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
