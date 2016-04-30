#ifndef SYNTAX_ANALYZER_PART_H_INCLUDED
#define SYNTAX_ANALYZER_PART_H_INCLUDED

/*********** �﷨�������� ********************/
//<����>
void Program();

//<��䴮>
void StatementString();

//<��ͨ���>
void NormalStatement();

//<�ض��ṹ���>
void SpecialStatement();

//<��������>
void VariableDeclaration();

//<��ֵ���>
void ValueAssignment();

//<�ж����>
void Judgement();

//<loop��䴮>
void LoopStatement();

//<else ���>
void ElseStatement();

//<���ʽ>
void Expression();

//<��>
void Iterm();

//<����>
void Factor();

#endif // SYNTAX_ANALYZER_PART_H_INCLUDED
