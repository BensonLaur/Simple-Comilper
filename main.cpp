/***
    �����һ�׶δʷ���������ƣ�project LexicalAnalyzer�������������
    ���еݹ��½��Ӻ��������﷨���������
*/
#include <stdio.h>
#include <string.h>
#include "AnalyzerDefinations.h"

int p_input;             //���뵥�ʵĻ�����ָ��
char input[1024];        //���뵥�ʵĻ�����
int currentLine=1;       //��¼��ǰ��ȡtoken���ڵ��к�

int main()
{
    int over=1;
    WORD* oneword = new WORD;
    printf("Enter Your words(end with #):\n");
    scanf("%[^#]s",input);     //�ӿ���̨����Դ���룬��#���������Զ�������
    strcat(input,"\n");
    p_input = 0;

    //��input�е����ݼ����к���ʾ
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

    //�ʷ��������﷨����
    int countOfToken=0;
    while(over<1000 ) //over<1000 ʱΪ��Ч�ֱ���
    {
        oneword = scaner();                     //��ȡһ��token
        //if(countOfToken++%4==0)printf("\n");    //ÿ�����4�Զ�Ԫ��
        //if(oneword->typenum < 1000 && oneword->typenum>0)             //Ϊ��Ч�ֱ���ʱ�����Ԫ��
        //    printf("(%2d,%8s)\t",oneword->typenum,oneword->word);
        over = oneword->typenum;

        if(over==-1)        //over=-1ʱΪ�ֱ���ʶ�����ʱ��ֵ
        {
            printf("\nLine [%3d] error:�޷�ʶ�� %s\n",currentLine,oneword->word);
            countOfToken==0;
        }else if(over ==-2) //over=-2ʱΪ��һ��'/'���µ�ע�ʹ���
        {
            printf("\nLine [%3d] error:���ڶ������'/' ���� �ɽ��е�һ��'/'���µĴ���ע�� \n",
                   currentLine-1,oneword->word);
            countOfToken==0;
        }
    }
    if(over==1001)printf("\nLine [%3d] error:ע��û�н�β\n",currentLine);

    printf("\npress # to exit:");
    scanf("%[^#]s",input);
}

