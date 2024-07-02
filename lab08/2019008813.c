#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);



int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
	while(EOF!=fscanf(fin, "%c", &cv)){ //인풋의 마지막 줄이 공백이 아니면 두 번 출력되는 오류 수정
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 0)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n"); //줄바꿈
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr root = (BNodePtr)malloc(sizeof(struct BNode)); //root 할당
    root->order = order; //order 설정
    root->key = (int *)malloc((order)*sizeof(int)); //full에서 한 칸 뒤로 미룰거 생각해서 order만큼 할당
    root->child = (BNodePtr *)malloc((order+1)*sizeof(BNodePtr)); //full이면 하나 붙이고 스플릿해야하므로 order+1로 할당
    root->is_leaf = 1; //처음 root는 leaf
    root->size = 0; //size는 비어있으면 0으로 설정

    return root; //root 반환
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언
    *root = Insert_sub(*root, *root, key, 0); // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}

BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){ //p_pos는 split하고 붙일 곳.
    int pos;
    for(pos=0;pos<=node->size;pos++){ //위치 찾기
        if(key<node->key[pos] || pos==node->size)
            break;
    }
    if(!node->is_leaf){ //현재 노드가 리프가 아니라면
        node->child[pos] = Insert_sub(node, node->child[pos], key, pos); //재귀
        if(node->size >= node->order) //재귀 후 size가 full이면 split
            node = split_node(parent, node, p_pos);
    }
    else{ //현재 노드가 리프면 insert
        int i;
        for(i=node->size-1;i>=pos;i--){ //key 한 칸씩 밀기, child는 leaf면 없으니까 상관 x.
            node->key[i+1] = node->key[i];
        }
        
        //key값 insert 및 사이즈 증가
        node->key[pos] = key;
        node->size++;
        if(node->size>=node->order) //size가 full이면
            node = split_node(parent, node, p_pos);

    }
    return node; //node 반환
}

BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){ //여기 pos는 이어붙일 곳 pos
    int mid = (node->order-1)/2;
    int i;
    BNodePtr right_child;
    BNodePtr new_root;
    //right child생성 하고 key 옮겨담기
    right_child = Createnode(node->order);
    right_child->is_leaf = node->is_leaf;
    for(i=mid+1;i<node->size;i++){
        right_child->key[right_child->size]=node->key[i];
        right_child->size++;
    }
    
    if(!right_child->is_leaf){ //leaf node가 아닌 경우
        for(i=0;mid+1+i<=node->size;i++) //child 옮겨담기
            right_child->child[i] = node->child[mid+1+i];
    }
    node->size=mid; //size 줄이기.

    //parent 처리: 중앙값 올리고, parent와 split한 두 node 연결
    if(parent==node){ //루트 노드 split인 경우
        new_root = Createnode(node->order); //새로운 root 생성
        new_root->is_leaf=0; //자식이 있으므로 leaf 아님
        //중앙값 올리고 size 증가
        new_root->key[0] = node->key[mid];
        new_root->size++;
        //왼쪽 child는 기존 node
        new_root->child[0] = node;
        // 그다음은 right child
        new_root->child[1] = right_child;
        return new_root; //새로운 root 반환
    }
    else{ //루트 노드 아닌경우
        for(i=parent->size-1;i>=pos;i--){ //key랑 child 한칸씩 밀기
            parent->key[i+1] = parent->key[i];
            parent->child[i+2] = parent->child[i+1];
        }
        //중앙값 올리고 size 증가
        parent->key[pos] = node->key[mid];
        parent->size++;
        //부모노드에 right child 연결
        parent->child[pos+1] = right_child;
    }
    return node; //node 반환
}

/* 노드 하나를 동적할당하는 함수, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){ 

    return CreateTree(order); //is_leaf는 본 함수 호출 바로 다음 문장에서 다시 설정
}

/* find node that has key */
int Find(BNodePtr root, int key){
    int i;
    for(i=0;i<=root->size;i++){ //size+1만큼 반복
        if(key<root->key[i]||i==root->size) //값 보다 작으면 탈출
            break;
        if(key==root->key[i]) //값 찾았으면 1
            return 1;
    }
    if(!root->is_leaf) //탈출하고 leaf가 아니면 재귀
        return Find(root->child[i], key);
    return 0; //탈출하고 leaf였으면 0
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root){
    int i;
    if(!root->is_leaf){ //leaf 아니면
        for(i=0;i<=root->size;i++){ //size+1만큼 반복
            PrintTree(root->child[i]); //재귀
            if(i<root->size) //size보다 작으면 올바른 index이므로 출력
                fprintf(fout, "%d ", root->key[i]);
        }
    }
    else{ //leaf면 
        for(i=0;i<root->size;i++){ //하나씩 출력
            fprintf(fout, "%d ", root->key[i]);
        }
    }
}


/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    int i;
    
    if(!root->is_leaf){ //leaf아니면
        for(i=0;i<=root->size;i++){ //모든 child에 대해 재귀
            DeleteTree(root->child[i]);
        }
    }
    free_node(root); //메모리 해제 함수 호출
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    free(node->child); //child 주소 담는 배열 메모리 해제
    free(node->key); //key 담는 배열 메모리 해제
    free(node); //해당 node 구조체 메모리 해제
}
