#include <stdio.h>
#include <string.h>
#include "AnalyzerDefinations.h"

#define _KEY_WORD_END "Ending"  //�ؼ��ֵĽ�����ʶ

extern char input[1024];        //���뵥�ʵĻ�����
char token[1024]="";     //�������ʻ�����
extern int p_input;             //���뵥�ʵĻ�����ָ��
extern int currentLine;       //��¼��ǰ��ȡtoken���ڵ��к�
int p_token;               //�������ʻ�����ָ��
char ch;
char* rwtab[]={"static" ," " ," "  ," " ," " ,
                "void"  ," "," " ,"char"   ,"short",
                "int"   ,"long"  ," "   ,"for"    ,"continue" ,
                " "," "  ," "  ," "     ,"while"  ,
                "break" ,"if"    ,"else"     ,"return" ,_KEY_WORD_END};//��ʶ��Ĺؼ���

char szbuffer[256]; //��ʱ������
/* ÿ�ζ�ȡһ��token */
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
        myword->word = token; //��������ֻ����ַ�����ʽ
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
            sprintf(szbuffer,"[�ַ�:%c]",'!');
            myword->word = (char *)szbuffer;
            return (myword);

            break;
        case '\0':
            myword->typenum = 1000;
            myword->word = "OVER";
            return (myword);

            break;
        default:
                if(ch=='\5')//ע�ͳ���������ֻ��һ��б�ܣ�����-2
                {
                    myword->typenum = -2;
                    myword->word = "InvalidComment";
                }
                else{
                    myword->typenum = -1;
                    sprintf(szbuffer,"[�ַ�:%c]",ch);
                    myword->word = (char *)szbuffer;
                }
                return(myword);
    }

}




/* �ӻ�������ȡһ���ַ� */
char m_getch(){
    ch = input[p_input];
    p_input = p_input +1;
    return (ch);
}

/* ���Կո�س����Ʊ�� */
void getbc(){
    while(ch==' '||ch==10 ||ch=='\t')
    {
        if(ch==10)currentLine+=1;
        ch= input[p_input];
        p_input = p_input+1;
    }
}

/* ��ch���ӵ�token�� */
void concat(){
    token[p_token] = ch;
    p_token = p_token+1;
    token[p_token]='\0';
}

/* �ж�ch�Ƿ�Ϊ��ĸ */
int letter(){
    if((ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z'))return 1;
    else return 0;
}

/* �ж�ch�Ƿ�Ϊ���� */
int digit(){
    if(ch>='0'&&ch<='9' )return 1;
    else return 0;
}

/* ����token��Ӧ�ֱ��� */
int reserve(){
    int i=0;
    while(strcmp(rwtab[i],_KEY_WORD_END)){
        if(!strcmp(rwtab[i],token))
           return i+1;
        i++;
    }
    return 26;
}

/* ָ�����һ����Ԫ */
void retract(){
    p_input = p_input-1;
}

/* ����ע�� "/*"�ﵽ��βʱ�޶�Ӧ����"* /"����false ,��������ͳɹ����귵��true*/
bool skipComment(){
    bool toEnd=false;
    char preCh=ch;
    if(ch=='/')
    {
        ch=m_getch();
        if(ch=='/')         //Ϊ��//��ע��
        {
            while(ch!='\n')
            {
                m_getch();
            }
            getbc();
            return true;
        }
        else if(ch =='*')   //Ϊ��/*  */��ע��
        {
            preCh=' ';
            int tempLine=currentLine;
            while(!(preCh=='*' && ch=='/'))
            {
                if(ch==10)currentLine++;
                if(ch=='\0'){//�����β��δ�������������Ҫ����false
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
            ch='\5';        //����switch(ch)��default ��֧����
            return true;   //�ô������default�д�����Ϊtrue
        }

        //ֻ�е����β��δ���������������
        return false;
    }
}
