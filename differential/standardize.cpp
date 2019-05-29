#include <string.h>
#include <ctype.h>
#include <mbstring.h>
#include "differential.h"

/*���͂��ꂽ�����̃G���[�`�F�b�N�����A�������₷���悤�ɋK�i������*/
int standardize(char prob[])
{
	int flagxf, flagxl;			//�����܂肪x�̎����ǂ����̃t���O(�O���̎��ƌ㔼�̎�)
	int block1, block2;			//�����܂�(sin(x)��x��(...)�Ȃ�)�̑O���̎n�߂ƌ㔼�̏I��������
	int par;					//����()�̎��x���v�Z���邽�߂̕ϐ�

	/*�S�p�������Ȃ������ׂ�*/
	for (int i = 0; prob[i]; i++) {
		if (_ismbblead(prob[i])) {
			i++;
			return ERR1;
		}
	}

	/*�啶�����������ɂ��A�X�y�[�X����������*/
	for(int i=0; prob[i]; i++){
		if(isupper(prob[i]))
			prob[i] = tolower(prob[i]);
		if(isspace(prob[i])){
			for(int j=i+1; prob[j-1] != '\0'; j++)
				prob[j-1] = prob[j];

			i--;
		}
	}

	/*�W���Ȃǂ̒l��0����n�܂��Ă������A����0����菜��*/
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
	
	/*���ʂ̌����`�F�b�N*/
	if (brac_check(prob) > 0)
		return ERR2;
	else if (brac_check(prob) < 0)
		return ERR3;

	/*��d���ʂ�����΁A���̂�����g����菜��*/
	rid_double_brackets(prob);

	/*"^"�̗p����ꂽ�w���֐���S��exp�̌`�ɕϊ�����*/
	
	/*	<���@>
	 *	1.��납��A^��T��
	 *	2.����������A^�̌�̂����܂�̍Ō�̓Y�����𓾂�
	 *	3.^�̑O�̂����܂�̍ŏ��̓Y�����𓾂�
	 *	4.����x�Ȃ����G���[:���l���v�Z���Ă�������
	 *	5.����x�̎����G���[:�ΐ������@�͂ł��܂���
	 *	6.�O��x�̎���exp�ɒu��������
	 *	7.�㔼x�̎���() ^ ()�̂����܂�S�̂�()�ň͂�
	 *	8.�w�b�h��O���̎��̐擪�Ɏ����Ă���
	 *	�ȉ��J��Ԃ�
	 */

	for (int i = strlen(prob) - 1; i > 0; i--) {
		/*"^"������������*/
		if (prob[i] == '^') {
			/*���̂����܂��T��(���̍Ō�̓Y������block2��)*/

			/*...^(...)�̏ꍇ*/
			if (prob[i + 1] == '(') {
				/*�����ʂ�T��*/
				par = 1;
				for (block2 = i + 2; par > 0 && prob[block2]; block2++) {
					if (prob[block2] == '(')
						par++;
					else if (prob[block2] == ')')
						par--;
				}
				if (par > 0)
					return ERR8;		//�G���[
				block2--;				//for���ŃC���N�������g������������␳
				if (block2 == i + 2)
					return ERR4;		//�G���[

				/*�����܂肪x�̎����ǂ������ׂ�*/
				flagxl = 0;
				for (int j = i + 2; j < block2; j++)
					if (prob[j] == 'x') {
						flagxl = 1;
						break;
					}
			}
			/*^+123��A^-45�Ȃǂ̏ꍇ*/
			else if ((prob[i+1] == '+' || prob[i+1] == '-') && isdigit(prob[i+2])){
				/*�������Ō�܂ł��ǂ�*/
				for (block2 = i + 3; isdigit(prob[block2]); block2++);
				block2--;				//for���ŃC���N�������g������������␳

				flagxl = 0;
			}
			/*^345�Ȃǂ̏ꍇ*/
			else if (isdigit(prob[i + 1])) {
				/*�������Ō�܂ł��ǂ�*/
				for (block2 = i + 2; isdigit(prob[block2]); block2++);
				block2--;

				flagxl = 0;
			}
			/*^x�̏ꍇ*/
			else if (prob[i + 1] == 'x') {
				block2 = i + 1;
				flagxl = 1;
			}
			/*^-x�̏ꍇ*/
			else if (prob[i + 1] == '-' && prob[i + 2] == 'x') {
				block2 = i + 2;
				flagxl = 1;
			}
			/*^sin(...)�Ȃǂ̏ꍇ*/
			else {
				par = 1;

				/*'('�̌���*/
				for (block2 = i + 1; prob[block2] != '(' && prob[block2]; block2++);

				/*')'�̌���*/
				for (block2++; par > 0 && prob[block2]; block2++) {
					if (prob[block2] == '(')
						par++;
					else if (prob[block2] == ')')
						par--;
				}
				if (par > 0)			//���ʂ��Ȃ������ꍇ
					return ERR4;		//�G���[:�w�����s���ł�
				block2--;				//for���[�v�ŃC���N�������g������������␳

				/*�����܂肪x�̎����ǂ������ׂ�*/
				flagxl = 0;
				for (int j = i + 2; j < block2; j++)
					if (prob[j] == 'x') {
						flagxl = 1;
						break;
					}
				if (flagxl == 0)
					return ERR4;
			}

			/*^�̑O�̂����܂��T��(���̍ŏ��̓Y������block1��)*/

			/*(...)^...�̏ꍇ*/
			if (prob[i - 1] == ')') {
				/*���ʂ̎n�߂�T��*/
				if (i == 1)				//��O����
					return ERR8;

				par = -1;
				for (block1 = i - 2; par < 0 && block1 >= 0; block1--) {
					if (prob[block1] == '(')
						par++;
					else if (prob[block1] == ')')
						par--;
				}
				if (par < 0)
					return ERR8;		//�G���[
				block1++;				//for���Ńf�N�������g������������␳

				/*�����܂肪x�̎����ǂ������ׂ�*/
				flagxf = 0;
				for (int j = i - 2; j > block1; j--)
					if (prob[j] == 'x') {
						flagxf = 1;
						break;
					}
			}
			/*356^...�Ȃǂ̏ꍇ*/
			else if (isdigit(prob[i - 1])) {
				/*�����̎n�܂��T��*/
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
				/*�㔼�������݂̂��`�F�b�N���A�����Ȃ�Ί��ʂ�+����菜��*/
				if (prob[i + 1] == '(') {
					if (prob[i + 2] == '+' && isdigit(prob[i + 3])) {
						/*�S�Đ������`�F�b�N*/
						for (int j = i + 4; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*���ʂ�+����菜��*/
						for (int j = i + 3; j < block2; j++)
							prob[j - 2] = prob[j];
						for (int j = block2 + 1; prob[j - 3]; j++)
							prob[j - 3] = prob[j];

						block2 -= 3;					//block2�̒���
					}
					else if (prob[i + 2] == '-' && isdigit(prob[i + 3])) {
						/*�S�Đ������`�F�b�N*/
						for (int j = i + 4; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*���ʂ���菜��*/
						for (int j = i + 2; j < block2; j++)
							prob[j - 1] = prob[j];
						for (int j = block2 + 1; prob[j - 2]; j++)
							prob[j - 2] = prob[j];

						block2 -= 2;					//block2�̒���
					}
					else {
						/*�S�Đ������`�F�b�N*/
						for (int j = i + 2; j < block2; j++)
							if (isdigit(prob[j]) == 0)
								return ERR4;

						/*���ʂ���菜��*/
						for (int j = i + 2; j < block2; j++)
							prob[j - 1] = prob[j];
						for (int j = block2 + 1; prob[j - 2]; j++)
							prob[j - 2] = prob[j];

						block2 -= 2;					//block2�̒���
					}
				}
				/*^345��^-4636��^+88�Ȃǂ̎�*/
				else {
					if (prob[i + 1] == '+') {
						/*+����菜��*/
						for (int j = i + 2; prob[j - 1]; j++)
							prob[j - 1] = prob[j];

						block2--;					//block2�̒���
					}
				}

				/*^0�܂���^-0�̏ꍇ�G���[*/
				if (prob[i + 1] == '0' || (prob[i + 1] == '-' && prob[i + 2] == '0'))
					return ERR12;

				/*�܂��͂܂�Ă��Ȃ��Ȃ�...^...�̂����܂�S�̂�()�ň͂�*/
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
				/*exp�ɒu��������*/

				/* block1   i  block2*/
				/* �@��    ��   ��   */
				/* �@ (-123)^(...)   */
				/*       ���ϊ�      */
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
			/*�Y����i�̒���*/
			if (flagxf == 0 && flagxl == 1)
				i = block1;
		}
	}
	
	/*�R���}���܂�exp,log�̑����������݂��邩�A���݂���Ȃ琔���݂̂ł��邩���`�F�b�N����B*/
	for (int i = 0; prob[i] != '\0'; i++) {
		int comma;							//�R���}�̓Y����
		int digit;							//�R���}�̑O�̐����̌���
 
		if ((prob[i] == 'e' && prob[i + 1] == 'x' && prob[i + 2] == 'p' && prob[i + 3] == '(') ||
			(prob[i] == 'l' && prob[i + 1] == 'o' && prob[i + 2] == 'g' && prob[i + 3] == '(')) {
			par = 1;
			int closebrac;			//exp,log�̕����ʂ̓Y����

			/*exp,log�̕����ʂ�T��*/
			for (closebrac = i + 4; par > 0 && prob[closebrac]; closebrac++) {
				if (prob[closebrac] == '(')
					par++;
				else if (prob[closebrac] == ')')
					par--;
			}
			/*�G���[:���ʂ̈ʒu���s��*/
			if (par > 0)
				return ERR8;

			/*exp,log�̃R���}�̈ʒu��(�����)�T��*/
			for (comma = i + 4; comma < closebrac; comma++) {
				if (prob[comma] == ',') {
					/*par=0����A"exp("����","�܂ł̊��ʂ̎��x���v�Z*/
					for (int j = i + 4; j < comma; j++) {
						if (prob[j] == '(')
							par++;
						else if (prob[j] == ')')
							par--;
					}

					/*exp,log���R���}���܂�ł��Ȃ�������*/
					/*��comma<0�ɂ͂Ȃ�Ȃ�*/
					if (par > 0)
						break;
					/*�R���}���܂�ł������A�����������݂��邩�A����Ȃ琔���݂̂����ׂ�*/
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

						/*��̌����𐔂���*/
						for (digit = comma - 1; isdigit(prob[digit]) && prob[digit]; digit--);
						/*�ꂪ�����������`�F�b�N*/
						if (digit == i + 3 || (digit == i+4 && (prob[digit] == '+' || prob[digit] == '-')))
							break;
						else
							return ERR11;
					}
				}
			}
			/*�Y��������*/
			if (par == 0)
				i = comma;
			else
				i += 3;
		}
	}

	return 0;
}

/*exp(abc,defg)��(abc^(defg))�ɂ���*/
void exp_convert(char prob[])
{
	//'\0'�Ŗ��߂�̂�Y�ꂸ��

	int com_at;								//�R���}�̓Y����
	int brac_close;							//�����ʂ̓Y����
	int par;								//���ʂ̎��x���v�Z����ϐ�

	for(int i=0; prob[i] != '\0'; i++){
		if(prob[i] == 'e'){
			if(prob[i+1] == 'x' && prob[i+2] == 'p'&& prob[i+3] =='('){
				/*�R���}�̏ꏊ������(com_at���Y������)
				  ������Ȃ������Ƃ���com_at��-1�ɂ���*/
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

				/*�����ʂ̏ꏊ��T��*/
				par = 1;
				for(brac_close = i+4; par > 0; brac_close++){
					if(prob[brac_close] == '(')
						par++;
					else if(prob[brac_close] == ')')
						par--;
				}
				brac_close--;

				/*exp(abc)�Ȃǂ̎�*/
				/*(e^(abc))�����*/
				if(com_at == -1){
					prob[i  ] = '(';
					prob[i+1] = 'e';
					prob[i+2] = '^';
					for(int j=strlen(prob)-1; j > brac_close; j--)
						prob[j+1] = prob[j];
					prob[brac_close+1] = ')';

					i += 3;					//i�̏C��
				}
				/*�ꂪ���̐��̎�*/
				/*((abc)^(defg))�����*/
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

					i = com_at;			//i�̏C��
				}
				/*exp(abc,defg)�Ȃǂ̎�*/
				/*(abc^(defg))�����*/
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

					i = com_at-2;			//i�̏C��
				}
			}
		}
	}
}


/*�{�c�ɂȂ���...^...��exp(...)�ϊ��̃v���O����*/

///*123^(...) �� exp(123,...)*/
//for(int i=strlen(prob)-1; i > 0; i--){
//	if(prob[i] == '^'){
//		for(digit1 = i+1; isdigit(prob[digit1]); digit1++);	//'^'�̌�̐����̌���(�L��)�𒲂ׂ�
//		digit1--;											//for���[�v�ŃC���N�������g������������␳

//		/*123^(...)�Ȃǂ̎�*/
//		if(digit1 == i){
//			
//			/*'^'�̑O�̐������ǂ��܂ł����ׂ�*/
//			for(digit2 = i-1; isdigit(prob[digit2]) && digit2 != 0; digit2--);

//			/*digit2�������̐擪���w���悤�ɂ���*/
//			if(digit2 != 0 || isdigit(prob[0]) == 0)
//				digit2++;

//			/*'^'�̌�̂����܂肪�ǂ��܂ł����ׂ�*/

//			/*123^x�Ȃǂ̎�*/
//			if(prob[i+1] == 'x')
//				block = i+1;
//			/*123^(...)��123^sin(...)�Ȃǂ̎�*/
//			else{
//				par = 1;

//				/*'('�̌���*/
//				for(block = i+1; prob[block] != '(' && prob[block]; block++);

//				/*')'�̌���*/
//				for(block++; par > 0 && prob[block]; block++){
//					if(prob[block] == '(')
//						par++;
//					else if(prob[block] == ')')
//						par--;
//				}
//				if (par > 0)
//					return -1;

//				block--;		//for���[�v�ŃC���N�������g������������␳
//			}
//			
//			/*exp(123,(...))�����*/

//			/* digit2 i  block */
//			/*  �@�� ��   ��   */
//			/*  �@ 123^(...)   */
//			/*      ���ϊ�     */
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
//			i = digit2;				//�Y�����␳
//		}
//		/*(...)^123�Ȃǂ̎�*/
//		else{
//			/*'^'�̑O�̂����܂肪�ǂ��܂ł����ׂ�*/

//			/*x^123�Ȃǂ̎�*/
//			if(prob[i-1] == 'x')
//				block = i-1;
//			/*(...)^123�Ȃǂ̎�*/
//			else if(prob[i-1] == ')'){
//				par = -1;

//				/*'('�̌���*/
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

//			/*45^(...)^123�Ȃǂ̎�*/
//			if(prob[block-1] == '^'){

//				/*�O��'^'�̍��ɂ��鐔���̐擪�̓Y�����𒲂ׂ�*/
//				for(digit3 = block-2; isdigit(prob[digit3]) != 0 && digit3 != 0; digit3--);
//				if(digit3 != 0 || isdigit(prob[0]) == 0)
//					digit3++;

//				/*exp(...,...)�̌`�ɂ���*/

//				/* digit3 block  i  digit1 */
//				/*  ��     ��    ��    ��   */
//				/*  �S�T�O�i...�j�O�P�Q�R   */
//				/*         ���ϊ�           */
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

//				i = digit3;			//�Y�����␳
//			}
//		}
//	}
//}
