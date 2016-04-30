/*********************************************************
*
*   file:SyntaxAnalyzerPart.cpp
*   description:implements the function define in SyntaxAnalyzerPart.h
*
**********************************************************/

#include "AnalyzerDefinations.h"

extern int Syn;               //暂存种别码
extern WORD* oneword;         //暂存token
extern int currentLine;       //记录当前读取token所在的行号
extern int cError;            //语法错误个数
static int LoopFlag=0;        //为for和while语句中的break 和 continue而设计，当进入一层循环结构
                              //则LoopFlag自增1，跳出循环结构则自减1，
                              //这样，通过判断LoopFlag是否大于0 即可知道是否在循环中
//<程序>
void Program()
{
    oneword=LexicalScanner(Syn);
    if(Syn!=11)//int
    {
        printf("\nLine [%3d] error:应有主函数的返回类型int，而不能为[%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn==26)//main
    {
        if(strcmp(oneword->word,"main")!=0)
        {
            printf("\nLine [%3d] error:主函数的名称必须为合法的标识符\"main\"而不能为 [%s]\n",
                   currentLine,oneword->word);
            cError++;
        }
    }
    else
    {
        printf("\nLine [%3d] error:应有主函数的名称\"main\",而不能为 [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=34)//(
    {
        printf("\nLine [%3d] error:此处应为\"(\", 而不能为 [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=35)//)
    {
        printf("\nLine [%3d] error:此处应为\")\", 而不能为 [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=38)//{
    {
        printf("\nLine [%3d] error:此处应为\"{\", 而不能为 [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    StatementString();//<语句串>

    if(Syn!=39)//}
    {
        printf("\nLine [%3d] error:主程序缺少\"}\" \n",currentLine);
        cError++;
    }
    if(Syn!=1000)//还没到结尾'\0'
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=1000)
        {
            printf("\nLine [%3d] error:主程序外从 [%s] 开始(包括其本身)为多余字符\n",
                   currentLine,oneword->word);
            cError++;
        }
        else if(cError==0)
            printf("\nSucceed in Syntax Analysing!!!\n");
    }

    if(cError!=0)
    printf("\n---------总共有 [%d] 个错误-----------\n",cError);

}

//<语句串>
//由 program 调用,SpecialStatement 中的if(){}调用，或 由LoopStatement 调用
void StatementString()
{
    if(Syn==39 || Syn==1000)//如果是 } 或 程序结束标识
        return ;

    //由于LoopStatement 也会调用StatementString,故应该考虑一下LoopStatement中的逻辑
    //LoopStatement会先判断不为 break 或 continue 时调用这里，故应该在此处可以不考虑
    //而在后面读取之后继续则要考虑

    if((Syn==10||Syn==11||Syn==12) || Syn==26)//<基本类型> 或 <ID>
    {
        NormalStatement();

        //NormalStatement()负责扫描完语句（包含符号;）
        //再来判断上一个符号是否为;
        if(Syn!=41)
        {
            printf("\nLine [%3d] error:符号 [%s] 应该有结束符 \";\"\n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
        }

        if(Syn==41 )//; 经过一次读取或错误处理后若得到;,则继续读取一个字符
            oneword=LexicalScanner(Syn);
    }
    else if(Syn==14 || Syn==20 || Syn==22) //for 或 while 或 if
    {
        SpecialStatement();

        //SpecialStatement()负责扫描完语句（包含符号}）
        //还有再读取多} 后面的一个字符（这个设计是为了考虑if(){}else{}语句，
        //详看SpecialStatement()函数的注释）
    }
    else if(Syn==24)//return语句
    {
        //在此处理return 的语义逻辑

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> 或 <NUM> 或 (
        {
            Expression();

            //如果得到的是}，在进入下面的while循环则会正常结束statementString
            if(Syn==41 )//; 正常结束或经过错误处理后若得到;,则继续读取一个字符
                oneword=LexicalScanner(Syn);
        }
        else //其他所有不合法情况(包含;)
        {
            printf("\nLine [%3d] error:return后面必须有表达式，而不能接[%s] ！\n",
               currentLine,oneword->word);
            cError++;
            if(Syn==41 )//; ,由于错误的发生由 ; 本身引起，故直接读下一个字符继续
                oneword=LexicalScanner(Syn);
            else //其他错误，//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
                oneword=ScanWithError(Syn);
        }
    }
    else if(!(Syn==41||(Syn==21 || Syn==15)))//既不是分号;，也不是break 或continue
    {
        printf("\nLine [%3d] error:在 [%s] 处存在无效语句开头！\n",
               currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处

        if(Syn==41 )//; 经过错误处理后若得到;,则继续读取一个字符
            oneword=LexicalScanner(Syn);
    }
    else //到这里就只能是break 或 continue 或其他不合语法的词了(比如<NUM>)
    {
        if(LoopFlag==0)//没有在循环中，break 和 continue 不允许存在
        {
            if(Syn==21 || Syn==15)
            {
                printf("\nLine [%3d] error: [%s] 用法错误，必须位于循环结构体中！\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处

                if(Syn==41 )//; 经过错误处理后若得到;,则继续读取一个字符
                oneword=LexicalScanner(Syn);
            }
        }
        else {} //在循环中，则到while统一处理


    }

    //如果经过错误处理，或者从SpecialStatement()中扫描结尾得到的是 }，则结束语句串

    while(Syn!=39 && Syn!=1000)//如果不是 }，且没到结束标识， 则进入循环，继续新一轮的识别
    {
        if(LoopFlag==0)//没有在循环中，break 和 continue 不允许存在
        {
            if(Syn==21 || Syn==15)
            {
                printf("\nLine [%3d] error: [%s] 用法错误，必须位于循环结构体中！\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处

                if(Syn==41 )//; 经过错误处理后若得到;,则继续读取一个字符
                oneword=LexicalScanner(Syn);
            }
        }
        else  //LoopFlag>0 ,在循环结构体中
        {
            if(Syn==21 || Syn==15)//若此时存在break, 和continue
            {
                //语义分析时在这里处理break或continue的逻辑

                //处于循环中的statementString 中发现break或continue，则直接返回
                return ;
            }
        }

        if((Syn==10||Syn==11||Syn==12) || Syn==26)// <基本类型> 或 <ID>
        {
            NormalStatement();

            //NormalStatement()负责扫描完语句（包含符号;）
            //再来判断上一个符号是否为;
            if(Syn!=41)
            {
                printf("\nLine [%3d] error:在符号 [%s] 处 应该有结束符 \";\"\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
            }

            if(Syn==41 )//; 经过一次读取或错误处理后若得到;,则继续读取一个字符
                oneword=LexicalScanner(Syn);
        }
        else if(Syn==14 || Syn==20 || Syn==22) //for 或 while 或 if
        {
            SpecialStatement();

            //SpecialStatement()负责扫描完语句（包含符号}）
            //还有再读取多} 后面的一个字符（这个设计是为了考虑if(){}else{}语句，
            //详看SpecialStatement()函数的注释）
        }
        else if(Syn==24)//return语句
        {
            //在此处理return 的语义逻辑

            oneword=LexicalScanner(Syn);
            if(Syn==26||Syn==27||Syn==32)//<ID> 或 <NUM> 或 (
            {
                Expression();

                //如果得到的是}，在进入下一次的while循环则会正常结束statementString
                if(Syn==41 )//; 正常结束或经过错误处理后若得到;,则继续读取一个字符
                    oneword=LexicalScanner(Syn);
            }
            else //其他所有不合法情况(包含;)
            {
                printf("\nLine [%3d] error:return后面必须有表达式，而不能接[%s] ！\n",
                    currentLine,oneword->word);
                cError++;
                if(Syn==41 )//; ,由于错误的发生由 ; 本身引起，故直接读下一个字符继续
                    oneword=LexicalScanner(Syn);
                else //其他错误，//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
                    oneword=ScanWithError(Syn);
            }
        }
        else if(!(Syn==41||(Syn==21 || Syn==15)))//既不是分号;，也不是break 或continue
        {
            printf("\nLine [%3d] error:在 [%s] 处存在无效语句开头！\n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处

            if(Syn==41 )//; 经过错误处理后若得到;,则继续读取一个字符
            oneword=LexicalScanner(Syn);
        }
        else //到这里就只能是break 或 continue了
        {
            if(LoopFlag==0)//没有在循环中，break 和 continue 不允许存在
            {
                if(Syn==21 || Syn==15)
                {
                    printf("\nLine [%3d] error: [%s] 用法错误，必须位于循环结构体中！\n",
                        currentLine,oneword->word);
                    cError++;
                    oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处

                    if(Syn==41 )//; 经过错误处理后若得到;,则继续读取一个字符
                    oneword=LexicalScanner(Syn);
                }
            }
            else {} //在循环中，则到while统一处理
        }

        if(Syn==41)//; 经过一次读取或错误处理后若得到;,则继续读取一个字符
            oneword=LexicalScanner(Syn);
    }

    //如果经过错误处理，或者从SpecialStatement()\return 语句中扫描结尾等待得到的是 }，则到此
    //或者是 break，continue 后接;,或其错误处理到}
    return ;
}

//<普通语句>
void NormalStatement()
{
    //if((Syn==10||Syn==11||Syn==12) || Syn==26)
    if(Syn==10||Syn==11||Syn==12) //<基本类型>
    {
        VariableDeclaration();
        if(Syn!=41){
            if(Syn==42)//=
                printf("提示:声明语句不支持赋值操作，由此产生如下错误：",
                    oneword->word);
        }

    }
    else //Syn==26   <ID>
    {
        ValueAssignment();
    }

}

//<特定结构语句> (只由StatementString()调用)
//注意：本来SpecialStatement() 读取到结构自身最后的}便可，但是由于if()语句需要执行
//      多读取一个词，以便继续判断 Else 语句是否存在，故，SpecialStatement统一多读一个词
void SpecialStatement()
{
    //if(Syn==14 || Syn==20 || Syn==22) //for 或 while 或 if
    if(Syn==14)//for
    {
        LoopFlag+=1;
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for 语句后面应该接\"(\"，而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26)//<ID>
            ValueAssignment();

        if(Syn!=41)//;
        {
            if(Syn==10 || Syn==11 || Syn==12)
                printf("\nLine [%3d] error:for型语句不支持在for中声明[%s] ",
                    currentLine,oneword->word);
            else
                printf("\nLine [%3d] error:for(s1;s2;s3)结构中s1后应为\";\"而非 [%s] \n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> 或 <NUM> 或 (
            Judgement();

        if(Syn!=41)//;
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)结构中s2后应为\";\"而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26)//<ID>
            ValueAssignment();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)结构中s3 [%s]处使用错误! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            LoopStatement();

            //LoopStatement() 负责扫描完整的for结构语句（包括}）
            //如果没有}，StatementString() 负责报错
        }
        else
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)结构中)后应为\"{\"而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        LoopFlag-=1;
        oneword=LexicalScanner(Syn);//多读一个词
    }
    else if(Syn==20)//while
    {
        LoopFlag+=1;
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for 语句后面应该接\"(\"，而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> 或 <NUM> 或 (
            Judgement();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:while(s1)结构中s1 [%s]处使用错误! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            LoopStatement();

            //LoopStatement() 负责扫描完整的for结构语句（包括}）
            //如果没有}，StatementString() 负责报错
        }
        else
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)结构中)后应为\"{\"而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        LoopFlag-=1;
        oneword=LexicalScanner(Syn);//多读一个词
    }
    else //if
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for 语句后面应该接\"(\"，而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> 或 <NUM> 或 (
            Judgement();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:if(s1)结构中s1 [%s]处使用错误! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            if(LoopFlag==0)//if不在循环中
                StatementString();
            else //if在循环中
                LoopStatement();
            //StatementString() 负责扫描完整的if结构语句（包括}）
            //如果没有}，返回后的 StatementString() 负责报错
        }
        else
        {
            printf("\nLine [%3d] error:规定if(s1){}结构中)后应为\"{\"而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }

        //执行Else可能存在的语句
        ElseStatement();

        //到此为读取到了if(){}结构的}后面一个字符，或else{} 结构的}后面一个字符
    }

}

//<判断语句>
void Judgement()
{
    char * tempWord;
    Expression();

    if(!(Syn>=43 && Syn<=48)) //不为判断符号
        if(Syn!=41 && Syn!=35) //也不为; 也不为),则返回，由调用者做错误扫描到}处理
            return ;
        else //为;, 返回后会继续执行，但是提示不建议这么做
        {
            printf("提示：Line [%3d] 不建议在判断语句中使用非判断语句!\n",currentLine);
            return ;
        }
    tempWord = oneword->word;

    oneword=LexicalScanner(Syn);
    if(!(Syn==26||Syn==27||Syn==32))//<ID> 或 <NUM> 或 (
        if(Syn!=41 && Syn!=35) //也不为; 也不为),则返回，由调用者做错误扫描到}处理
            return ;
        else
        {
            printf("\nLine [%3d] error:判断语句使用出错, [%s] 后不应该接 [%s] \n",
               currentLine,tempWord,oneword->word);
            cError++;
            return;//报错完毕，在当前为;情况下直接返回，在for中完成了继续处理
                    //在 if 语句中需要额外判断 是否为 ; ,是则报错
        }

    Expression();

}

//<loop语句串>
void LoopStatement()
{
    if(Syn==39 || Syn==1000)//如果是 } 或 程序结束标识
        return ;

    char* tempWord=oneword->word;
    if(Syn==21 || Syn==15)//break;continue;
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=41)//;
        {
            printf("\nLine [%3d] error:在[%s] 后应该有\";\",而不为 [%s] \n",
               currentLine,tempWord,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            return;
        }

        //继续读取
        oneword=LexicalScanner(Syn);
    }
    else
        StatementString();

    while(Syn!=39 && Syn!=1000 )//不为}，或不为程序结束
    {
        if(Syn==21 || Syn==15)//break;continue;
        {
            oneword=LexicalScanner(Syn);
            if(Syn!=41)//;
            {
                printf("\nLine [%3d] error:在[%s] 后应该有\";\",而不为 [%s] \n",
                currentLine,tempWord,oneword->word);
                cError++;
                oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
                return;
            }

            //继续读取
            oneword=LexicalScanner(Syn);
        }
        else
            StatementString();
    }
}

//<else 语句>
void ElseStatement()
{
    //能到此则当前符号为 }，继续读取下一个
    oneword=LexicalScanner(Syn);

    if(Syn==23)//else
    {
        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            StatementString();

            //StatementString() 负责扫描完整的else结构语句（包括}）
            //如果没有}，返回到SpecialStatement()，后再到StatementString() 负责报错

            oneword=LexicalScanner(Syn);//扫描多一个
        }
        else
        {
            printf("\nLine [%3d] error:规定if(s1){}else{}结构中else后应为\"{\"而非 [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//扫描跳至下一个  } 的地方
            oneword=LexicalScanner(Syn);//扫描多一个
            return;
        }
    }
    else
    {
        //不做处理，直接返回
    }
}

//<变量声明>
void VariableDeclaration()
{
    oneword=LexicalScanner(Syn);

    if(Syn!=26)//<ID>
    {
        printf("\nLine [%3d] error:声明语句的基本类型后面应该有合法标识符，而不应为 [%s]\n",
                    currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
        return ;
    }

    oneword=LexicalScanner(Syn);//正常情况下期望出现;
}

//<赋值语句>
void ValueAssignment()
{
    oneword=LexicalScanner(Syn);
    if(Syn!=42) // =
    {
        if(Syn>=28 && Syn<=31)//+ - * /
            return ; //做类似于a++ a--  a+=1; a*=2; 时不提示，直接等待返回后的报错
        else
            printf("提示:Line[%3d] 以标识符开头的语句应为赋值语句,用法不推荐！\n",currentLine);
        return;
    }

    oneword=LexicalScanner(Syn);
    Expression();
}

//<表达式>
void Expression()
{
    Iterm();
    while(Syn==28 || Syn==29) // + -
    {
        oneword=LexicalScanner(Syn);
        Iterm();
    }
}

//<项>
void Iterm()
{
    Factor();
    while(Syn==30 || Syn==31)// * /
    {
        oneword=LexicalScanner(Syn);
        Factor();
    }
}

//<因子>
void Factor()
{
    if(Syn==26)//<ID>
    {
        oneword=LexicalScanner(Syn);
        return;
    }
    if(Syn==27)//<NUM>
    {
        oneword=LexicalScanner(Syn);
        return;
    }
    if(Syn==34)//(
    {
        oneword=LexicalScanner(Syn);
        Expression();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:在 [%s] 处应有符号\")\"\n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
            return ;
        }

        //上一个为），再读取一个
        oneword=LexicalScanner(Syn);
    }
    else  //不为上述三种情况
    {
        printf("\nLine [%3d] error:表达式格式错误，在 [%s] 处应有合法表达式！\n",
                    currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//扫描跳至下一个 ; 或者 } 的地方 或者是特殊句型开始处
        return ;
    }
}


