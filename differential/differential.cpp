#include <string.h>
#include <ctype.h>
#include "differential.h"

int main(void)
{
	char prob[500] = {};	//���͂��ꂽ������S�̂��i�[
	char prob1[400] = {};	//[]�����璊�o���ꂽ��������i�[
	char prob2[400] = {};	//prob1������������ʂ��i�[
	Func func;				//�ǂ̊�{�֐��Ȃ̂�
	int fail;				//�~�X������΁A1
	int fin;				//���S�ɔ������I����0

	while(1){
		/*������*/
		fin = 1;
		fail = 0;
		init(prob);

		printf("�������鐔������͂��Ă��������B(h����͂Ńw���v��\��)\n");
		scanf("%400[^\n]", prob);
		scanf("%*[^\n]");			//���̓o�b�t�@�ɗ]�������������o���Ă���
		scanf("%*c");				//���s���������o���Ă���
		
		/*h�����͂��ꂽ���w���v��\��*/
		if(prob[0] == 'h' && prob[1] == '\0'){
			help();
			continue;
		}

		/*���͂��ꂽ���̃G���[�`�F�b�N*/
		switch (standardize(prob)) {
		case 0:		break;					//�G���[�Ȃ�
		case ERR1:	printf("�G���[:�S�p�������܂܂�Ă��܂��B\n\n");			continue;
		case ERR2:	printf("�G���[:\")\"������܂���B\n\n");					continue;
		case ERR3:	printf("�G���[:\"(\"������܂���B\n\n");					continue;
		case ERR4:	printf("�G���[:�w�����s���ł��B\n\n");						continue;
		case ERR5:	printf("�G���[:�ꂪ�s���ł��B\n\n");						continue;
		case ERR6:	printf("�G���[:�萔���m�͌v�Z���Ă��������B\n\n");			continue;
		case ERR7:	printf("�G���[:�ΐ������@�͂ł��܂���B\n\n");				continue;
		case ERR8:	printf("�G���[:���ʂ̈ʒu���s���ł��B\n\n");				continue;
		case ERR9:	printf("�G���[:�����͂��Ă��������B\n\n");				continue;
		case ERR10:	printf("�G���[:���0��1�͎g���܂���B\n\n");				continue;
		case ERR11:	printf("�G���[:��ɂ͐����݂̂��g�p���Ă��������B\n\n");	continue;
		case ERR12:	printf("�G���[:�w����0�͎w��ł��܂���B\n\n");				continue;
		default:	printf("!�z��O�̃G���[!\n\n");								continue;
		}

		printf("%s\n", prob);

		/*�������Z�q[]�𕶎���̍ŏ��ƍŌ�ɂ���*/
		prob[strlen(prob)+1] = ']';
		for(int i = strlen(prob)-1; i >= 0; i--)
			prob[i+1] = prob[i];
		prob[0] = '[';

		while(fin){
			printf("%s\n", prob);
			init(prob1);
			init(prob2);
			
			/*[]���𒊏o���Aprob1�Ɋi�[*/
			search_brackets(prob, prob1);

			/*���`�����ł���Ȃ炷��*/
			if(islinear_apart(prob1)){
				linear_apart(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			/*�ς̔������ł���Ȃ炷��*/
			else if(search_char(prob1, '*', ' ', 0) >= 0){
				product(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			/*���̔������ł���Ȃ炷��*/
			else if(search_char(prob1, '/', ' ', 0) >= 0){
				quotient(prob1, prob2);
				brackets_link(prob, prob2);
				continue;
			}
			
			/*�������鎮���萔��������(prob��0�����̎�)*/
			if(prob[0] == '0' && prob[1] == '\0')
				break;
			
			func = judge_basic_func(prob1);			//�ǂ̊�{�֐������f

			if(func == ERR){						//��{�֐��łȂ��Ȃ���̓~�X
				fail = 1;
				break;
			}
			else{
				differentiate(prob1, prob2, func);	//��{�֐������
				brackets_link(prob, prob2);
			}

			/*���S�ɔ������I����������f*/
			for(int i=0; prob[i] != '[' && prob[i] != ']'; i++)
				if(prob[i] == '\0'){
					fin = 0;
					break;
				}
		}
		
		exp_convert(prob);							//exp(��,��) �� ��^��
		
		rid_double_brackets(prob);					//��d�J�b�R�����
		rid_brackets(prob);							//�S�̂̃J�b�R�����
		
		if(fail == 1)
			printf("�����Ɏ��s���܂����B\n\n");		//�G���[
		else
			printf("\n������������ƁA\n%s\n�ł��B\n\n", prob);
	}
	return 0;
}
