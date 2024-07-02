#include<stdio.h>
#include<stdlib.h>

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

int main(){
    BNodePtr root;
    root->key = (int *)malloc((3)*sizeof(int));
    root->key[0] = 1;
    root->key[1] = 2;
    root->child = (BNodePtr *)malloc((3+1)*sizeof(BNodePtr)); //full이면 하나 붙이고 스플릿해야하므로 order+1로 할당
    for(int i = 0;i<=3;i++) //child 초기화
        root->child[i] = NULL;
    // root->key[2] = 3;
    for(int i =0;i<3;i++){
        printf("%d\n", root->key[i]);
        printf("%p\n", root->child[i]);
    }
    return 0;
}