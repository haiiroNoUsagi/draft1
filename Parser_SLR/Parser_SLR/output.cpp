///*
//Parser_SLR导出文件
//	测试SLR分析器,便于日后导出
//*/
//
//
//#include"LRParser.h"
////#include"LRParser_2.h"
//#include"LRParser_3.h"
//#pragma comment(lib,"Grammar.lib")
//#pragma comment(lib,"Scanner.lib")
//////----------------------------------------------------------------------
//////词法分析器
////extern "C" _declspec(dllimport) vector<pair<string,string>>* _Scanner_GetTokens(string _Lex_Path__absolute, string _Target_File__absolute);
//
////----------------------------------------------------------------------
////使用生成的文法类
//extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
//extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
//extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance, string expr);
//
//int main()
//{
//	void* grammar = _Grammar_getInstance("grammar.txt");
//
//	//LRParser_2 test;
//	//test.init(
//	//	grammar,
//	//	_Grammar_getProductions,
//	//	_Grammar_getProdItems,
//	//	_Grammar_getFirst,
//	//	_Grammar_getFollow
//	//);
//
//	//vector<pair<string, string>>* tokens = _Scanner_GetTokens("G:\\糟糕的草稿2\\编译原理的一组程序\\flex\\Lex_ver2.0\\Debug","G:\\糟糕的草稿2\\编译原理的一组程序\\testFile1.txt");
//
//	//// 添加可忽略错误信息
//	////test.add_ignorableErr(")", "可能多余的右括号:). 尝试忽略并继续");
//	////test.add_ignorableErr("*", "可能多余的符号\"*\". 尝试忽略并继续");
//	////test.add_ignorableErr("+", "可能多余的符号\"+\". 尝试忽略并继续");
//	//test.parse(*tokens, cout);
//	/*delete tokens;*/
//
//	LRParser_3 test;
//	test.init(
//		grammar,
//		_Grammar_getProductions,
//		_Grammar_getProdItems,
//		_Grammar_getFirst,
//		_Grammar_getFollow
//	);
//	test.parse(NULL, false, cout);
//	delete grammar;
//	system("pause");
//	return 0;
//}