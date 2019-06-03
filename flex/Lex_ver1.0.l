letters [a-zA-Z]
numbers [0-9]
notemp [^" "a-zA-Z0-9]

var ({letters}|_)({letters}|{numbers}|_)*
number {numbers}+
operation {notemp}+

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