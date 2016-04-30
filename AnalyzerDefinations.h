#ifndef ANALYZER_DEFINATIONS_H_INCLUDED
#define ANALYZER_DEFINATIONS_H_INCLUDED

typedef struct          //单词二元组的结构体
{
    int typenum;        //种别码
    char *word;         //对应的值
} WORD;

#include <stdio.h>
#include <string.h>
#include "LexicalAnalyzerPart.h"  //词法分析部分函数声明
#include "SyntaxAnalyzerPart.h"    //语法分析部分函数声明

//正常情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
WORD* LexicalScanner(int &Syn);

//出错情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
WORD* ScanWithError(int &Syn);

//出错情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
//扫描跳至下一个  } 的地方 或者是特殊句型开始处
WORD* ScanWithErrorToBlockEnd(int &Syn);

#endif // ANALYZERDEFINATIONS_H_INCLUDED
