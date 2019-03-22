#include<stdio.h>
 
int main(int argc,char* argv[]){
   //1.声明文件句柄
   FILE* file;
   //2.声明空buff,用来读取文件中的内容
   char buf[1024] =  {0, };
   //3.内容写入文件的方式为末尾追加，则游标是处于文件末尾位置的
   file = fopen("hello.txt","a+");
   //4.["写入的文本内容",一个字符占用n个字节,被写入文本的字符数,文本写入的file目的地]
   fwrite("hello,bofeng!",1,13,file);
   //5.将文件末尾游标复位到文件最开头的位置，否则 将文件中内容读取到空buf时，buf中数据为空
   rewind(file);
   //6.将file中内容读取到buf 中，1表示一个字符占1个字节，13表示读取从游标开始的n个字符，因程序已经运行过若干遍，所以可以读取 n*13个字符
   fread(buf,1,26,file);
   //7.关闭文件,释放资源很重要，要知道在c/c++中充斥着各种手动close resource 的操作
   fclose(file);
   //8.输出buf中内容
   printf("The file content is: %s\n",buf);
   return 0;
}
