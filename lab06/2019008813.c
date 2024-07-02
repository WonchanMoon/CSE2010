#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
//함수 추가
AVLTree Height(AVLTree T); //노드의 높이 구하는 함수
AVLTree Balance(AVLTree T); //노드의 밸런스 맞추는 함수
int Diff(AVLTree T); //밸런스 맞출지 판단하기 위한 노드의 양옆 높이 차이 함수


int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}

AVLTree Height(AVLTree T){ //NULL은 높이: -1
	if(T->left == NULL && T->right == NULL){ //둘 다 NULL이면 높이 0
		T->height = 0;
	}
	else if(T->left == NULL){ //왼쪽이 NULL이면 오른쪽 높이 +1
		T->height = T->right->height + 1;
	}
	else if(T->right == NULL){ //오른쪽이 NULL이면 왼쪽 높이 +1
		T->height = T->left->height + 1;
	}
	else{ //양쪽 다 NULL 아니면 높은 쪽 높이 +1
		T->height = (T->left->height > T->right->height ? T->left->height : T->right->height) + 1;
	}
	return T;
}

int Diff(AVLTree T){ //NULL의 높이: -1
	int diff;

	if(T->left == NULL && T->right == NULL){ //둘 다 NULL이면 0
		diff = 0;
	}
	else if(T->left == NULL){ //왼쪽 NULL이면 왼쪽 높이를 -1 설정 후 빼기
		diff = (-1) - T->right->height;
	}
	else if(T->right == NULL){ //오른쪽 NULL이면 오른쪽 높이를 -1 설정 후 빼기
		diff = T->left->height - (-1);
	}
	else{ //둘 다 NULL 아니면 두 높이 뺀 값
		diff = T->left->height - T->right->height;
	}
	return diff;
}

AVLTree Balance(AVLTree T){
	int diff =	Diff(T);
	if(diff > 1){ //왼쪽 높이 - 오른쪽 높이 > 1이면 왼쪽에서 문제 생긴 거
		int diff_L = Diff(T->left); //T의 왼쪽 노드의 diff
		if(diff_L>=0){ //0보다 크면 왼쪽의 왼쪽이 문제니까 LL + 같으면 LL
			// fprintf(fout, "LL\n");
			T = SingleRotateWithLeft(T);
		}
		else if(diff_L<0){ //0보다 작으면 왼쪽의 오른쪽이 문제니까 LR
			// fprintf(fout, "LR\n");
			T = DoubleRotateWithLeft(T);
		}
	}
	else if(diff < -1){ //왼쪽 높이 - 오른쪽 높이 < -1이면 오른쪽에서 문제 생긴 거
		int diff_R = Diff(T->right);
		if(diff_R>0){ //0보다 크면 오른쪽의 왼쪽이 문제니까 RL
			// fprintf(fout, "RL\n");
			T = DoubleRotateWithRight(T);
		}
		else if(diff_R<=0){ //0보다 작으면 오른쪽의 오른쪽이 문제니까 RR + 같으면 RR
			// fprintf(fout, "RR\n");
			T = SingleRotateWithRight(T);
		}
	}
	return T;
}

AVLTree Insert(ElementType X, AVLTree T){
	if(T==NULL){ //비었으면 그 자리에 Insert
		T = (AVLNode *)malloc(sizeof(AVLNode));
		T->element = X;
		T->left = NULL;
		T->right = NULL;
	}
	else if(X<T->element){ //X가 노드 값보다 작으면
		T->left = Insert(X, T->left); //왼쪽에서 Insert
	}
	else if(X>T->element){ //X가 노드 값보다 크면
		T->right = Insert(X, T->right); //오른쪽에서 Insert
	}
	else{ //X가 노드 값이랑 같으면 이미 있으므로 error
		fprintf(fout, "insertion error : %d is already in the tree!\n", X);
	}
	
	T = Height(T); //재귀하면서 노드들의 height 재계산
	T = Balance(T); //재계산된 height로 노드들 balance 맞춰주기
	return T;
}

AVLTree Delete(ElementType X, AVLTree T){ //전 과제인 BST의 delete 참고
	if(T == NULL){ //노드가 NULL이면 tree에 X가 없으므로 error
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
		return T;
	}
	if(X<T->element){ //삭제할 값이 노드보다 작으면
		T->left = Delete(X, T->left); //왼쪽에서 재귀
	}
	else if(X>T->element){ //삭제할 값이 노드보다 크면
		T->right = Delete(X, T->right); //오른쪽에서 재귀
	}
	else{ //삭제할 값이 노드랑 같으면
		if(T->left==NULL && T->right==NULL){ //자식 노드가 없으면
			free(T); //메모리 해제
			T = NULL; //삭제한 노드 NULL처리
		}
		else if(T->left==NULL){ //자식노드가 오른쪽에 있으면
			Position temp = T; //임시 노드 선언
			T = T->right; //오른쪽 노드를 땡김
			free(temp); //원래 노드 메모리 해제
		}
		else if(T->right==NULL){ //자식노드가 왼쪽에 있으면
			Position temp = T; //임시 노드 선언
			T = T->left; //왼쪽 노드 땡김
			free(temp); //원래 노드 메모리 해제
		}
		else{ //자식노드가 둘 다 있으면
			Position temp = T->right; //오른쪽으로 이동
			while(temp->left!=NULL){ //왼쪽이 빌때까지 반복
			temp = temp->left; //왼쪽으로 이동
			}
			T->element = temp->element; //삭제할 노드에 직후 값을 삽입
			T->right = Delete(temp->element, T->right); //직후 값 delete (재귀)
			}
	}
	if(T!=NULL){ //재귀하면서 노드가 NULL이 아니면
		T = Height(T); //노드들 height 재계산
		T = Balance(T); //재계산된 height로 balance 맞추기
	}
	return T;
}

Position SingleRotateWithLeft(Position node){ //LL
	//시계방향 회전
	Position L = node->left;
	node->left = L->right;
	L->right = node;
	
	//원래 노드랑 그 왼쪽 노드랑 height가 바뀌었으므로 재계산
	node = Height(node);
	L = Height(L);

	return L; //왼쪽 노드를 반환 (현재는 가장 상위 노드)
}

Position SingleRotateWithRight(Position node){ //RR
	//반시계방향 회전
	Position R = node->right;
	node->right = R->left;
	R->left = node;

	//원래 노드랑 오른쪽 자식 노드랑 height가 바뀌었으므로 재계산
	node = Height(node);
	R = Height(R);

	return R; //오른쪽 노드를 반환 (현재는 가장 상위 노드)

}

Position DoubleRotateWithLeft(Position node){ //LR
	//LR이므로 왼쪽 자식 노드에서 반시계방향 회전하고 주어진 노드에서 시계방향 회전
	node->left = SingleRotateWithRight(node->left);
	node = SingleRotateWithLeft(node);
	return node;
}

Position DoubleRotateWithRight(Position node){ //RL
	//RL이므로 오른쪽 자식 노드에서 시계방향 회전하고 주어진 노드에서 반시계방향 회전
	node->right = SingleRotateWithLeft(node->right);
	node = SingleRotateWithRight(node);
	return node;
}

void PrintInorder(AVLTree T){ //일반 BST에서와 마찬가지로 재귀함수를 통해 구현
	if(T != NULL){
		PrintInorder(T->left);
		fprintf(fout, "%d(%d) ",T->element, T->height);
		PrintInorder(T->right);
	}
}

void DeleteTree(AVLTree T){ //BST와 마찬가지로 재귀함수를 통해 구현
	if(T != NULL){
		DeleteTree(T->left);
		DeleteTree(T->right);
		free(T);
	}
}