#ifndef ANALYZER_DEFINATIONS_H_INCLUDED
#define ANALYZER_DEFINATIONS_H_INCLUDED

typedef struct          //单词二元组的结构体
{
    int typenum;        //种别码
    char *word;         //对应的值
} WORD;

#include "LexicalAnalyzerPart.h"  //词法分析部分
#include "SyntaxAnalyzerPart.h"    //语法分析部分

#endif // ANALYZERDEFINATIONS_H_INCLUDED
