#include<stdio.h>
#include<stdlib.h>
 
int main(int argc,char* argv[]){
   //1.声明指针变量a，个人更倾向于使用 ［类型*］ 变量名，而不擅长使用[类型] *变量名，前者更加直观，含义即 指向某类型变量的指针
   int *a;
   int *b;
 
   a = (int*)malloc(sizeof(int));//在堆内存中分配int类型数据占内存大小的空间，并将内存空间首地址赋给a
   b = (int*)malloc(sizeof(int));//同上
 
   *a = 1;
   *b = 2;
    
   int c[3]={1,2,3};
 
   printf("addr of a: %p,a saved addr: %p,a points value %d\n",&a,a,*a);//指针的指针，即存放指针的空间地址,指针变量存储的地址值，指针指向的内存区域存储的vlaue
   printf("addr of b: %p,b saved addr: %p,b points value %d\n",&b,b,*b);//同上
    
   printf("addr of c: %p,c saved addr: %p,c value1: %d,c value2: %d\n",&c,c,*c,c[2]);
   return 0;
 
}
