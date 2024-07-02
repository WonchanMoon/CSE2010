#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b,result, error_flag=0;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){
        if(('0'<input_str[i])&&(input_str[i]<='9')){ //ASCII로 값이 1~9면
			if(IsFull(stack)){ //스택이 다 차있으면
				error_flag = ISFULL; //ISFULL
				break; //while 나가기
			}
            Push(stack, input_str[i]-48); //ASCII '0'=48이니까 48뺀 값을 push
        }
		else{
			if(input_str[i]=='+'){ //'+'이면
				int b = Pop(stack); //위에 값 b
				int a = Pop(stack); //그 아래 값 a
				if (IsEmpty(stack)) { //a,b pop 시키고 스택이 비어있으면
					error_flag = ISEMPTY; //ISEMPTY
					break; //while 나가기
				}
				Push(stack, a+b); //a+b 값을 push
			}
			else if(input_str[i]=='-'){ //'-'이면
				int b = Pop(stack); //위에 값 b
				int a = Pop(stack); //그 아래 값 a
				if (IsEmpty(stack)) { //a,b pop 시키고 스택이 비어있으면
					error_flag = ISEMPTY; //ISEMPTY
					break; //while 나가기
				}
				Push(stack, a-b); //a-b 값을 push
			}
			else if(input_str[i]=='*'){ //'*'이면
				int b = Pop(stack); //위에 값 b
				int a = Pop(stack); //그 아래 값 a
				if (IsEmpty(stack)) { //a,b pop 시키고 스택이 비어있으면
					error_flag = ISEMPTY; //ISEMPTY
					break; //while 나가기
				}
				Push(stack, a*b); //a*b 값을 push
			}
			else if(input_str[i]=='/'){ //'/'이면
				int b = Pop(stack); //위에 값 b
				int a = Pop(stack); //그 아래 값 a
				if (IsEmpty(stack)) { //a,b pop 시키고 스택이 비어있으면
					error_flag = ISEMPTY; //ISEMPTY
					break; //while 나가기
				}
				else if (b == 0) { //나누는 값이 0이면
					error_flag = DIVIDEZERO; //DIVIDEZERO
					break; //while 나가기
				}
				Push(stack, a/b); //a/b 값을 push
			}
			else{ // %연산
				int b = Pop(stack); //위에 값 b
				int a = Pop(stack); //그 아래 값 a
				if (IsEmpty(stack)) { //a,b pop 시키고 스택이 비어있으면
					error_flag = ISEMPTY; //ISEMPTY
					break; //while 나가기
				}
				else if (b == 0) { //나누는 값이 0이면
					error_flag = DIVIDEZERO; //DIVIDEZERO
					break; //while 나가기
				}
				Push(stack, a%b); //a%b 값을 push
			}
		}


		if(error_flag) break;

		int t = Top(stack);
		fprintf(fout, "%d ",t);
		result=t;
		i++;
	}

	if(error_flag == ISFULL){
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if(error_flag == DIVIDEZERO){
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    Stack* S = (Stack*)malloc(sizeof(struct Stack)); //스택을 가리키는 포인터 S에 구조체 만큼 메모리 할당
    S->max_stack_size = max; //stack의 최대 용량을 max로 설정
    S->key = (int*)malloc(sizeof(int)*(S->max_stack_size)); //배열을 가리킬 포인터 key에 최대 용량*int사이즈 만큼 메모리 할당
	S->top = -1; //0부터 접근할거니까 -1로 초기화
    return S; //포인터 반환
}

void DeleteStack(Stack* S){
    free(S->key); //포인터 key에 할당된 메모리 해제
    free(S); //포인터 S에 할당된 메모리 해제
}

void Push(Stack* S, int X){
	S->top = (S->top)+1; //stack 맨 위 index 하나 미루고
	(S->key)[S->top]=X; //그 index 위치에 X값 저장
}

int Pop(Stack* S){
	int obj = (S->key)[S->top]; //현재 stack 맨 위에 있는 값 obj에 저장
	S->top = (S->top)-1; //stack의 맨 위를 가리키는 index 1감소
	return obj; //저장했던 obj 반환
}

int Top(Stack* S){
	return (S->key)[S->top]; //stack 맨 위 index에 해당하는 값 반환
}

int IsFull(Stack* S){
	if(S->top != 19) return 0; //맨 위 index가 19가 아니면 다 찬게 아니므로 0반환
	return ISFULL; //그 외에는 ISFULL
}

int IsEmpty(Stack* S){
	if(S->top > -2) return 0; //index 0에 해당하는 값을 pop하고 -1이 된 상태에서는 계산이 가능하므로,
	return ISEMPTY; //비어있는 상태에서 pop을 시도해 index가 -2가 될시 ISEMPTY
}