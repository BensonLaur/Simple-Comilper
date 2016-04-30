#ifndef ANALYZER_DEFINATIONS_H_INCLUDED
#define ANALYZER_DEFINATIONS_H_INCLUDED

typedef struct          //���ʶ�Ԫ��Ľṹ��
{
    int typenum;        //�ֱ���
    char *word;         //��Ӧ��ֵ
} WORD;

#include <stdio.h>
#include <string.h>
#include "LexicalAnalyzerPart.h"  //�ʷ��������ֺ�������
#include "SyntaxAnalyzerPart.h"    //�﷨�������ֺ�������

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
WORD* LexicalScanner(int &Syn);

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
WORD* ScanWithError(int &Syn);

//���������ʹ�õ� �ʷ�ɨ������������ɨ�赽�Ĵʣ�Syn ���ط��شʵ� �ֱ���
//ɨ��������һ��  } �ĵط� ������������Ϳ�ʼ��
WORD* ScanWithErrorToBlockEnd(int &Syn);

#endif // ANALYZERDEFINATIONS_H_INCLUDED
