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

extern int currentLine;       //记录当前读取token所在的行号
extern int cError;            //语法错误个数

//正常情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
WORD* LexicalScanner(int &Syn){
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;
    if(Syn==-1)        //Syn=-1时为种别码识别错误时的值
    {
        printf("\nLine [%3d] error:无法识别 %s\n",currentLine,oneword->word);
        cError++;
        oneword = scaner();
        Syn = oneword->typenum;

    }
    else if(Syn ==-2) //Syn=-2时为由一个'/'导致的注释错误
    {
        printf("\nLine [%3d] error:存在多余符号'/' 或者 由仅有的一个'/'导致的错误注释 \n",
                currentLine-1,oneword->word);
        cError++;
        oneword = scaner();
        Syn = oneword->typenum;
    }
    if(Syn==1001){
        printf("\nLine [%3d] error:注释没有结尾\n",currentLine);
        cError++;
    }

    return (oneword);
}

//出错情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
WORD* ScanWithError(int &Syn)
{
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;

    //扫描不到 ; 或者 } 的地方 或者是特殊句型开始处，则继续扫描
    while( !(Syn==41 || Syn== 39|| (Syn==14 || Syn==20 || Syn==22)) )
    {
        if(Syn==1000)break; //如果程序结束了，跳出扫描
        oneword = scaner();
        Syn = oneword->typenum;
    }

    return (oneword);
}

//出错情况下使用的 词法扫描器，返回新扫描到的词，Syn 返回返回词的 种别码
//扫描跳至下一个  } 的地方 或者是特殊句型开始处
WORD* ScanWithErrorToBlockEnd(int &Syn)
{
    WORD* oneword = new WORD;
    oneword = scaner();
    Syn = oneword->typenum;

    //扫描不到 } ，则继续扫描
    while( Syn != 39 )//}
    {
        if(Syn==1000)break; //如果程序结束了，跳出扫描
        oneword = scaner();
        Syn = oneword->typenum;
    }

    return (oneword);
}
