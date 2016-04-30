/*********************************************************
*
*   file:main.cpp
*   description:entry of program
*
*   结合上一阶段词法分析的设计（project LexicalAnalyzer），在其基础上
*   进行递归下降子函数法的语法分析的设计
*
**********************************************************/
#include "AnalyzerDefinations.h"

int p_input;             //输入单词的缓冲区指针
char input[1024];        //输入单词的缓冲区
int currentLine=1;       //记录当前读取token所在的行号
int Syn=1;               //暂存种别码
int cError =0;           //语法错误个数
WORD* oneword = new WORD;//暂存token

int main()
{
    memset(input,0,sizeof(char)*1024);//保证将输入缓冲区所有位置置为'\0'
    printf("Enter Your words(end with #):\n");
    scanf("%[^#]s",input);     //从控制台读入源代码，以#结束，可以多行输入
    strcat(input,"\n");
    p_input = 0;

    //将input中的内容加上行号显示
    printf("Your words:\n");
    bool isNewLine=true;
    int line,i;
    for(line=1,i=0;input[i]!='\0';i++){
        if(isNewLine){
            printf("%-3d|",line++);
            isNewLine=false;
        }
        printf("%c",input[i]);
        if(input[i]==10)isNewLine=true;
    }
    printf("\n");

    //进入词法语法分析
    Program();

    printf("\npress # to exit:");
    scanf("%[^#]s",input);
}

/** 测试用例 *************
int main()
{
    int a;  int b;  int i;          //声明时不能同时赋值
    a=0;    b=0;
    for( i=0 ; i<100 ; i=i+1)       //for循环，可嵌套
    {
        if( i<10 ){ continue; }     //if和else 语句必须有{}符号
        else { b= a*(a+b*(a+3));}

        while(b<200)                //while循环
        {
            b=(a+b)*i;
            if(i>50){break;}
            if(i==99){return 1;}
        }
    }
    return 0;
}#
*/
