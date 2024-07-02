#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
    ElementType element;
    Position next;
};

List MakeEmpty(List L){
    L = (List)malloc(sizeof(struct Node)); //노드에 해당하는 사이즈로 메모리 할당.
    L -> element = -1; //Head의 element는 -1로 고정.
    L -> next = NULL; //다음 노드를 가리키는 포인터는 NULL.
    return L; //포인터 반환.
};
int IsEmpty(List L){
    return L -> next == NULL; //Head의 다음 노드가 비어있으면 TRUE.
};
int IsLast(Position P, List L){
    return P -> next == NULL; //다음 노드를 가리키지 않으면 Tail.
};
void Insert(ElementType X, List L, Position P){
    Position tmp;
    tmp = malloc(sizeof(struct Node)); //새로운 노드에 메모리 할당.
    tmp -> element = X; //노드의 element 값 X로 설정.
    tmp -> next = P -> next; //next값은 앞 노드가 가리키고 있던 뒷 노드로 설정.
    P -> next = tmp; //앞 노드의 포인터는 새로운 노드를 가리킴.
}; 
void PrintList(List L){
    if(L->next==NULL) fprintf(fout, "empty list!\n"); //Head 다음 노드가 없으면 빈 리스트.
    else{
        Position P;
        P=L; //L로 초기화.
        while(P->next!=NULL){ //다음 노드가 비어있지 않으면
            P = P -> next; //다음 노드로 이동.
            fprintf(fout, "key:%d ",P->element); //노드의 element 출력.
        };
        fprintf(fout, "\n"); //줄바꿈.
    };
};
Position FindPrevious (ElementType X, List L){
    Position P;
    P = L;
    while (P->next!=NULL&&P->next->element!=X){ //Tail이 아니고 다음 노드의 값이 X가 아니면 계속 탐색.
        P=P->next; //다음 노드로 이동.
    };
    return P; //X 찾았으면 그 노드 전 노드 포인터 반환. 못 찾았으면 Tail을 가리키는 포인터 반환.
};
void Delete(ElementType X, List L){
    Position P, tmp;
    P = FindPrevious(X, L);
    if(P->next==NULL) fprintf(fout, "deletion(%d) failed : element %d is not in the list\n",X,X); //P가 Tail을 가리키면 X를 가진 노드가 없는 것.
    else{
        tmp = P -> next; //없앨 노드 가리키기.
        P -> next = tmp -> next; //앞 노드는 없앨 노드의 뒷 노드를 가리킴.
        free(tmp); //없앨 노드 없애기.
    };
    
};
Position Find(ElementType X, List L){
    if(X==-1) return L; //-1이면 Head를 가리키는 포인터 반환.
    Position P;
    P = L -> next; //Head 다음 노드부터 찾기 시작.
    while(P!=NULL && P -> element !=X){ //P가 NULL이 아니고 값이 X가 아니면 계속 탐색.
        P = P -> next; //다음노드로 이동.
    };
    return P; //X가 있는 노드를 가리키는 포인터 반환.

};
void DeleteList (List L){
    Position P;
    P = L; //L로 초기화.
    while(!IsLast(P,L)){ //마지막 노드가 아니면
        P = P -> next; //다음 노드 접근해서
        Delete(P->element, L); // 그 노드 지우기.
    };
    free(L); //Head 가리키는 포인터에 할당된 메모리 해제.
};


int main(int argc, char **argv){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");
    char x;
    Position header=NULL, tmp=NULL;
    header = MakeEmpty(header);

    while(fscanf(fin,"%c",&x)!=EOF){
    if(x=='i'){
        int a,b;
        fscanf(fin, "%d%d",&a,&b);
        tmp = Find(a, header); 
        if(tmp!=NULL){
            fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a); 
            continue;
        }
        tmp = Find(b, header); 
        if(tmp==NULL){
            fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
            continue;
        }
        Insert(a, header, tmp);
    }
    else if(x=='d'){
        int a; 
        fscanf(fin,"%d",&a); 
        Delete(a, header);
    }
    else if(x=='f'){
        int a; 
        fscanf(fin,"%d",&a);
        tmp = FindPrevious(a, header);
        if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
        else{
            if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
            else fprintf(fout, "key of the previous node of %d is head\n",a);
        }
    }
    else if(x=='p') PrintList(header);
    }
    DeleteList(header);
    fclose(fin); 
    fclose(fout);

    return 0;
}