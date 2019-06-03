	
number [0-9]
value {number}+(\.{number}+)?

operation0 [\+\-]
operation1 [\*/]
operation2[\(\)]

%{
#pragma comment(lib,"libfl.lib")
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#ifdef _DEBUG
#define DLL_MODE dllexport
#else
#define DLL_MDOE dllimport
#endif
typedef void (*__callback_func)(const char*, int);
__callback_func callbackFunc;
%}

%%
"$" callbackFunc("$", -1); yyterminate();
" " ;
{value} ECHO;printf("    value\n");callbackFunc(yytext, 0);
{operation0} ECHO;printf("    operation0\n");callbackFunc(yytext, 1);
{operation1} ECHO;printf("    operation1\n");callbackFunc(yytext, 2);
{operation2} ECHO;printf("    operation2\n");callbackFunc(yytext, 3);
%%
//
//void _print(char* c, int t)
//{
//	printf("print: %s    , type:%d", c, t);
//}
//
//main()
//{
//	callbackFunc = _print;
//	yylex();
//}

__declspec(DLL_MODE)int _lex_enable(FILE* input, FILE* output, __callback_func callback_func)
{
	if (input)
		yyin = input;
	else
		return -1;
	if (output)
		yyout = output;
	else
		yyout = stdout;

	callbackFunc = callback_func;
	yylex();
	return 0;
}