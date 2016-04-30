#ifndef SYNTAX_ANALYZER_PART_H_INCLUDED
#define SYNTAX_ANALYZER_PART_H_INCLUDED

/*********** 语法分析函数 ********************/
//<程序>
void Program();

//<语句串>
void StatementString();

//<普通语句>
void NormalStatement();

//<特定结构语句>
void SpecialStatement();

//<变量声明>
void VariableDeclaration();

//<赋值语句>
void ValueAssignment();

//<判断语句>
void Judgement();

//<loop语句串>
void LoopStatement();

//<else 语句>
void ElseStatement();

//<表达式>
void Expression();

//<项>
void Iterm();

//<因子>
void Factor();

#endif // SYNTAX_ANALYZER_PART_H_INCLUDED
