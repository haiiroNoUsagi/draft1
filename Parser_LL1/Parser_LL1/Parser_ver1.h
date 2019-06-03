#pragma once
/*
���϶��µ�Ԥ�����
(ע���޸���Ŀ���п�Ϊ/MD)
(�ķ�Ҫ������ݹ�)
	����ԭ��:
		1. ��ʼ��
			��ʼ���׶�,��ȡ�ķ��ļ�(ע��,�մ�ʹ��~����)
			����Ԥ�������.
		2. �������봮
			������ôʷ�������,��˳�ʼ���Ĳ�����Ҫ��ôʷ������������
			����������ʷ���������,���������,ֱ���������,���ؼǺ���;
			��ʱ,�Ǻ�������ͨ���ص������ص���������
		3. ����Ǻ���
			����Ҫ��,�趨����Ǻż�������.
		4. �߷����߼���
*/

// �������ս������ս���Ķ���
// Ϊ�˱��ڼ���,Ŀǰ������򵥵��жϷ���

#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include<unordered_set>
#include<unordered_map>
using namespace std;
// �������﷨��������ʷ�������
/*�ⲿ������*/
#pragma comment(lib,"Scanner.lib")
extern "C" __declspec(dllimport)int _lex_enable(FILE* input, FILE* output, void(*callback_func)(const char*, int));
extern "C" __declspec(dllimport)FILE* _lex_getInput();
#pragma comment(lib,"Grammar.lib")
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance, string expr);

static vector<string>* tokens;

class Parser_ver1
{
private:
	vector<string>* products;										// ����ʽ
	vector<vector<string>*>* product_items;							// ����ʽ��Ӧ���Ҳ�
	unordered_map<string, unordered_map<string, int>*>* table;	// ������

	//unordered_set<string>* terminators;				// �ս���б�(�����ж�)
	//unordered_set<string>* nonterminals;			// ���ս���б�

public:
	/*��ʼ������*/
	Parser_ver1();
	Parser_ver1(const char* program_file);			// ʹ���ķ��ļ���ʼ��
	void initGrammar(const char* program_file);		// �ֶ���ʼ��
	~Parser_ver1();

	/*��ȡ״̬*/
	int getStat();

	/*�趨��������*/
	void addAnalyzeRule();

	/*��������*/
	void parse(const char* fileName, bool analyze, ostream& output);

};

