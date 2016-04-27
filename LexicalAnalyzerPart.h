#ifndef LEXICAL_ANALYZER_PART_H_INCLUDED
#define LEXICAL_ANALYZER_PART_H_INCLUDED


/*********** 词法分析函数 ********************/
/* 每次读取一个token */
WORD* scaner();

/* 从缓冲区读取一个字符 */
char m_getch();

/* 忽略空格回车和制表符 */
void getbc();

/* 将ch连接到token中 */
void concat();

/* 判断ch是否为字母 */
int letter();

/* 判断ch是否为数字 */
int digit();

/* 返回token对应种别码 */
int reserve();

/* 指针回退一个单元 */
void retract();

/* 跳过注释 "/*"达到结尾时无对应结束"* /"返回false ,其他错误和成功读完返回true*/
bool skipComment();


#endif // LEXICAL_ANALYZERPART_H_INCLUDED
