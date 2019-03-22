#include <stdio.h>

enum e_type{
    red =10,
    green=20,
    blue,
};    

int main(int argc,char* argv[])
{
    enum e_type et;
    et=blue;

    printf("the color is %d\n", et);
    return 0;
}
