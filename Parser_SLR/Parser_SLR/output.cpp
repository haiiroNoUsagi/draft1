///*
//Parser_SLR�����ļ�
//	����SLR������,�����պ󵼳�
//*/
//
//
//#include"LRParser.h"
////#include"LRParser_2.h"
//#include"LRParser_3.h"
//#pragma comment(lib,"Grammar.lib")
//#pragma comment(lib,"Scanner.lib")
//////----------------------------------------------------------------------
//////�ʷ�������
////extern "C" _declspec(dllimport) vector<pair<string,string>>* _Scanner_GetTokens(string _Lex_Path__absolute, string _Target_File__absolute);
//
////----------------------------------------------------------------------
////ʹ�����ɵ��ķ���
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
//	//vector<pair<string, string>>* tokens = _Scanner_GetTokens("G:\\���Ĳݸ�2\\����ԭ���һ�����\\flex\\Lex_ver2.0\\Debug","G:\\���Ĳݸ�2\\����ԭ���һ�����\\testFile1.txt");
//
//	//// ��ӿɺ��Դ�����Ϣ
//	////test.add_ignorableErr(")", "���ܶ����������:). ���Ժ��Բ�����");
//	////test.add_ignorableErr("*", "���ܶ���ķ���\"*\". ���Ժ��Բ�����");
//	////test.add_ignorableErr("+", "���ܶ���ķ���\"+\". ���Ժ��Բ�����");
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