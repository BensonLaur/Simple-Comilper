/*********************************************************
*
*   file:main.cpp
*   description:entry of program
*
*   �����һ�׶δʷ���������ƣ�project LexicalAnalyzer�������������
*   ���еݹ��½��Ӻ��������﷨���������
*
**********************************************************/
#include "AnalyzerDefinations.h"

int p_input;             //���뵥�ʵĻ�����ָ��
char input[1024];        //���뵥�ʵĻ�����
int currentLine=1;       //��¼��ǰ��ȡtoken���ڵ��к�
int Syn=1;               //�ݴ��ֱ���
int cError =0;           //�﷨�������
WORD* oneword = new WORD;//�ݴ�token

int main()
{
    memset(input,0,sizeof(char)*1024);//��֤�����뻺��������λ����Ϊ'\0'
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

    //����ʷ��﷨����
    Program();

    printf("\npress # to exit:");
    scanf("%[^#]s",input);
}

/** �������� *************
int main()
{
    int a;  int b;  int i;          //����ʱ����ͬʱ��ֵ
    a=0;    b=0;
    for( i=0 ; i<100 ; i=i+1)       //forѭ������Ƕ��
    {
        if( i<10 ){ continue; }     //if��else ��������{}����
        else { b= a*(a+b*(a+3));}

        while(b<200)                //whileѭ��
        {
            b=(a+b)*i;
            if(i>50){break;}
            if(i==99){return 1;}
        }
    }
    return 0;
}#
*/
