#ifndef ANALYZER_DEFINATIONS_H_INCLUDED
#define ANALYZER_DEFINATIONS_H_INCLUDED

typedef struct          //���ʶ�Ԫ��Ľṹ��
{
    int typenum;        //�ֱ���
    char *word;         //��Ӧ��ֵ
} WORD;

#include "LexicalAnalyzerPart.h"  //�ʷ���������
#include "SyntaxAnalyzerPart.h"    //�﷨��������

#endif // ANALYZERDEFINATIONS_H_INCLUDED
