/*
�ķ���:����һ���ķ�.�������ķ������ļ�,�������ڴ�����ķ�����
	ע���ķ��еĿմ���~����
	�ķ�֧��'��
*/

#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<fstream>

#include"Graph.h"

#ifdef GRAMMAR_DEV
#define DLL_TYPE dllexport
#else
#define DLL_TYPE dllimport
#endif

using namespace std;
class  Grammar
{
private:
	//������
	unordered_set<string>* x;

	bool enabled;
	
	// ��Ӧ��ϵ:1����ʽ��->1����ʽ�б�->n����ʽ�Ҳ�
	unordered_map<
		string,		//����ʽ��
		vector<					//����ʽ�Ҳ��б�
			vector<string>*		//����ʽ�Ҳ�
								//ע��,�Ҳ����һλ��raw
		>*
	>* Product_List;


	// ����FIRST\FOLLOW����
	unordered_map<string, unordered_set<string>*>* FIRSTS;
	unordered_map<string, unordered_set<string>*>* FOLLOWS;

	void initFirstFollow();
	void initFirst();
	void initFollow();

	string startID;
public:
	Grammar();
	~Grammar();

	// ���ô˷���,���ɸ��ķ�
	bool analyzeGrammarFile(string filePath);
	
	// �����󲿶�Ӧ�Ĳ���ʽ����
	int getListCount(string left);

	// �����󲿶�Ӧ����ʽ��Ԫ�ظ���(-1)
	int getItemsCount(string left, int index);

	// ����ĳһ��Ԫ��
	string getProductItem(string left, int index1, int index2);

	// ���ع����Ӧ��ԭʽ
	string getRawProduct(string left,int index);

	// ����FIRST����FOLLOW��
	const unordered_set<string>& FIRST(string left);
	const unordered_set<string>& FOLLOW(string left);
};

