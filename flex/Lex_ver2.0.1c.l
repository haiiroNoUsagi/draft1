	
number [0-9]
value {number}+(\.{number}+)?

operation0 [\+\-]
operation1 [\*/]
operation2[\(\)]

%{

%}

%%
" " ;
{value} ECHO;printf("    value\n");res->token_type = 0;res->token=yytext;res->next = (token_node*)malloc(sizeof(token_node)); res = res->next;
{operation0} ECHO;printf("    operation0\n");res->token_type = 1;res->token=yytext;res->next = (token_node*)malloc(sizeof(token_node)); res = res->next;
{operation1} ECHO;printf("    operation1\n");res->token_type = 2;res->token=yytext;res->next = (token_node*)malloc(sizeof(token_node)); res = res->next;
{operation2} ECHO;printf("    operation2\n");res->token_type = 3;res->token=yytext;res->next = (token_node*)malloc(sizeof(token_node)); res = res->next;
%%
main()
{
	struct token_node{
		int token_type;
		char* token;
		token_node* next;
	};
	token_node* res = (token_node*)malloc(sizeof(token_node));
	yylex();
	while(res)
	{
		token_node* p =res->next;
		free(res);
		res = p;
	}
}