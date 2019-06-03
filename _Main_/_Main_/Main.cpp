/*
工程:Main,是测试用工程,所有的测试都会在这里进行
	在Debug文件夹放置对应的dll文件
*/

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unordered_set>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
//以下是模版,将需要测试的dll依次添加,并添加对应的头文件和dll 
//#pragma comment(lib,"")
//------------------------------------------------------------------------------------------
#pragma comment(lib,"Functions.lib")
#pragma comment(lib,"Grammar.lib")

//------------------------------------------------------------------------------------------

/*-----词法分析-----*/
//调用词法分析器返回记号流
extern "C" _declspec(dllimport) vector<string>* _Scanner_GetTokens(string _Lex_Path__absolut, string _Target_File__absolut);

/*-------文法-------*/
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport)unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
extern "C" _declspec(dllimport)unordered_set<string>* _Grammar_getFollow(void* instance, string expr);


//------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector<string>* tokens = _Scanner_GetTokens(
		"G:\\糟糕的草稿2\\编译原理的一组程序\\flex\\Lex_ver1.1\\Debug",
		"G:\\糟糕的草稿2\\编译原理的一组程序\\flex\\Lex_ver1.0\\Debug\\test.txt"
	);

	for (vector<string>::iterator it = tokens->begin(); it != tokens->end(); ++it)
	{
		cout << *it << endl;
	}

	//Grammar* g = new Grammar();

	void* g = _Grammar_getInstance("Grammar.txt");

	vector<string>* list = _Grammar_getProductions(g);

	delete tokens;
	delete g;
	system("pause");
	return 0;
}
