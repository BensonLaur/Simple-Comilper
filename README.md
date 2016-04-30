#Simple Compiler
###first commit:finish the lexcical Analysis design
by 2016/4/30 12:59

### 3rd、4th commit:finish the Syntax Analysis design
by 2016/4/30 13:12

/*********************************************************
*
*   file: README
*   description: describes the design of compiler,including
*                Lexical,Syntax,Semantic anlysis design.
*
*   note:the design is not completed yet!
*        progress:
*           Lexical analysis   [check]
*           Syntax  analysis   [check]
*           semantic analysis  [uncheck]
*

**********************************************************/

/****** 词法分析阶段 *******************/

各种单词符号对应的种别码
TOKEN           VALUE

'short'           10

'int'             11

'long'            12

'for'             14

'continue'        15

'while'           20

'break'           21

'if'              22

'else'            23

'return'          24

letter(letter|digit)*   26

digit digit*            27

'+'                       28
                       
'-'                       29
                       
'*'                       30
                       
'/'                       31

'('                       34

')'                       35

'{'                       38

'}'                       39

';'                       41

'='                       42

'=='                      43

'>='                      44

'<='                      45

'>'                       46

'<'                       47

'!='                      48

'#'                       0


/****** 语法分析阶段 *****************/

待分析的简单语言的语法 (recursive descent parser 递归下降子程序语法分析;文法要求：LL(1)文法)

示意如下：

（1）<程序> ::= int main '{' <语句串> '}'

（2）<语句串> ::= ( <普通语句> ';' ) | (<特定结构语句>)|';' | return <表达式> ';'

                { ( <普通语句> ';' ) | (<特定结构语句>)|';' | return <表达式> ';'}
                 
（3）<普通语句> ::= <变量声明> | <赋值语句>

（4）<特定结构语句> ::= for '(' <赋值语句> ';' <判断语句> ';' <赋值语句> ')' '{' <loop语句串> '}'

                       |while '(' <判断语句> ')' '{' <loop语句串> '}     '
                       
                       |if '(' <判断语句> ')' '{'<语句串>'}' <else 语句>
                       
（5）<else 语句> ::= ( else '{' <语句串> '}' ) | <空>

（6）<loop语句串> ::= <语句串> | ( break ';') | (continue ';') {<语句串> | ( break ';') | (continue ';')}

（7）<判断语句> ::= <表达式> <判断符号> <表达式>

（8）<判断符号> ::= "==" | ">=" | "<=" |">" |"<" |"!="

（9）<变量声明> ::= <基本类型> <ID>

（10）<赋值语句> ::= <ID> = <表达式>

（11）<表达式> ::= <项> { '+' <项> | '-' <项> }

（12）<项> ::= <因子> { '*' <因子> | '/' <因子> }

（13）<因子> ::= <ID> | <NUM> | '(' <表达式> ')'

（14）<基本类型> ::= int | long | short


