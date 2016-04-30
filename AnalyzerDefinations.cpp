/*********************************************************
*
*   file:AnalyzerDefinations.cpp
*   description:implements the functions define in AnalyzerDefinations.h
*               including three kind of scaner corresponding to three situations.
*
*   3 situations:
*       LexicalScanner: simply get the next token,and show a error if necessary.
*       ScanWithError: usually called when meet a processing error, it will skip
                       the current error to the next ";" or "}" met first.
*       ScanWithErrorToBlockEnd:similar to above one, only diff is scan to
                                meet the first }.
*
**********************************************************/
#include "AnalyzerDefinations.h"

extern int currentLine;       //��¼��ǰ��ȡtoken���ڵ��к�
extern int cError;            //�﷨�������

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
WORD* LexicalScanner(int &Syn){
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;
    if(Syn==-1)        //Syn=-1ʱΪ�ֱ���ʶ�����ʱ��ֵ
    {
        printf("\nLine [%3d] error:�޷�ʶ�� %s\n",currentLine,oneword->word);
        cError++;
        oneword = scaner();
        Syn = oneword->typenum;

    }
    else if(Syn ==-2) //Syn=-2ʱΪ��һ��'/'���µ�ע�ʹ���
    {
        printf("\nLine [%3d] error:���ڶ������'/' ���� �ɽ��е�һ��'/'���µĴ���ע�� \n",
                currentLine-1,oneword->word);
        cError++;
        oneword = scaner();
        Syn = oneword->typenum;
    }
    if(Syn==1001){
        printf("\nLine [%3d] error:ע��û�н�β\n",currentLine);
        cError++;
    }

    return (oneword);
}

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
WORD* ScanWithError(int &Syn)
{
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;

    //ɨ�費�� ; ���� } �ĵط� ������������Ϳ�ʼ���������ɨ��
    while( !(Syn==41 || Syn== 39|| (Syn==14 || Syn==20 || Syn==22)) )
    {
        if(Syn==1000)break; //�����������ˣ�����ɨ��
        oneword = scaner();
        Syn = oneword->typenum;
    }

    return (oneword);
}

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
//ɨ��������һ��  } �ĵط� ������������Ϳ�ʼ��
WORD* ScanWithErrorToBlockEnd(int &Syn)
{
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;

    //ɨ�費�� } �������ɨ��
    while( Syn != 39 )//}
    {
        if(Syn==1000)break; //�����������ˣ�����ɨ��
        oneword = scaner();
        Syn = oneword->typenum;
    }

    return (oneword);
}
