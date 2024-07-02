#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

//greatest common division
int gcd(int a, int b){
    if(a>b){
        int c=a;
        a=b;
        b=c;
    }   
    int result;
    if(a==0){
        a=b;
    }

    for(int i=1;i<=a;i++){
        if(a%i==0 && b%i==0){
            result=i;
        }
    }
    return result;
}

//least common multiple
int lcm(int a, int b){
    int result = a*b/gcd(a,b);
    return result;
}

int main(int argc, char **argv){
    fin=fopen(argv[1],"r");
    fout=fopen(argv[2], "w");

    int a,b;
    int gcd_value;
    int lcm_value;

    while(fscanf(fin,"%d%d", &a, &b)!=EOF){
        gcd_value=gcd(a,b);
        fprintf(fout, "gcd value:%d ", gcd_value);
        lcm_value=lcm(a,b);
        fprintf(fout, "lcm value:%d\n", lcm_value);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
