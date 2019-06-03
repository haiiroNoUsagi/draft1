
number [0-9]
value {number}+(\.{number}+)?

operation0 [\+\-]
operation1 [\*/]
operation2[\(\)]
%%
" " ;
{value} ECHO;printf("    value\n");
{operation0} ECHO;printf("    operation0\n");
{operation1} ECHO;printf("    operation1\n");
{operation2} ECHO;printf("    operation2\n");
%%
main()
{
	yylex();
	printf("$    end");
}