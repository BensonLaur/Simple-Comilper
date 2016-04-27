/***
    结合上一阶段词法分析的设计（project LexicalAnalyzer），在其基础上
    进行递归下降子函数法的语法分析的设计
*/
#include <stdio.h>
#include <string.h>
#include "AnalyzerDefinations.h"

int p_input;             //输入单词的缓冲区指针
char input[1024];        //输入单词的缓冲区
int currentLine=1;       //记录当前读取token所在的行号

int main()
{
    int over=1;
    WORD* oneword = new WORD;
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

    //词法分析与语法分析
    int countOfToken=0;
    while(over<1000 ) //over<1000 时为有效种别码
    {
        oneword = scaner();                     //读取一个token
        //if(countOfToken++%4==0)printf("\n");    //每行输出4对二元组
        //if(oneword->typenum < 1000 && oneword->typenum>0)             //为有效种别码时输出二元组
        //    printf("(%2d,%8s)\t",oneword->typenum,oneword->word);
        over = oneword->typenum;

        if(over==-1)        //over=-1时为种别码识别错误时的值
        {
            printf("\nLine [%3d] error:无法识别 %s\n",currentLine,oneword->word);
            countOfToken==0;
        }else if(over ==-2) //over=-2时为由一个'/'导致的注释错误
        {
            printf("\nLine [%3d] error:存在多余符号'/' 或者 由仅有的一个'/'导致的错误注释 \n",
                   currentLine-1,oneword->word);
            countOfToken==0;
        }
    }
    if(over==1001)printf("\nLine [%3d] error:注释没有结尾\n",currentLine);

    printf("\npress # to exit:");
    scanf("%[^#]s",input);
}

