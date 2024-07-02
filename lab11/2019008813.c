#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array* new = (Array*)malloc(sizeof(struct Array)); //array 할당
    new->size = size;
    new->values = (int*)malloc(sizeof(int)*size); //값 넣을 배열 할당
    return new; //array 반환
}
void QuickSort(Array* array, int left, int right, int depth){
    if(left<right){ //같으면 1개니까 취급 x
        int pivot;
        pivot = Partition(array, left, right, depth); //쪼갠 pivot의 index
        QuickSort(array, left, pivot-1, depth); //왼쪽 sort
        QuickSort(array, pivot+1, right, depth); //오른쪽 sort
    }
}
int Partition(Array* array, int left, int right, int depth){
    int pivot = array->values[right]; //오른쪽 끝 값 pivot 설정
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right); //초기 값 순서 출력
    int i, j;
    i = left-1; //왼쪽 포인터
    for(j=right;j>i;j--){ //오른쪽 포인터
        if(array->values[j]<pivot){ //오른쪽에서 pivot보다 작으면
            while(array->values[i]<pivot && i<j) //왼쪽에서 pivot보다 큰 index 찾기
                i++;
            if(i!=j){ //둘이 같지 않으면 swap 후 바뀐 순서 출력
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
            }
        }
    }
    if(i+1 != right){ //같지 않으면 swap으로 pivot자리 옮기고 바뀐 순서 출력
        swap(&array->values[i+1], &array->values[right]);
        PrintArray(array, left, right);
    }
    return i+1; //바뀐 pivot 자리 index 반환
}
void PrintArray(Array* array, int left, int right){
    int i;
    for(i=0;i<left;i++) //비어있는 앞부분 '-' 출력
        fprintf(fout, "%-3s", "-");
    for(i=left;i<=right;i++) //정렬 부분 출력
        fprintf(fout, "%-3d", array->values[i]);
    for(i=right+1;i<array->size;i++) //비어있는 뒷 부분 '-' 출력
        fprintf(fout, "%-3s", "-");    
    fprintf(fout, "\n"); //행바꿈
}
void DeleteArray(Array* array){
    free(array->values); //값 저정해놓은 배열 메모리 해제
    free(array); //array 메모리 해제
}
void swap(int* a, int* b){ // 배열 값 swap
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}