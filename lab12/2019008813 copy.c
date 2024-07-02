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
void Union(DisjointSets* sets, int i, int j) {
    int r1 = find(sets, i); // i를 가진 set의 root
    int r2 = find(sets, j); // j를 가진 set의 root 

    if (sets->ptr_arr[r1] > sets->ptr_arr[r2]) // r2가 r1보다 height가 높은 경우(height는 disk공간을 줄이기 위해 음수로 표시)   
        sets->ptr_arr[r1] = r2; // r1을 r2에 연결함
    else {
        if (sets->ptr_arr[r1] == sets->ptr_arr[r2]) // r1과 r2의 height가 같은 경우
            sets->ptr_arr[r1]--; // r1의 height를 1 높임(음수이므로 -1)
        sets->ptr_arr[r2] = r1; // r2를 r1에 연결함
    }
}

int find(DisjointSets* sets, int i) {
    while (sets->ptr_arr[i] > 0) // 부모 노드가 존재하는 경우 반복
        i = sets->ptr_arr[i]; // 부모 노드로 i를 업데이트하여 다시 탐색
    return i; // root노드가 나오면 리턴
}

void createMaze(DisjointSets* sets, DisjointSets* maze_print, int num) {
    while (find(sets, 1) != find(sets, sets->size_maze)) {
        int wall_condition = 1, a, b, wall;
        while(wall_condition) { // wall_condition이 1인 경우 반복
            wall = rand() % maze_print->size_maze + 1; // 뚫을 벽의 인덱스를 난수를 받아서 생성
            a = (wall + 1) / 2; // 벽의 왼쪽 또는 위 노드
            if (maze_print->ptr_arr[wall] == 0) { // 벽이 이미 뚫려 있는 경우
                continue;
            }
            else if (wall % 2 == 1 && a % num == 0) { // 가장 오른쪽 외벽을 선택한 경우
                continue;
            }
            else if (wall % 2 == 0 && a > (num - 1) * num) { // 가장 아래쪽 외벽을 선택한 경우
                continue;
            }
            else
                wall_condition = 0; // 뚫을 수 있는 벽인 경우 wall_condition에 0을 저장하고 while문 종료
        }
        a = (wall + 1) / 2; // wall의 왼쪽 또는 위 노드
        if (wall % 2 == 0) // 아래 벽을 뚫는 경우
            b = a + num; // wall 아래 노드
        else // 오른쪽 벽을 뚫는 경우
            b = a + 1; // wall 오른쪽 노드
        if (find(sets, a) != find(sets, b)) { // a와 b가 같은 set이 있지 않은 경우
            maze_print->ptr_arr[wall] = 0; // a와 b사이의 벽을 제거
            Union(sets, a, b); // a와 b를 union
        }
    }
}

void printMaze(DisjointSets* sets, int num) {
    for (int i = 1; i <= num; i++) // 가장 위 테두리 출력
        fprintf(fout, "+---");
    fprintf(fout, "+\n");
    
    for (int i = 0; i < num; i++) { // i = 0부터 num - 1까지 반복
        if (i == 0) // 첫번째 줄인 경우 막혀있으면 안되므로 공백 출력
            fprintf(fout, " "); 
        else
            fprintf(fout, "|"); // 나머지는 막혀있으므로 | 출력
        for (int j = 1; j <= num; j++) { // 오른쪽 벽 출력
            if (sets->ptr_arr[2 * i * num + 2 * j - 1] > 0)  // 오른쪽 벽의 값이 1이면
                fprintf(fout, "   |"); // | 출력
            else // 0이면
                fprintf(fout, "    "); // 공백 출력
        }
        fprintf(fout, "\n"); // 줄 출력 완료 후 줄바꿈
        for (int j = 1; j <= num; j++) { // 아래쪽 벽 출력
            fprintf(fout, "+"); // +로 시작
            if (sets->ptr_arr[2 * i * num + 2 * j] > 0) // 아래쪽 벽의 값이 1이면
                fprintf(fout, "---"); // --- 출력
            else // 0인 경우
                fprintf(fout, "   "); // 공백 출력
        }
        fprintf(fout, "+\n"); // + 출력 및 줄바꿈 출력 후 마무리
    }
}

void freeMaze(DisjointSets* sets, DisjointSets* maze_print) {
    free(sets->ptr_arr); // sets->ptr_arr 메모리 해제
    free(maze_print->ptr_arr); // maze_print->ptr_arr 메모리 해제
    free(sets); // sets 메모리 해제
    free(maze_print); // maze print 메모리 해제
}