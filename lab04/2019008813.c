#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread
}ThreadedTree;

ThreadedPtr CreateTree(){
	ThreadedPtr tree = NULL;
	tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(tree==NULL){
		fprintf(fout, "Out of Space!\n");
		return NULL;
	}
	tree->data = -1;
	tree->left_thread = 1;
	tree->left_child = tree;
	tree->right_thread = 1;
	tree->right_child = tree;
	return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
	ThreadedPtr pos = root; //위치 표시하기 위한 포인터
	ThreadedPtr new; //새로 넣을 노드 포인터
	new = (ThreadedPtr)malloc(sizeof(ThreadedTree)); //새 노드에 메모리 할당
	new->data=data; //데이터 넣기
	new->left_thread = 1; //쓰레드 값은 끝 노드이므로 전부 1
	new->right_thread = 1;

	int i = 6; //입력이 100이하 자연수니까 이진수로 6자리면 표현 가능
	for(i; i>=0; i--){ //idx를 2진수로 볼 때 처음 1이 나오는 자리 찾기
		if(idx&(1<<i)) break;
	}
	for(i; i>0; i--){ //새로운 노드 넣을 부모노드로 이동
		if(idx&(1<<i)){ //idx를 2진수로 볼 때 i번째 자릿수가 1이면
			pos = pos->right_child; //오른쪽으로 이동
		}
		else{ //i번째 자릿수가 0이면
			pos = pos->left_child; //왼쪽으로 이동
		}
	}
	if(idx&1){ //idx를 2진수로 볼 때 마지막 자릿수가 1이면 오른쪽에 삽입
		pos->right_thread = 0; //부모노드의 오른쪽 쓰레드는 0
		new->right_child = pos->right_child; //부모노드가 오른쪽 가리키는 노드를 새 노드가 오른쪽으로 가리킴
		pos->right_child = new; //부모노드의 오른쪽에 새 노드 삽입
		new->left_child = pos; //새 노드의 왼쪽은 부모노드를 가리키게 함
	}
	else{ //idx를 2진수로 볼 때 마지막 자릿수가 0이면 왼쪽에 삽입
		pos->left_thread = 0; //부모노드의 왼쪽 쓰레드는 0
		new->left_child = pos->left_child; //부모노드가 왼쪽 가리키는 노드를 새 노드가 왼쪽으로 가리킴
		pos->left_child = new; //부모노드의 왼쪽에 새 노드 삽입
		new->right_child = pos; //새 노드의 오른쪽은 부모노드를 가리키게 함
	}

}

void printInorder(ThreadedPtr root){
	ThreadedPtr pos = root->right_child; //시작위치 노드를 루트 다음노드로 설정(1개는 무조건 삽입되므로 예외 없음)
	while(pos->left_thread==0){ //왼쪽 끝까지 이동
		pos = pos->left_child;
	}
	fprintf(fout, "inorder traversal : "); //프린트 시작
	while(pos->data != -1){ //루트 노드가 아니면
		fprintf(fout, "%d ", pos->data); //해당위치의 데이터 출력
		if(pos->right_thread){ //오른쪽으로 이동할 때, 올라가는 방향이면
			pos = pos->right_child; //올라가기
		}
		else{ //내려가는 방향이면
			pos = pos->right_child; //내려가고
			while(pos->left_thread==0){ //왼쪽 끝까지 이동
				pos = pos->left_child;
			}
		}
	}
}

void DeleteTree(ThreadedPtr root){ //전반적으로 printInorder와 비슷함
	ThreadedPtr pos = root->right_child; //시작위치 노드를 루트 다음노드로 설정(1개는 무조건 삽입되므로 예외 없음)
	ThreadedPtr pos_del; //지울 노드를 지정할 포인터
	while(pos->left_thread==0){ //왼쪽 끝으로 이동
		pos = pos->left_child;
	}
	while(pos->data != -1){ //루트 노드가 아니면
		pos_del = pos; //현재 노드를 지울 노드로 설정
		if(pos->right_thread){ //올라가는 방향이면
			pos = pos->right_child; //올라가기
		}
		else{ //내려가는 방향이면
			pos = pos->right_child; //내려가고
			while(pos->left_thread==0){ //왼쪽 끝으로 이동
				pos = pos->left_child;
			}
		}
		free(pos_del); //이동하고나서 지우기
	}
	free(root); //루트도 지우기
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			return 0;
		}
	}

	printInorder(root);
	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
