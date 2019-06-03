#include"Parser_ver1.h"

int main()
{
	Parser_ver1 pv1;
	pv1.initGrammar("grammar.txt");
	pv1.parse(NULL, false, cout);
	getchar();
	return 0;
}