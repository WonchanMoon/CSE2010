#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);


void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size); //size+1 => size 수정. size 늘리는건 아래 할당할 때 구현

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	printShortestPath(g);

	deleteGraph(g);
}

Graph* createGraph(int size){
	Graph* new = (Graph*)malloc(sizeof(struct Graph));
	new->size = size;
	new->nodes = (Node*)malloc(sizeof(struct Node)*(size+1));
	//간선은 0 초기화
	new->vertices = (int **)malloc(sizeof(int *)*(size+1)); //행
	int i;
	for(i=1;i<=size;i++){ //열
		*(new->vertices + i) = (int *)malloc(sizeof(int)*(size+1));
	}
	for(i=1;i<=size;i++){
		for(int j=1;j<=size;j++){
			new->vertices[i][j] = 0;
			// if(i==j)
			// 	new->vertices[i][j] = i; //자기노드로 향하는 간선은 자기 노드로 
		}
	}
	//distance = INF, prev = -1
	//시작 dist = 0
	for(i=1;i<=size;i++){
		new->nodes[i].vertex = i;
		new->nodes[i].dist = INF;
		new->nodes[i].prev = -1;
	}
	new->nodes[1].dist = 0;
	
	return new;
}
void deleteGraph(Graph* g){
	//direction free -> vertex free -> ㅁㅁㅁ free 순서 지키기
	free(g->vertices);
	free(g->nodes);
	free(g);
}
void printShortestPath(Graph* g){
	Heap* minheap = createMinHeap(g->size); //힙으로 관리
	insertToMinHeap(minheap, 1, 0); //start node 삽입
	Node now;
	int i, dist_sum;
	while(minheap->Size!=0){
		now = deleteMin(minheap);
		for(i=1;i<=g->size;i++){
			if(g->vertices[now.vertex][i]!=0 && now.vertex!=i){ //인접 노드 찾기
				dist_sum = g->nodes[now.vertex].dist + g->vertices[now.vertex][i];
				if(g->nodes[i].dist > dist_sum){ //더 빠른 경로면 업데이트 및 힙에 넣기
					g->nodes[i].dist = dist_sum;
					g->nodes[i].prev = now.vertex;
					insertToMinHeap(minheap, i, dist_sum);
				}
			}
		}
	}
	int j, k;
	int prev[g->size]; //size만큼 배열 할당
	for(i=2;i<=g->size;i++){ //1빼고 나머지 순회하며 print
		if(g->nodes[i].prev != -1){ //prev가 -1 아니어야 연결된 노드
			j = i;
			k = 0;
			while(g->nodes[j].prev != 1){ //i의 prev 순회
				j = g->nodes[j].prev;
				prev[k] = j; //순서대로 넣기
				k++;
			}
			k--;
			fprintf(fout, "1->");
			for(;k>=0;k--) //넣은 순서 반대로 꺼내기
				fprintf(fout, "%d->",prev[k]);
			fprintf(fout, "%d ", i);
			fprintf(fout, "(cost : %d)\n", g->nodes[i].dist);
		}
		else //연결 안됐을 시
			fprintf(fout, "can not reach to node %d\n", i);
	}

	deleteMinHeap(minheap);
}
Heap* createMinHeap(int heapSize){
	//heap 할당
	Heap* heap = (Heap *)malloc(sizeof(struct Heap)); //메모리 할당
    heap->Size = 0; //사이즈 초기화
    heap->Capacity = heapSize; //용량 설정
    heap->Element = (Node *)malloc((heap->Capacity+1)*sizeof(struct Node)); //데이터를 담을 배열 메모리 할당. 인덱스는 1부터 쓸 예정으로 한 자리 추가해서 할당
    return heap; //힙 리턴
}
void deleteMinHeap(Heap* minHeap){
	free(minHeap->Element);
	free(minHeap);
}
void insertToMinHeap(Heap* minHeap, int vertex, int distance){
	//minheap의 사이즈 증가, heap 조건 충족
	minHeap->Size++;
	//삽입해야하는 부분의 vertex, dist 업데이트
	int i;
	Node temp;
	(minHeap->Element)[minHeap->Size].vertex = vertex; //끝에 추가
	(minHeap->Element)[minHeap->Size].dist = distance;
	for(i = minHeap->Size;i>1;i/=2){ //부모보다 거리 작으면 부모랑 교체
		if((minHeap->Element)[i].dist<(minHeap->Element)[i/2].dist){
			temp = (minHeap->Element)[i/2];
			(minHeap->Element)[i/2] = (minHeap->Element)[i];
			(minHeap->Element)[i] = temp;
		}
	}
}
Node deleteMin(Heap* minHeap){
	//minheap에선 return r값이 root, 교환할 값은 맨 마지막 노드
	Node root = (minHeap->Element)[1];
		(minHeap->Element)[1] = (minHeap->Element)[minHeap->Size]; //끝 노드를 루트로 보냄
		minHeap->Size--; //사이즈 감소
		int index = 1; 
		int left=2;
		int right=3;
		int small;
		Node temp;
		while (left <= minHeap->Size){ //왼쪽 오른쪽 자식 노드랑 비교해서 dist 더 작은 노드를 선택. 이후 index노드 보다 크면 교체. 왼쪽 자식이 없을 때까지 반복
			small = left;
			if(((minHeap->Element)[left].dist>(minHeap->Element)[right].dist) && (right <= minHeap->Size))
				small = right;
			if((minHeap->Element)[index].dist>(minHeap->Element)[small].dist){
				temp = (minHeap->Element)[index];
				(minHeap->Element)[index] = (minHeap->Element)[small];
				(minHeap->Element)[small] = temp;
				index = small;
				left = index*2;
				right = index*2 + 1;
			}
			else
				break; //교체가 안된다면 더 이상 안내려가므로 반복문 탈출
		}
		return root; //min 반환
}