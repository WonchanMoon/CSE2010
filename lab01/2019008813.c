#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;


long long sum_between_two(int a, int b) {
    long long sum = 0;
    if(a<b){
        for(int i=a;i<=b;i++){
            sum+=i;
        }
    }
    else if(a>b){
        for(int i=b;i<=a;i++){
            sum+=i;
        }
    }
    else{
        sum=a;
    }
    return sum;
}

int main(int argc, char **argv){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");

    int a,b;
    long long sum;

    while(fscanf(fin, "%d%d", &a, &b)!=EOF){
        sum = sum_between_two(a,b);
        fprintf(fout, "sum of (%d) and (%d) is %lld\n", a,b,sum);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
