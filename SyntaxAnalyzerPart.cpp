/*********************************************************
*
*   file:SyntaxAnalyzerPart.cpp
*   description:implements the function define in SyntaxAnalyzerPart.h
*
**********************************************************/

#include "AnalyzerDefinations.h"

extern int Syn;               //�ݴ��ֱ���
extern WORD* oneword;         //�ݴ�token
extern int currentLine;       //��¼��ǰ��ȡtoken���ڵ��к�
extern int cError;            //�﷨�������
static int LoopFlag=0;        //Ϊfor��while����е�break �� continue����ƣ�������һ��ѭ���ṹ
                              //��LoopFlag����1������ѭ���ṹ���Լ�1��
                              //������ͨ���ж�LoopFlag�Ƿ����0 ����֪���Ƿ���ѭ����
//<����>
void Program()
{
    oneword=LexicalScanner(Syn);
    if(Syn!=11)//int
    {
        printf("\nLine [%3d] error:Ӧ���������ķ�������int��������Ϊ[%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn==26)//main
    {
        if(strcmp(oneword->word,"main")!=0)
        {
            printf("\nLine [%3d] error:�����������Ʊ���Ϊ�Ϸ��ı�ʶ��\"main\"������Ϊ [%s]\n",
                   currentLine,oneword->word);
            cError++;
        }
    }
    else
    {
        printf("\nLine [%3d] error:Ӧ��������������\"main\",������Ϊ [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=34)//(
    {
        printf("\nLine [%3d] error:�˴�ӦΪ\"(\", ������Ϊ [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=35)//)
    {
        printf("\nLine [%3d] error:�˴�ӦΪ\")\", ������Ϊ [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    if(Syn!=38)//{
    {
        printf("\nLine [%3d] error:�˴�ӦΪ\"{\", ������Ϊ [%s]\n",
               currentLine,oneword->word);
        cError++;
    }

    oneword=LexicalScanner(Syn);
    StatementString();//<��䴮>

    if(Syn!=39)//}
    {
        printf("\nLine [%3d] error:������ȱ��\"}\" \n",currentLine);
        cError++;
    }
    if(Syn!=1000)//��û����β'\0'
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=1000)
        {
            printf("\nLine [%3d] error:��������� [%s] ��ʼ(�����䱾��)Ϊ�����ַ�\n",
                   currentLine,oneword->word);
            cError++;
        }
        else if(cError==0)
            printf("\nSucceed in Syntax Analysing!!!\n");
    }

    if(cError!=0)
    printf("\n---------�ܹ��� [%d] ������-----------\n",cError);

}

//<��䴮>
//�� program ����,SpecialStatement �е�if(){}���ã��� ��LoopStatement ����
void StatementString()
{
    if(Syn==39 || Syn==1000)//����� } �� ���������ʶ
        return ;

    //����LoopStatement Ҳ�����StatementString,��Ӧ�ÿ���һ��LoopStatement�е��߼�
    //LoopStatement�����жϲ�Ϊ break �� continue ʱ���������Ӧ���ڴ˴����Բ�����
    //���ں����ȡ֮�������Ҫ����

    if((Syn==10||Syn==11||Syn==12) || Syn==26)//<��������> �� <ID>
    {
        NormalStatement();

        //NormalStatement()����ɨ������䣨��������;��
        //�����ж���һ�������Ƿ�Ϊ;
        if(Syn!=41)
        {
            printf("\nLine [%3d] error:���� [%s] Ӧ���н����� \";\"\n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
        }

        if(Syn==41 )//; ����һ�ζ�ȡ�����������õ�;,�������ȡһ���ַ�
            oneword=LexicalScanner(Syn);
    }
    else if(Syn==14 || Syn==20 || Syn==22) //for �� while �� if
    {
        SpecialStatement();

        //SpecialStatement()����ɨ������䣨��������}��
        //�����ٶ�ȡ��} �����һ���ַ�����������Ϊ�˿���if(){}else{}��䣬
        //�꿴SpecialStatement()������ע�ͣ�
    }
    else if(Syn==24)//return���
    {
        //�ڴ˴���return �������߼�

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> �� <NUM> �� (
        {
            Expression();

            //����õ�����}���ڽ��������whileѭ�������������statementString
            if(Syn==41 )//; ���������򾭹�����������õ�;,�������ȡһ���ַ�
                oneword=LexicalScanner(Syn);
        }
        else //�������в��Ϸ����(����;)
        {
            printf("\nLine [%3d] error:return��������б��ʽ�������ܽ�[%s] ��\n",
               currentLine,oneword->word);
            cError++;
            if(Syn==41 )//; ,���ڴ���ķ����� ; �������𣬹�ֱ�Ӷ���һ���ַ�����
                oneword=LexicalScanner(Syn);
            else //��������//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
                oneword=ScanWithError(Syn);
        }
    }
    else if(!(Syn==41||(Syn==21 || Syn==15)))//�Ȳ��Ƿֺ�;��Ҳ����break ��continue
    {
        printf("\nLine [%3d] error:�� [%s] ��������Ч��俪ͷ��\n",
               currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��

        if(Syn==41 )//; ��������������õ�;,�������ȡһ���ַ�
            oneword=LexicalScanner(Syn);
    }
    else //�������ֻ����break �� continue �����������﷨�Ĵ���(����<NUM>)
    {
        if(LoopFlag==0)//û����ѭ���У�break �� continue ���������
        {
            if(Syn==21 || Syn==15)
            {
                printf("\nLine [%3d] error: [%s] �÷����󣬱���λ��ѭ���ṹ���У�\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��

                if(Syn==41 )//; ��������������õ�;,�������ȡһ���ַ�
                oneword=LexicalScanner(Syn);
            }
        }
        else {} //��ѭ���У���whileͳһ����


    }

    //����������������ߴ�SpecialStatement()��ɨ���β�õ����� }���������䴮

    while(Syn!=39 && Syn!=1000)//������� }����û��������ʶ�� �����ѭ����������һ�ֵ�ʶ��
    {
        if(LoopFlag==0)//û����ѭ���У�break �� continue ���������
        {
            if(Syn==21 || Syn==15)
            {
                printf("\nLine [%3d] error: [%s] �÷����󣬱���λ��ѭ���ṹ���У�\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��

                if(Syn==41 )//; ��������������õ�;,�������ȡһ���ַ�
                oneword=LexicalScanner(Syn);
            }
        }
        else  //LoopFlag>0 ,��ѭ���ṹ����
        {
            if(Syn==21 || Syn==15)//����ʱ����break, ��continue
            {
                //�������ʱ�����ﴦ��break��continue���߼�

                //����ѭ���е�statementString �з���break��continue����ֱ�ӷ���
                return ;
            }
        }

        if((Syn==10||Syn==11||Syn==12) || Syn==26)// <��������> �� <ID>
        {
            NormalStatement();

            //NormalStatement()����ɨ������䣨��������;��
            //�����ж���һ�������Ƿ�Ϊ;
            if(Syn!=41)
            {
                printf("\nLine [%3d] error:�ڷ��� [%s] �� Ӧ���н����� \";\"\n",
                    currentLine,oneword->word);
                cError++;
                oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
            }

            if(Syn==41 )//; ����һ�ζ�ȡ�����������õ�;,�������ȡһ���ַ�
                oneword=LexicalScanner(Syn);
        }
        else if(Syn==14 || Syn==20 || Syn==22) //for �� while �� if
        {
            SpecialStatement();

            //SpecialStatement()����ɨ������䣨��������}��
            //�����ٶ�ȡ��} �����һ���ַ�����������Ϊ�˿���if(){}else{}��䣬
            //�꿴SpecialStatement()������ע�ͣ�
        }
        else if(Syn==24)//return���
        {
            //�ڴ˴���return �������߼�

            oneword=LexicalScanner(Syn);
            if(Syn==26||Syn==27||Syn==32)//<ID> �� <NUM> �� (
            {
                Expression();

                //����õ�����}���ڽ�����һ�ε�whileѭ�������������statementString
                if(Syn==41 )//; ���������򾭹�����������õ�;,�������ȡһ���ַ�
                    oneword=LexicalScanner(Syn);
            }
            else //�������в��Ϸ����(����;)
            {
                printf("\nLine [%3d] error:return��������б��ʽ�������ܽ�[%s] ��\n",
                    currentLine,oneword->word);
                cError++;
                if(Syn==41 )//; ,���ڴ���ķ����� ; �������𣬹�ֱ�Ӷ���һ���ַ�����
                    oneword=LexicalScanner(Syn);
                else //��������//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
                    oneword=ScanWithError(Syn);
            }
        }
        else if(!(Syn==41||(Syn==21 || Syn==15)))//�Ȳ��Ƿֺ�;��Ҳ����break ��continue
        {
            printf("\nLine [%3d] error:�� [%s] ��������Ч��俪ͷ��\n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��

            if(Syn==41 )//; ��������������õ�;,�������ȡһ���ַ�
            oneword=LexicalScanner(Syn);
        }
        else //�������ֻ����break �� continue��
        {
            if(LoopFlag==0)//û����ѭ���У�break �� continue ���������
            {
                if(Syn==21 || Syn==15)
                {
                    printf("\nLine [%3d] error: [%s] �÷����󣬱���λ��ѭ���ṹ���У�\n",
                        currentLine,oneword->word);
                    cError++;
                    oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��

                    if(Syn==41 )//; ��������������õ�;,�������ȡһ���ַ�
                    oneword=LexicalScanner(Syn);
                }
            }
            else {} //��ѭ���У���whileͳһ����
        }

        if(Syn==41)//; ����һ�ζ�ȡ�����������õ�;,�������ȡһ���ַ�
            oneword=LexicalScanner(Syn);
    }

    //����������������ߴ�SpecialStatement()\return �����ɨ���β�ȴ��õ����� }���򵽴�
    //������ break��continue ���;,���������}
    return ;
}

//<��ͨ���>
void NormalStatement()
{
    //if((Syn==10||Syn==11||Syn==12) || Syn==26)
    if(Syn==10||Syn==11||Syn==12) //<��������>
    {
        VariableDeclaration();
        if(Syn!=41){
            if(Syn==42)//=
                printf("��ʾ:������䲻֧�ָ�ֵ�������ɴ˲������´���",
                    oneword->word);
        }

    }
    else //Syn==26   <ID>
    {
        ValueAssignment();
    }

}

//<�ض��ṹ���> (ֻ��StatementString()����)
//ע�⣺����SpecialStatement() ��ȡ���ṹ��������}��ɣ���������if()�����Ҫִ��
//      ���ȡһ���ʣ��Ա�����ж� Else ����Ƿ���ڣ��ʣ�SpecialStatementͳһ���һ����
void SpecialStatement()
{
    //if(Syn==14 || Syn==20 || Syn==22) //for �� while �� if
    if(Syn==14)//for
    {
        LoopFlag+=1;
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for ������Ӧ�ý�\"(\"������ [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26)//<ID>
            ValueAssignment();

        if(Syn!=41)//;
        {
            if(Syn==10 || Syn==11 || Syn==12)
                printf("\nLine [%3d] error:for����䲻֧����for������[%s] ",
                    currentLine,oneword->word);
            else
                printf("\nLine [%3d] error:for(s1;s2;s3)�ṹ��s1��ӦΪ\";\"���� [%s] \n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> �� <NUM> �� (
            Judgement();

        if(Syn!=41)//;
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)�ṹ��s2��ӦΪ\";\"���� [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26)//<ID>
            ValueAssignment();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)�ṹ��s3 [%s]��ʹ�ô���! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            LoopStatement();

            //LoopStatement() ����ɨ��������for�ṹ��䣨����}��
            //���û��}��StatementString() ���𱨴�
        }
        else
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)�ṹ��)��ӦΪ\"{\"���� [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        LoopFlag-=1;
        oneword=LexicalScanner(Syn);//���һ����
    }
    else if(Syn==20)//while
    {
        LoopFlag+=1;
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for ������Ӧ�ý�\"(\"������ [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> �� <NUM> �� (
            Judgement();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:while(s1)�ṹ��s1 [%s]��ʹ�ô���! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            LoopStatement();

            //LoopStatement() ����ɨ��������for�ṹ��䣨����}��
            //���û��}��StatementString() ���𱨴�
        }
        else
        {
            printf("\nLine [%3d] error:for(s1;s2;s3)�ṹ��)��ӦΪ\"{\"���� [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            LoopFlag-=1;
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        LoopFlag-=1;
        oneword=LexicalScanner(Syn);//���һ����
    }
    else //if
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=34)//(
        {
            printf("\nLine [%3d] error:for ������Ӧ�ý�\"(\"������ [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==26||Syn==27||Syn==32)//<ID> �� <NUM> �� (
            Judgement();

        if(Syn!=35)//)
        {
            printf("\nLine [%3d] error:if(s1)�ṹ��s1 [%s]��ʹ�ô���! \n",
                currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            if(LoopFlag==0)//if����ѭ����
                StatementString();
            else //if��ѭ����
                LoopStatement();
            //StatementString() ����ɨ��������if�ṹ��䣨����}��
            //���û��}�����غ�� StatementString() ���𱨴�
        }
        else
        {
            printf("\nLine [%3d] error:�涨if(s1){}�ṹ��)��ӦΪ\"{\"���� [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }

        //ִ��Else���ܴ��ڵ����
        ElseStatement();

        //����Ϊ��ȡ����if(){}�ṹ��}����һ���ַ�����else{} �ṹ��}����һ���ַ�
    }

}

//<�ж����>
void Judgement()
{
    char * tempWord;
    Expression();

    if(!(Syn>=43 && Syn<=48)) //��Ϊ�жϷ���
        if(Syn!=41 && Syn!=35) //Ҳ��Ϊ; Ҳ��Ϊ),�򷵻أ��ɵ�����������ɨ�赽}����
            return ;
        else //Ϊ;, ���غ�����ִ�У�������ʾ��������ô��
        {
            printf("��ʾ��Line [%3d] ���������ж������ʹ�÷��ж����!\n",currentLine);
            return ;
        }
    tempWord = oneword->word;

    oneword=LexicalScanner(Syn);
    if(!(Syn==26||Syn==27||Syn==32))//<ID> �� <NUM> �� (
        if(Syn!=41 && Syn!=35) //Ҳ��Ϊ; Ҳ��Ϊ),�򷵻أ��ɵ�����������ɨ�赽}����
            return ;
        else
        {
            printf("\nLine [%3d] error:�ж����ʹ�ó���, [%s] ��Ӧ�ý� [%s] \n",
               currentLine,tempWord,oneword->word);
            cError++;
            return;//������ϣ��ڵ�ǰΪ;�����ֱ�ӷ��أ���for������˼�������
                    //�� if �������Ҫ�����ж� �Ƿ�Ϊ ; ,���򱨴�
        }

    Expression();

}

//<loop��䴮>
void LoopStatement()
{
    if(Syn==39 || Syn==1000)//����� } �� ���������ʶ
        return ;

    char* tempWord=oneword->word;
    if(Syn==21 || Syn==15)//break;continue;
    {
        oneword=LexicalScanner(Syn);
        if(Syn!=41)//;
        {
            printf("\nLine [%3d] error:��[%s] ��Ӧ����\";\",����Ϊ [%s] \n",
               currentLine,tempWord,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            return;
        }

        //������ȡ
        oneword=LexicalScanner(Syn);
    }
    else
        StatementString();

    while(Syn!=39 && Syn!=1000 )//��Ϊ}����Ϊ�������
    {
        if(Syn==21 || Syn==15)//break;continue;
        {
            oneword=LexicalScanner(Syn);
            if(Syn!=41)//;
            {
                printf("\nLine [%3d] error:��[%s] ��Ӧ����\";\",����Ϊ [%s] \n",
                currentLine,tempWord,oneword->word);
                cError++;
                oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
                return;
            }

            //������ȡ
            oneword=LexicalScanner(Syn);
        }
        else
            StatementString();
    }
}

//<else ���>
void ElseStatement()
{
    //�ܵ�����ǰ����Ϊ }��������ȡ��һ��
    oneword=LexicalScanner(Syn);

    if(Syn==23)//else
    {
        oneword=LexicalScanner(Syn);
        if(Syn==38)//{
        {
            oneword=LexicalScanner(Syn);
            StatementString();

            //StatementString() ����ɨ��������else�ṹ��䣨����}��
            //���û��}�����ص�SpecialStatement()�����ٵ�StatementString() ���𱨴�

            oneword=LexicalScanner(Syn);//ɨ���һ��
        }
        else
        {
            printf("\nLine [%3d] error:�涨if(s1){}else{}�ṹ��else��ӦΪ\"{\"���� [%s] \n",
               currentLine,oneword->word);
            cError++;
            oneword=ScanWithErrorToBlockEnd(Syn);//ɨ��������һ��  } �ĵط�
            oneword=LexicalScanner(Syn);//ɨ���һ��
            return;
        }
    }
    else
    {
        //��������ֱ�ӷ���
    }
}

//<��������>
void VariableDeclaration()
{
    oneword=LexicalScanner(Syn);

    if(Syn!=26)//<ID>
    {
        printf("\nLine [%3d] error:�������Ļ������ͺ���Ӧ���кϷ���ʶ��������ӦΪ [%s]\n",
                    currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
        return ;
    }

    oneword=LexicalScanner(Syn);//�����������������;
}

//<��ֵ���>
void ValueAssignment()
{
    oneword=LexicalScanner(Syn);
    if(Syn!=42) // =
    {
        if(Syn>=28 && Syn<=31)//+ - * /
            return ; //��������a++ a--  a+=1; a*=2; ʱ����ʾ��ֱ�ӵȴ����غ�ı���
        else
            printf("��ʾ:Line[%3d] �Ա�ʶ����ͷ�����ӦΪ��ֵ���,�÷����Ƽ���\n",currentLine);
        return;
    }

    oneword=LexicalScanner(Syn);
    Expression();
}

//<���ʽ>
void Expression()
{
    Iterm();
    while(Syn==28 || Syn==29) // + -
    {
        oneword=LexicalScanner(Syn);
        Iterm();
    }
}

//<��>
void Iterm()
{
    Factor();
    while(Syn==30 || Syn==31)// * /
    {
        oneword=LexicalScanner(Syn);
        Factor();
    }
}

//<����>
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
            printf("\nLine [%3d] error:�� [%s] ��Ӧ�з���\")\"\n",
                    currentLine,oneword->word);
            cError++;
            oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
            return ;
        }

        //��һ��Ϊ�����ٶ�ȡһ��
        oneword=LexicalScanner(Syn);
    }
    else  //��Ϊ�����������
    {
        printf("\nLine [%3d] error:���ʽ��ʽ������ [%s] ��Ӧ�кϷ����ʽ��\n",
                    currentLine,oneword->word);
        cError++;
        oneword=ScanWithError(Syn);//ɨ��������һ�� ; ���� } �ĵط� ������������Ϳ�ʼ��
        return ;
    }
}


