#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(EOF!=fscanf(fin, "%c", &cv)){ //인풋의 마지막 줄이 공백이 아니면 두 번 출력되는 오류 수정
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(maxHeap, key);
				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}
	
	free(maxHeap->Elements); //free array
	free(maxHeap); //free struct

	return 0;
}

Heap* CreateHeap(int heapSize){
	/* create a new heap */
    Heap* heap = (Heap *)malloc(sizeof(struct HeapStruct)); //메모리 할당
    heap->Size = 0; //사이즈 초기화
    heap->Capacity = heapSize; //용량 설정
    heap->Elements = malloc((heap->Capacity+1)*sizeof(int)); //데이터를 담을 배열 메모리 할당. 인덱스는 1부터 쓸 예정으로 한 자리 추가해서 할당
    return heap; //힙 리턴
}

void Insert(Heap *heap, int value){
	/* insert a value into the heap */
    // Full
    if(IsFull(heap))
        fprintf(fout, "insert error : heap is full\n");
    // 중복
	else if(Find(heap, value))
		fprintf(fout, "insert error : %d is already in the heap\n", value);
    //insert
    else{
		heap->Size += 1; //사이즈 += 1
		int temp;
		fprintf(fout, "insert %d\n", value);
		(heap->Elements)[heap->Size] = value; //끝에 추가
		for(int i = heap->Size;i>1;i/=2){ //부모보다 크면 부모랑 교체(재귀)
			if((heap->Elements)[i]>(heap->Elements)[i/2]){
				temp = (heap->Elements)[i/2];
				(heap->Elements)[i/2] = (heap->Elements)[i];
				(heap->Elements)[i] = temp;
			}
		}
	}
    
}

int Find(Heap* heap, int value){
	/* find a value in the heap */
	for(int i = 1;i<=heap->Size;i++){ //배열 탐색
		if((heap->Elements)[i]==value)
			return 1;
	}
	return 0;

}

int DeleteMax(Heap* heap){
	/* delete Max value from the heap */
	//비었으면 에러
	if(heap->Size == 0){ //0이면 비어있음
        fprintf(fout, "delete error : heap is empty\n");
		return -INF;
	}
	else{ //비어있지 않으면 max 삭제
		int root = (heap->Elements)[1];
		(heap->Elements)[1] = (heap->Elements)[heap->Size]; //끝 노드를 루트로 보냄
		heap->Size -= 1; //사이즈 감소
		int index = 1; 
		int left=2;
		int right=3;
		int big;
		int temp;
		while (left <= heap->Size){ //왼쪽 오른쪽 자식 노드랑 비교해서 더 큰 노드를 선택. 이후 index노드 보다 크면 교체. 왼쪽 자식이 없을 때까지 반복
			big = left;
			if(((heap->Elements)[left]<(heap->Elements)[right]) && (right <= heap->Size))
				big = right;
			if((heap->Elements)[index]<(heap->Elements)[big]){
				temp = (heap->Elements)[index];
				(heap->Elements)[index] = (heap->Elements)[big];
				(heap->Elements)[big] = temp;
				index = big;
				left = index*2;
				right = index*2 + 1;
			}
			else
				break; //교체가 안된다면 더 이상 안내려가므로 반복문 탈출
		}
		return root; //max 반환
	}
}

void PrintHeap(Heap* heap){
	/* print the heap */
    //비었으면 에러
    if(heap->Size == 0)
        fprintf(fout, "print error : heap is empty\n");
    else{ //안 비었으면 level에 따라 출력
		for(int i = 1;i<=heap->Size;i++){
			fprintf(fout, "%d ",(heap->Elements)[i]);
		}
		fprintf(fout, "\n");
    }
}

int IsFull(Heap* heap){
	/* check if the heap is full */
    return (heap->Capacity == heap->Size); //사이즈가 용량과 같으면 가득 찼으므로 1 반환
}

