#include <stdio.h>

/* 10진수를 2진수로 변환하기(비트 연산자 이용)*/
int main(void)
{
	int integer = 0, i = 0; //integer : 입력받을 10진수 정수, i : 비트 자리, 

	printf("10진수 양의 정수를 입력하세요 : "); //10진수 정수 입력
	scanf("%d", &integer);
	printf("10진수 %d는 2진수로 ", integer);

	for (i = 6; i >= 0; i--) //앞쪽에 0 표기되는 것을 방지하기 위해 2진수가 처음 시작하는 비트 자리 확인
	{
		if (integer&(1 << i))
		{
			break;
		}
	}
	for (i; i >= 0; i--) // 2진수가 시작하는 비트 자리 부터 '&'연산으로 2진수 출력
	{
		if (integer&(1 << i))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("입니다.");
	printf("\n");
}