/*�ڶ��浼���ļ�*/

#include"LRParser_3.h"
#include"LRParser_3.cpp"
#pragma comment(lib,"Grammar.lib")
#pragma comment(lib,"Scanner.lib")
////----------------------------------------------------------------------
////�ʷ�������
//extern "C" _declspec(dllimport) vector<pair<string,string>>* _Scanner_GetTokens(string _Lex_Path__absolute, string _Target_File__absolute);

//----------------------------------------------------------------------
//ʹ�����ɵ��ķ���
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance, string expr);

// ����ִ�еĲ���
template<typename T>
T add(T a, T b)
{
	return a + b;
}

template<typename T>
T mul(T a, T b)
{
	return a * b;
}

template<typename T>
T jian(T a, T b)
{
	return a - b;
}

template<typename T>
T chu(T a, T b)
{
	return a / b;
}

template<typename T>
T print(T a, T b)
{
	cout << "�õ��˽�� : " << a << endl;
	return 0;
}

template<typename T>
T nothing(T a, T b)
{
	return a;
}


int main()
{
	//void* grammar = _Grammar_getInstance("grammar.txt");
	void* grammar = _Grammar_getInstance("grammar2.txt");
	LRParser_3<float>* test = new LRParser_3<float>();
	test->init(
		grammar,
		_Grammar_getProductions,
		_Grammar_getProdItems,
		_Grammar_getFirst,
		_Grammar_getFollow
	);
	
	test->add_initVals("id");



	// �����������������--grammar
	//vector<int> p1({ 0,0 });
	//vector<int> p2({ 0,2 });
	//vector<int> p3({ 1,1 });
	//test.add_semanticRules(0, p1, print);
	//test.add_semanticRules(1, p2, add);
	//test.add_semanticRules(3, p2, mul);
	//test.add_semanticRules(5, p3, nothing);

	// �����������--grammar2(��������)
	vector<int> p1({ 0,0 });
	vector<int> p2({ 0,2 });
	vector<int> p3({ 1,1 });
	test->add_semanticRules(0, p1, print);
	test->add_semanticRules(1, p2, add);
	test->add_semanticRules(2, p2, jian);
	test->add_semanticRules(4, p2, mul);
	test->add_semanticRules(5, p2, chu);
	test->add_semanticRules(7, p3, nothing);


	test->parse(NULL, true, cout);
	delete grammar;
	delete test;
	system("pause");
	return 0;
}