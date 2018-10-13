#include <stdio.h>
#include <malloc.h>

#pragma warning(disable:4996)

unsigned char plain_text[16];
unsigned char cipher_text[16];
unsigned char key[16];

unsigned char initial_permutation[64] =
{ 58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7 };

unsigned char inverse_initial_permutation[64] =
{ 40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25 };

unsigned char expansion_permutation[48] =
{ 32,1,2,3,4,5,
4,5,6,7,8,9,
8,9,10,11,12,13,
12,13,14,15,16,17,
16,17,18,19,20,21,
20,21,22,23,24,25,
24,25,26,27,28,29,
28,29,30,31,32,1 };

unsigned char permutation[32] = { 16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25 };

unsigned char permutation_choice_1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

unsigned char permutation_choice_2[48] = {
	14,17,11,24,1,5,3,28,
	15,6,21,10,23,19,12,4,
	26,8,16,7,27,20,13,2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
};

unsigned char shift_key[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

unsigned char sBox[8][4][16] = {
	{
		{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
	},
	{
		{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
	},
	{
		{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
	},
	{
		{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
	},
	{
		{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
	},
	{
		{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
	},
	{
		{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
	},
	{
		{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
	}
};

void HextoBinary(unsigned char hex[], unsigned char binary[]) {
	unsigned char temp;
	for (int i = 0; i < 16; i++) {
		if (hex[i] <= 'F' && hex[i] >= 'A')
			temp = hex[i] - 55;
		else
			temp = hex[i];

		for (int j = 0; j < 4; j++) {
			binary[i * 4 + 3 - j] = temp % 2;
			temp /= 2;
		}
	}
}

void BinarytoHex(unsigned char binary[], unsigned char hex[]) {
	for (int i = 0; i < 16; i++) {
		hex[i] = binary[i * 4 + 0] * 8 + binary[i * 4 + 1] * 4 + binary[i * 4 + 2] * 2 + binary[i * 4 + 3];
	}

	for (int i = 0; i < 16; i++) {
		if (hex[i] > 9)
			hex[i] = hex[i] % 10 + 65;
		else
			hex[i] = hex[i] + 48;
	}
}

// ASCII 코드를 Binary로 변환
void ASCtoBinary(unsigned char asc[], unsigned char binary[]) {
	unsigned char temp;
	for (int i = 0; i < 8; i++) {
		temp = asc[i];
		for (int j = 0; j < 8; j++) {
			binary[i * 8 + 7 - j] = temp % 2;
			temp /= 2;
		}
	}
}

void BinarytoASC(unsigned char Lbinary[], unsigned char Rbinary[], unsigned char asc[]) {
	for (int i = 0; i < 4; i++) {
		asc[i] = Lbinary[i * 8 + 0] * 128 + Lbinary[i * 8 + 1] * 64 + Lbinary[i * 8 + 2] * 32 + Lbinary[i * 8 + 3] * 16 + Lbinary[i * 8 + 4] * 8 + Lbinary[i * 8 + 5] * 4 + Lbinary[i * 8 + 6] * 2 + Lbinary[i * 8 + 7];
		asc[i + 4] = Rbinary[i * 8 + 0] * 128 + Rbinary[i * 8 + 1] * 64 + Rbinary[i * 8 + 2] * 32 + Rbinary[i * 8 + 3] * 16 + Rbinary[i * 8 + 4] * 8 + Rbinary[i * 8 + 5] * 4 + Rbinary[i * 8 + 6] * 2 + Rbinary[i * 8 + 7];
	}
}

void Generate_Key(unsigned char key[], unsigned char permutation_key[][48]) {
	unsigned char temp1; //Shift를 위한 임시 변수
	unsigned char temp2; //Shift를 위한 임시 변수
	unsigned char temp_arr[56]; //생성된 Key를 저장하기 위한 임시 변수

	// PC-1 순열을 이용하여 키 섞기
	for (int i = 0; i < 56; i++) {
		temp_arr[i] = key[permutation_choice_1[i] - 1];
	}

	// 각 라운드 별 적용할 서브키 생성
	for (int i = 0; i < 16; i++) {

		// 라운드 별 Shift 테이블을 이용하여 Key Shift
		for (int j = 0; j < shift_key[i]; j++) {
			temp1 = temp_arr[0];
			temp2 = temp_arr[28];

			for (int k = 0; k < 27; k++) {
				temp_arr[k] = temp_arr[k + 1];
				temp_arr[k + 28] = temp_arr[k + 29];
			}

			temp_arr[27] = temp1;
			temp_arr[55] = temp2;
		}

		// PC-2 순열을 이용하여 최종 SubKey 생성
		for (int j = 0; j < 48; j++) {
			permutation_key[i][j] = temp_arr[permutation_choice_2[j] - 1];
		}
	}
}

void Input_Permutation(unsigned char binary[], unsigned char matrix[], int size) {
	unsigned char *temp;
	temp = (unsigned char *)malloc(sizeof(unsigned char) * size);

	for (int i = 0; i < size; i++) {
		temp[i] = binary[matrix[i] - 1];
	}
	for (int i = 0; i < size; i++) {
		binary[i] = temp[i];
	}

	free(temp);
}

void Split_Array(unsigned char arr[], unsigned char L[], unsigned char R[]) {
	for (int i = 0; i < 64; i++) {
		if (i < 32)
			L[i] = arr[i];
		else
			R[i - 32] = arr[i];
	}
}

void F(unsigned char R[], unsigned char R_expansion[], unsigned char temp_arr[], unsigned char permutation_key[][48], int round) {
	unsigned char temp;

	for (int i = 0; i < 48; i++) {
		R_expansion[i] = R[expansion_permutation[i] - 1];
	}

	for (int i = 0; i < 48; i++) {
		R_expansion[i] ^= permutation_key[round][i];
	}

	for (int i = 0; i < 8; i++) {
		int col = R_expansion[i * 6 + 0] * 2 + R_expansion[i * 6 + 5];
		int row = R_expansion[i * 6 + 1] * 8 + R_expansion[i * 6 + 2] * 4 + R_expansion[i * 6 + 3] * 2 + R_expansion[i * 6 + 4];

		temp = sBox[i][col][row];

		for (int j = 0; j < 4; j++) {
			temp_arr[i * 4 + 3 - j] = temp % 2;
			temp = temp / 2;
		}
	}
	Input_Permutation(temp_arr, permutation, 32);
}

void DES(int mode) {
	unsigned char input_binary[64]; // ASCII 로 받은 입력을 binary로 받기 위한 배열
	unsigned char key_binary[64]; // hex로 된 키 값을 binary로 받기 위한 배열
	unsigned char cipher_binary[64]; // 암호화된 binary를 받기 위한 배열
	unsigned char *L;
	unsigned char *R;
	unsigned char *temp_arr; // 임시 배열
	unsigned char R_expansion[48]; //R을 확장한 값을 저장하기 위한 배열
	unsigned char permutation_key[16][48]; // 각 라운드별를 위해 생성한 SubKey를 저장하기 위한 배열

	L = (unsigned char *)malloc(sizeof(unsigned char) * 32);
	R = (unsigned char *)malloc(sizeof(unsigned char) * 32);
	temp_arr = (unsigned char *)malloc(sizeof(unsigned char) * 32);

	HextoBinary(plain_text, input_binary); //Hex 값을 Binary로 변환
	HextoBinary(key, key_binary); //Hex 값을 Binary로 변환

	Input_Permutation(input_binary, initial_permutation, 64); //Binary를 IP를 통해 재배열
	Split_Array(input_binary, L, R); // 재배열된 바이너리를 32비트씩 나눔

	Generate_Key(key_binary, permutation_key); //key_binary 값을 이용하여 각 라운드마다 필요한 SubKey 생성

	if (mode == 1) {
		for (int round = 0; round < 16; round++) {
			F(R, R_expansion, temp_arr, permutation_key, round);
			for (int i = 0; i < 32; i++) {
				temp_arr[i] ^= L[i];
			}
			for (int i = 0; i < 32; i++) {
				L[i] = R[i];
				R[i] = temp_arr[i];
			}
		}
	}

	if (mode == 2) {
		for (int round = 15; round > -1; round--) {
			F(R, R_expansion, temp_arr, permutation_key, round);
			for (int i = 0; i < 32; i++) {
				temp_arr[i] ^= L[i];
			}
			for (int i = 0; i < 32; i++) {
				L[i] = R[i];
				R[i] = temp_arr[i];
			}
		}
	}

	// 최종적으로 L 과 R 위치를 바꿔 cipher_binary 배열에 저장
	for (int i = 0; i < 32; i++) {
		cipher_binary[i] = R[i];
		cipher_binary[i + 32] = L[i];
	}

	Input_Permutation(cipher_binary, inverse_initial_permutation, 64); // cipher_binary를 역순열에 적용
	BinarytoHex(cipher_binary, cipher_text); //cipher_binary를 Hex값으로 변환

	for (int i = 0; i < 16; i++) {
		printf("%c", cipher_text[i]);
	}
	printf("\n");

	free(L);
	free(R);
	free(temp_arr);
}

int main(void)
{
	int c;
	printf("=========================================\n");
	printf("1. Encrypt\n");
	printf("2. Decrypt\n");
	printf("3. Exit\n");
	printf("=========================================\n");
	printf("Select Menu : ");
	scanf("%d", &c);


	switch (c) {
	case 1:
		printf("Input Text : ");
		scanf("%s", &plain_text);
		printf("Input Key : ");
		scanf("%s", &key);
		DES(c);
		break;

	case 2:
		printf("Input Text : ");
		scanf("%s", &plain_text);
		printf("Input Key : ");
		scanf("%s", &key);
		DES(c);
		break;

	case 3:
		return 0;

	default:
		printf("Wrong Input\n");
		break;
	}
	return 0;
}