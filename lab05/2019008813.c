#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)) //key 이미 있는지 확인
					fprintf(fout, "insertion error: %d is already in the tree\n", key); //있으면 에러 출력
				
				else{
					root = insertNode(root, key); //insert 실행
					fprintf(fout, "insert %d\n", key); //insert 출력
				}
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%s", &cv); //%c로 하면 pi 뒤 공백이 없을 시, pi 후 insertion을 실행함.
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}

Tree insertNode(Tree root, int key){
	if(root==NULL){ //트리 비어있으면 루트에 insert
		root = (BST *)malloc(sizeof(BST)); //메모리 할당
		root->value = key; //값 삽입
		root->left = NULL; //왼쪽 초기화
		root->right = NULL; //오른쪽 초기화
	}
	else{ //루트 안비어있으면
		Tree ptr = root; //노드 가리킬 ptr 선언
		while(1){ //insert되면 break
			if(key<ptr->value){ //ptr값보다 작으면
				if(ptr->left==NULL){ //왼쪽 비어있으면 거기에 insert
					Tree node = (BST *)malloc(sizeof(BST));
					node->value = key;
					node->left = NULL;
					node->right = NULL;
					ptr->left = node;
					break;
				}
				else ptr = ptr->left; //안비어있으면 왼쪽으로 내려감
			}
			else{ //ptr값보다 크면
				if(ptr->right==NULL){ //오른쪽 비어있으면 거기에 insert
					Tree node = (BST *)malloc(sizeof(BST));
					node->value = key;
					node->left = NULL;
					node->right = NULL;
					ptr->right = node;
					break;
				}
				else ptr = ptr->right; //안비었으면  오른쪽으로 내려감
			}
		}
	}
	return root; //insert 후 tree 반환
}

Tree deleteNode(Tree root, int key){ //자식노드의 수로 지운 뒤 결과 다름
	if(key<root->value){ //삭제할 값이 노드보다 작으면
		root->left = deleteNode(root->left, key); //왼쪽에서 재귀
	}
	else if(key>root->value){ //삭제할 값이 노드보다 크면
		root->right = deleteNode(root->right, key); //오른쪽에서 재귀
	}
	else{ //삭제할 값이 노드랑 같으면
		if(root->left==NULL && root->right==NULL){ //자식 노드가 없으면
			free(root); //메모리 해제
			root = NULL; //삭제한 노드 NULL처리
		}
		else if(root->left==NULL){ //자식노드가 오른쪽에 있으면
			Tree temp = root; //임시 노드 선언
			root = root->right; //오른쪽 노드를 땡김
			free(temp); //원래 노드 메모리 해제
		}
		else if(root->right==NULL){ //자식노드가 왼쪽에 있으면
			Tree temp = root; //임시 노드 선언
			root = root->left; //왼쪽 노드 땡김
			free(temp); //원래 노드 메모리 해제
		}
		else{ //자식노드가 둘 다 있으면
			Tree temp = root->right; //오른쪽으로 이동
			while(temp->left!=NULL){ //왼쪽이 빌때까지 반복
			temp = temp->left; //왼쪽으로 이동
			}
			root->value = temp->value; //삭제할 노드에 직후 값을 삽입
			root->right = deleteNode(root->right, temp->value); //직후 값 delete (재귀)
			}
	}
	return root; //삭제 후 tree 반환
}

int findNode(Tree root, int key){ //있으면 1, 없으면 0 return
	if(root==NULL) return 0; //트리 비었으면 0
	else{
		Tree ptr = root;
		while(ptr->value!=key){ //ptr이 찾는 값이 아니면 계속 돌게 설정
			if(key<ptr->value){ //찾는값이 ptr보다 작으면
				if(ptr->left==NULL){ //ptr보다 작은 값이 없으면 0
					return 0; 
				}
				else ptr = ptr->left; //ptr보다 작은 값 있으면 그쪽으로 이동
			}
			else{ //찾는 값이 ptr보다 크면
				if(ptr->right==NULL){ //ptr보다 큰 값 없으면 0
					return 0;
				}
				else ptr = ptr->right; //있으면 그쪽으로 이동
			}
		}
		return 1; //찾는 값이 ptr과 일치하여 while문 빠져나오면 1
	}
}

void printInorder(Tree root){
	if(root!=NULL){ //트리가 비어있지 않으면 재귀함수를 통해 값 출력
		printInorder(root->left);
		fprintf(fout, "%d ", root->value); //inorder
		printInorder(root->right);
	}
}

void deleteTree(Tree root){
	if(root!=NULL){ //트리가 비어있지 않으면 재귀함수를 통해 free
		deleteTree(root->left);
		deleteTree(root->right);
		free(root); //pi와는 다르게 부모노드를 먼저 free시키지 않기 위해 나중에 free
	}
}
