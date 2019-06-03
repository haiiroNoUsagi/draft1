/*
����:Main,�ǲ����ù���,���еĲ��Զ������������
	��Debug�ļ��з��ö�Ӧ��dll�ļ�
*/

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unordered_set>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
//������ģ��,����Ҫ���Ե�dll�������,����Ӷ�Ӧ��ͷ�ļ���dll 
//#pragma comment(lib,"")
//------------------------------------------------------------------------------------------
#pragma comment(lib,"Functions.lib")
#pragma comment(lib,"Grammar.lib")

//------------------------------------------------------------------------------------------

/*-----�ʷ�����-----*/
//���ôʷ����������ؼǺ���
extern "C" _declspec(dllimport) vector<string>* _Scanner_GetTokens(string _Lex_Path__absolut, string _Target_File__absolut);

/*-------�ķ�-------*/
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport)unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
extern "C" _declspec(dllimport)unordered_set<string>* _Grammar_getFollow(void* instance, string expr);


//------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector<string>* tokens = _Scanner_GetTokens(
		"G:\\���Ĳݸ�2\\����ԭ���һ�����\\flex\\Lex_ver1.1\\Debug",
		"G:\\���Ĳݸ�2\\����ԭ���һ�����\\flex\\Lex_ver1.0\\Debug\\test.txt"
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
