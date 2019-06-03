letters [a-zA-Z]
words {letters}+

%{
int num_chars = 0, num_letters = 0, num_words = 0;
%}

%%
201693114 num_chars+= 9;printf("ÁºÁ¦º­");
{words} num_words+= 1;
{letters} num_letters++;num_chars++;ECHO;
\n num_chars++;ECHO;
. num_chars++;ECHO;

%%
main()
{
	yylex();

	printf("# of chars = %d, # of letters = %d, # of words = %d\n ", num_chars, num_letters,num_words);
}