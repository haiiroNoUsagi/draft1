letters [a-zA-Z]
numbers [0-9]
symbol [^" "a-zA-Z0-9]
logic [!=\<\>]?=

var ({letters}|_)({letters}|{numbers}|_)*
number {numbers}+
operation ({symbol}|{logic})

id ({var}|{operation}|{number}|{operation})


%{
//int num_chars = 0, num_letters = 0, num_words = 0;
%}

%%
" " ;
{id} ECHO;printf("\n");
%%
main()
{
	yylex();
	printf("$");
}