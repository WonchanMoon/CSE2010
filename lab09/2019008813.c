#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
	Topsort(G);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

Graph* CreateGraph(int* nodes, int n){
	Graph* new = (Graph *)malloc(sizeof(struct _Graph)); //메모리 할당
	//초기화
	new->size = n;
	new->node = nodes;
	sort_node(new->node, n); //sort
	new->matrix = (int **)malloc(sizeof(int *)*n); //행
	int i;
	for(i=0;i<n;i++){ //열
		*(new->matrix + i) = (int *)malloc(sizeof(int)*n);
	}
	for(i=0;i<n;i++){ //adjacency matrix 초기화
		for(int j=0;j<n;j++)
			new->matrix[i][j] = 0;
	}
	return new;
	
}
void InsertEdge(Graph* G, int a, int b){
	for(int i=0;i<G->size;i++){ //index 변환
		if(G->node[i]==a)
			a = i;
		if(G->node[i]==b)
			b = i;
	}
	G->matrix[a][b] = 1; //a->b edge 추가
}
void PrintGraph(Graph* G){ //adjacency matrix 출력
	fprintf(fout, "   ");
	int i;
	for(i=0;i<G->size;i++) //index
		fprintf(fout, "%d  ",G->node[i]);
	fprintf(fout, "\n");
	for(i=0;i<G->size;i++){
		fprintf(fout, "%d  ",G->node[i]); //index
		for(int j=0;j<G->size;j++)
			fprintf(fout, "%d  ", G->matrix[i][j]);
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
}
void DeleteGraph(Graph* G){ //메모리 해제
	free(*(G->matrix));
	free(G->matrix);
	free(G);
}
void Topsort(Graph* G){
	Queue* Q = MakeNewQueue(G->size); //큐 생성
	int idg[10]; //한 자리 수만 node가 될 수 있으니 10개면 충분
	int i;
	for(i=0;i<10;i++)
		idg[i] = 10; //두 자리 수인 10으로 초기화
	checkIndegree(G, idg); //idg에 값 추가
	for(i=0;idg[i]!=10 && i!=10;i++) //idg 원소들 enqueue
		Enqueue(Q, idg[i]);
	int node, node_index;
	int sort_count = 0;
	char* result = malloc(sizeof(char)*(G->size*2)+1); //각 노드 마다 1 공백 추가에 NULL 추가해서 동적할당
	char temp[3];
	while(!IsEmpty(Q)){ //비어있지 않으면
		node = Dequeue(Q); //dequeue
		sort_count++; //횟수 1 증가
		sprintf(temp,"%d",node); //str로 변환
		strcat(temp, " "); //공백 추가
		strcat(result, temp); //출력 문자열에 temp 추가
		for(i=0;i<G->size;i++){ //index 찾기
			if(G->node[i]==node)
				node_index = i;
		}
		for(i=0;i<G->size;i++) //edge 삭제
			G->matrix[node_index][i] = 0;
		G->matrix[node_index][node_index] = -1; //큐에 들어와있던건 -1로 유지
		for(i=0;i<10;i++) //다시 초기화
			idg[i] = 10;
		checkIndegree(G, idg); //idg에 값 추가
		for(i=0;idg[i]!=10 && i!=10;i++) //idg 원소들 enqeue
			Enqueue(Q, idg[i]);
	}
	if(sort_count==G->size) //노드가 전부 다 삭제 되었으면 결과 출력
		fprintf(fout, "%s", result);
	else //다 삭제 안되었으면 cycle 존재하는 것으로 에러 출력
		fprintf(fout, "sorting error : cycle!");

	fprintf(fout, "\n"); //모두 출력 후 줄바꿈 EOF
	free(result);
}
Queue* MakeNewQueue(int X){
	Queue* new = (Queue *)malloc(sizeof(struct _Queue)); //메모리 할당
	//초기화
	new->max_queue_size = X;
	new->key = (int *)malloc(sizeof(int)*X);
	new->first = 1;
	new->rear = 0;
	new->qsize = 0;

	return new;
}
int IsEmpty(Queue* Q){
	return Q->qsize == 0;
}
int Dequeue(Queue* Q){
	int temp = Q->key[Q->first];
	Q->first = (Q->first + 1)%(Q->max_queue_size); //배열로 구현하니까 끝에 도달하면 처음으로 가기
	Q->qsize--;

	return temp;
}
void Enqueue(Queue* Q, int X){
	Q->rear = (Q->rear + 1)%(Q->max_queue_size); //배열의 끝에 도달하면 처음으로
	Q->key[Q->rear] = X;
	Q->qsize++;
}
void DeleteQueue(Queue* Q){ //메모리 해제
	free(Q->key);
	free(Q);
}
void checkIndegree(Graph* G, int* idg){ //열 중 모든 값이 0인 값 찾기
	int n = 0;
	int sel = 1;
	for(int i=0;i<G->size;i++){
		if(G->matrix[i][i] == -1) //큐에 들어갔었으면 넘기기
			continue;
		sel = 1;
		for(int j=0;j<G->size;j++){
			if(G->matrix[j][i] != 0){
				sel = 0;
				continue;
			}
		}
		if(sel){
			idg[n] = G->node[i];
			G->matrix[i][i] = -1; //큐에 들어가면 또 들어갈 일 없음
			n++;
		}
	}
}
void sort_node(int* arr, int n){ //insertion sort
	int i, j, key;

	for(i=1;i<n;i++){
		key = arr[i];
		for(j=i-1;j>=0&&arr[j]>key;j--)
			arr[j+1] = arr[j];
		arr[j+1] = key;
	}
}