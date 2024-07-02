#include<stdio.h>
#include<stdlib.h>
#include<time.h>
// #include<unistd.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char* agrv[]) {
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	int i;

	sets->size_maze = num * num;
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
	for (i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = i;

	maze_print->size_maze = num * num * 2;
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
	for (i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
	//maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x

	//start and end position have not to have the wall
	maze_print->ptr_arr[maze_print->size_maze - 1] = 0;
}

void Union(DisjointSets *sets, int i, int j){
    int set_i, set_j;
    set_i = find(sets, i); // i가 속한 set
    set_j = find(sets, j); // j가 속한 set
    if(set_i == set_j) // 같으면 아무것도 안함.
        return;
    sets->ptr_arr[set_i] = set_j; // 다르면 i가 속한 set이 j가 속한 set으로 포함됨.
}

int find(DisjointSets *sets, int i){
    if(sets->ptr_arr[i] == i) // 인덱스랑 같으면 root
        return i;
    else
        return find(sets, sets->ptr_arr[i]); // 다르면 다음 연결 인덱스 비교
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num){
    int wall, cell1, cell2;
    
    while(find(sets, 1) != find(sets, sets->size_maze)){ // 처음과 끝이 속한 set이 다른가
        // sleep(1);
        while(1){
            wall = rand()%(maze_print->size_maze) + 1; // 랜덤한 벽 뽑기. 1 ~ num*num*2

            if(maze_print->ptr_arr[wall]==0) // 0이면 이미 뚫린 벽
                continue;
            else if( (num*num-num < (wall+1)/2) && !(wall%2) ) // 바닥 뚫지 않기. (wall+1)/2는 cell번호
                continue;
            else if( (((wall+1)/2)%num == 0) ) // 오른쪽 벽 뚫지 않기
                continue;
            else
                break;
        }
        // printf("%d\n", wall);
        if (wall%2){ // 홀수면 벽
            cell1 = (wall+1)/2; // 왼쪽
            cell2 = cell1 + 1; // 오른쪽
            }
        else{ // 짝수면 바닥
            cell1 = wall/2; // 위
            cell2 = wall/2 + num; // 아래
        }

        if(find(sets, cell1) != find(sets, cell2)){ // 두개가 disjoint면 합치기
            Union(sets, cell1, cell2);
            maze_print->ptr_arr[wall] = 0; // 합쳤으면 벽 뚫기
            // printf("%d %d\n", cell1, cell2);
        }
    }
}

void printMaze(DisjointSets *sets, int num){
    int i, j;
    // 맨 윗 줄 출력
    fprintf(fout, "+");
    for(i=1;i<=num;i++)
        fprintf(fout, "---+");
    fprintf(fout, "\n");
    // 행마다 출력
    for(i=1; i<=2*num ;i++){ // i는 행
        if(i%2){ // 홀수면 벽
            if(i==1)
                fprintf(fout, " ");
            else
                fprintf(fout, "|");
            for(j=1;j<=num;j++){ // j는 열
                if(sets->ptr_arr[(j+(i/2)*num)*2-1]) // 벽이 있는가?
                    fprintf(fout, "   |");
                else
                    fprintf(fout, "    ");
            }
            fprintf(fout, "\n");
        }
        else{ // 짝수면 바닥
            fprintf(fout, "+");
            for(j=1;j<=num;j++){ // j는 열
                if(sets->ptr_arr[(j+(i/2-1)*num)*2]) // 바닥이 있는가?
                    fprintf(fout, "---+");
                else
                    fprintf(fout, "   +");
            }
            fprintf(fout, "\n");
        }
    }
}

void freeMaze(DisjointSets *sets, DisjointSets *maze_print){ // free
    free(sets->ptr_arr);
    free(sets);
    free(maze_print->ptr_arr);
    free(maze_print);
}