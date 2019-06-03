/*
�ķ���:һ���ķ���ʵ��
	
	��ʼ��:��Ҫһ���ķ��ļ���·��

	���ⲿ�ṩ�˷���:
		����				����

		isEnabled()			�ж��ķ��Ƿ�ɹ�����

		getProductList()	��ȡ���ķ����в���ʽ(���з�,��˳������.)

		getFirst(string)	����ĳ���Ż���ʽ��first����
		getFollow(string)	ͬ��,����follow����

	ʹ��ԭ��:�ķ����ṩ�������ϻ��б��ָ��,���ǲ�Ӧ�����ⲿ������Щָ��

*/
//#ifdef GRAMMAR_DEV
//#define DLL_TYPE dllexport
//#else
//#define DLL_TYPE dllimport
//#endif


#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<fstream>

#include"Graph.h"

using namespace std;

class Grammar
{
private:
	/*----- ˽�б��� -----*/
	bool enabled;
	//����ʽ����Ҫ�ṹ
	unordered_map<
		string,				//����ʽ��
		vector<				//����ʽ�Ҳ��б�
		vector<string>*		//����ʽ�Ҳ�
							//ע��,�Ҳ����һλ��raw
		>*
	>* Product_List;

	unordered_map<int,pair<string,int>>* index;			//�ⲿ����ͨ��Ψһ�����ֻ�ȡ�Ҳ�����ʽ,���������һһ��Ӧ��

	vector<string>* rawList;							//�洢��ֵĲ���ʽ�б�
	
	unordered_set<string>* empty_in_First;				//��¼��ЩFirst�����к��пմ�

	unordered_map<string, unordered_set<string>*>* temp_sets_first;	//��ѯ���ı��ʽ�ļ���,��ʱ�����ڴ�
	//���ս������������
	unordered_map<string, unordered_set<string>*>* FIRSTS;
	unordered_map<string, unordered_set<string>*>* FOLLOWS;


	/*----- ˽�з��� -----*/

	// ��������������init�����е���,���ڳ�ʼ�����ս������������
	void initFirst();
	void initFollow();

	// �����Ҫ���ұ��ʽ��First��Follow����,��Ҫͨ��˽�з������
	unordered_set<string>* getFirstOfExpr(string expr);
public:
	Grammar();
	~Grammar();

	string startID;					//�ķ�����ʼ��

	//��ʼ���ķ�
	bool init(string filePath);

	bool isEnabled();

	// ��ȡ���в���ʽ�б�(�з�)
	vector<string>* getProductions();

	// ��ȡ����ʽ��Ӧ����
	string getProdLeft(int index);
	// ��ȡ����ʽԪ��
	vector<string>* getProdItems(int index);

	// First Follow���� 
	// �������ķ�ʱ,Ĭ��Ϊÿһ�����ս�����������ǵ�������
	// ��Ҫ������ʽ�ļ�,����˽�з���
	unordered_set<string>* getFirst(string input);
	unordered_set<string>* getFollow(string input);
};

