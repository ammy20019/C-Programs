#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void myfunction(){
    printf("this is a vpid function! you are not getting anythin here! bYE!");
}
int main(){
    char name[] = "Suzy";
    int age = 24;
    printf("There was a women named %s \n", name);
    printf("She was %d years old. \n", age);
    double money=2000.20;
    printf("She currently has Rs %.2f with her in savings bank account \n", money);
    char first_letter = 'S';
    printf("The first letter of her name is %c \n", first_letter);

    printf("%f", pow(2, 3) );

    printf("%f \n", sqrt (36));

    printf("%f \n", ceil(30.2));

    printf("%f \n", floor(30.2));
    myfunction();
    return 0;
}