#include <stdio.h>
#include <string.h>
#include "AnalyzerDefinations.h"

#define _KEY_WORD_END "Ending"  //关键字的结束标识

extern char input[1024];        //输入单词的缓冲区
char token[1024]="";     //单个单词缓冲区
extern int p_input;             //输入单词的缓冲区指针
extern int currentLine;       //记录当前读取token所在的行号
int p_token;               //单个单词缓冲区指针
char ch;
char* rwtab[]={"static" ," " ," "  ," " ," " ,
                "void"  ," "," " ,"char"   ,"short",
                "int"   ,"long"  ," "   ,"for"    ,"continue" ,
                " "," "  ," "  ," "     ,"while"  ,
                "break" ,"if"    ,"else"     ,"return" ,_KEY_WORD_END};//能识别的关键字

char szbuffer[256]; //临时缓冲区
/* 每次读取一个token */
WORD* scaner(){
    bool skipResult;
    WORD* myword = new WORD;
    //myword->typenum = 10;
    //myword->word = "";
    p_token = 0;
    m_getch();
    getbc();
    while(ch=='/'){
      skipResult=skipComment();
      if(!skipResult){
        myword->typenum = 1001;
        myword->word = "CommentNoEnd";
        return (myword);
      }
    }
    if(letter()){
        while(letter()||digit()){
            concat();
            m_getch();
        }
        retract();
        myword->typenum = reserve();
        myword->word = token;
        return (myword);
    }
    else if(digit())
    {
        while(digit())
        {
            concat();
            m_getch();
        }
        retract();
        myword->typenum =27;
        myword->word = token; //这里的数字还是字符串形式
        return (myword);
    }
    else switch(ch){
        case '+':
            myword->typenum = 28;
            myword->word = "+";
            return (myword);

            break;
        case '-':
            myword->typenum = 29;
            myword->word = "-";
            return (myword);
            break;
        case '*':
            myword->typenum = 30;
            myword->word = "*";
            return (myword);
            break;
        case '/':
            myword->typenum = 31;
            myword->word = "/";
            return (myword);
            break;
        case '(':
            myword->typenum = 34;
            myword->word = "(";
            return (myword);
            break;
        case ')':
            myword->typenum = 35;
            myword->word = ")";
            return (myword);
            break;
        case '{':
            myword->typenum = 38;
            myword->word = "{";
            return (myword);
            break;
        case '}':
            myword->typenum = 39;
            myword->word = "}";
            return (myword);
            break;
        case ';':
            myword->typenum = 41;
            myword->word = ";";
            return (myword);
            break;
        case '=':
            m_getch();
            if(ch=='=')
            {
                myword->typenum = 43;
                myword->word = "==";
                return (myword);
            }
            retract();
            myword->typenum = 42;
            myword->word = "=";
            return (myword);

            break;
        case '>':
            m_getch();
            if(ch=='=')
            {
                myword->typenum = 44;
                myword->word = ">=";
                return (myword);
            }
            retract();
            myword->typenum = 46;
            myword->word = ">";
            return (myword);

            break;
        case '<':
            m_getch();
            if(ch=='=')
            {
                myword->typenum = 45;
                myword->word = "<=";
                return (myword);
            }
            retract();
            myword->typenum = 47;
            myword->word = "<";
            return (myword);

        case '!':
            m_getch();
            if(ch=='=')
            {
                myword->typenum = 48;
                myword->word = "!=";
                return (myword);
            }
            retract();
            myword->typenum = -1;
            sprintf(szbuffer,"[字符:%c]",'!');
            myword->word = (char *)szbuffer;
            return (myword);

            break;
        case '\0':
            myword->typenum = 1000;
            myword->word = "OVER";
            return (myword);

            break;
        default:
                if(ch=='\5')//注释出错的情况，只有一个斜杠，返回-2
                {
                    myword->typenum = -2;
                    myword->word = "InvalidComment";
                }
                else{
                    myword->typenum = -1;
                    sprintf(szbuffer,"[字符:%c]",ch);
                    myword->word = (char *)szbuffer;
                }
                return(myword);
    }

}




/* 从缓冲区读取一个字符 */
char m_getch(){
    ch = input[p_input];
    p_input = p_input +1;
    return (ch);
}

/* 忽略空格回车和制表符 */
void getbc(){
    while(ch==' '||ch==10 ||ch=='\t')
    {
        if(ch==10)currentLine+=1;
        ch= input[p_input];
        p_input = p_input+1;
    }
}

/* 将ch连接到token中 */
void concat(){
    token[p_token] = ch;
    p_token = p_token+1;
    token[p_token]='\0';
}

/* 判断ch是否为字母 */
int letter(){
    if((ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z'))return 1;
    else return 0;
}

/* 判断ch是否为数字 */
int digit(){
    if(ch>='0'&&ch<='9' )return 1;
    else return 0;
}

/* 返回token对应种别码 */
int reserve(){
    int i=0;
    while(strcmp(rwtab[i],_KEY_WORD_END)){
        if(!strcmp(rwtab[i],token))
           return i+1;
        i++;
    }
    return 26;
}

/* 指针回退一个单元 */
void retract(){
    p_input = p_input-1;
}

/* 跳过注释 "/*"达到结尾时无对应结束"* /"返回false ,其他错误和成功读完返回true*/
bool skipComment(){
    bool toEnd=false;
    char preCh=ch;
    if(ch=='/')
    {
        ch=m_getch();
        if(ch=='/')         //为“//”注释
        {
            while(ch!='\n')
            {
                m_getch();
            }
            getbc();
            return true;
        }
        else if(ch =='*')   //为“/*  */”注释
        {
            preCh=' ';
            int tempLine=currentLine;
            while(!(preCh=='*' && ch=='/'))
            {
                if(ch==10)currentLine++;
                if(ch=='\0'){//到达结尾还未结束的情况，需要返回false
                    toEnd=true;
                    currentLine=tempLine;
                    break;
                }
                preCh = ch;
                ch = m_getch();
            }
            if(!toEnd){
                m_getch();
                getbc();
                return true;
            }
        }
        else {
            while(ch!='\n')
            {
                m_getch();
            }
            getbc();
            retract();
            ch='\5';        //进入switch(ch)的default 分支处理
            return true;   //该错误进入default中处理，故为true
        }

        //只有到达结尾还未结束的情况到这里
        return false;
    }
}
