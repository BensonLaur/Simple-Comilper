#ifndef LEXICAL_ANALYZER_PART_H_INCLUDED
#define LEXICAL_ANALYZER_PART_H_INCLUDED


/*********** �ʷ��������� ********************/
/* ÿ�ζ�ȡһ��token */
WORD* scaner();

/* �ӻ�������ȡһ���ַ� */
char m_getch();

/* ���Կո�س����Ʊ�� */
void getbc();

/* ��ch���ӵ�token�� */
void concat();

/* �ж�ch�Ƿ�Ϊ��ĸ */
int letter();

/* �ж�ch�Ƿ�Ϊ���� */
int digit();

/* ����token��Ӧ�ֱ��� */
int reserve();

/* ָ�����һ����Ԫ */
void retract();

/* ����ע�� "/*"�ﵽ��βʱ�޶�Ӧ����"* /"����false ,��������ͳɹ����귵��true*/
bool skipComment();


#endif // LEXICAL_ANALYZERPART_H_INCLUDED
