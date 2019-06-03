/*
LRParser��: LR��������
ͨ������๹��һ��SLR������
*/
#pragma once

#include<tuple>
#include<vector>
#include<string>
#include<iostream>
#include<unordered_set>
#include<unordered_map>
using namespace std;

//typedef string(*pf_string__pvoid)(void*);
typedef vector<string>*(*pf_pvectorSTR__pvoid)(void*);
typedef vector<string>*(*pf_pvectorSTR__pvoid_int)(void*,int);
typedef vector<string>*(*pf_pvectorSTR__pvoid_string)(void*,string);
typedef unordered_set<string>*(*pf_pusetSTR__pvoid_string)(void*, string);

// ����һ���ṹ��.
// ����:��Ŀ
struct Item_1
{
	string raw;								//ԭ����ʽ,�������
	vector<string>* prod_NOT_DELETE;		//���ķ��л�ȡ����ʽ�Ҳ�������,����ɾ��!
	int point;								//���λ��,����prod->at(point)�����
};

// ����һ���ṹ��,��¼ÿ�����ŵ���ϸ��Ϣ
struct Token_Property
{
	string raw_token;			// �÷��ŵ�ԭʽ
	string comment;				// �������ŵ����
};

class LRParser
{
private:
	/*--״̬��־--*/
	bool enabled;

	/*--�ⲿ���ñ���--*/
	void* grammar;
	vector<string>* _grammar_prodList;					//����ʽ�б�
	pf_pvectorSTR__pvoid_int _grammar_getProd;			//����������ȡ����ʽ�Ҳ�
	pf_pusetSTR__pvoid_string _grammar_getFirst;		//First����
	pf_pusetSTR__pvoid_string _grammar_getFollow;		//Follow����

	/*--˽�б���--*/
	string extendProd;									//��չ�ķ�����ʼ��
	vector<string>* extendProd_Vector;					//��չ�ķ�������

	unordered_map<string, Item_1*>* I_map;				//ԭ��Ŀ����Ŀ�ṹ��֮���ӳ��

	unordered_map<int,unordered_set<string>*>* closures;	//��Ŀ��In��Ӧ�ıհ�
	unordered_map<string, unordered_set<string>*>* gotos;	//goto�������й����б�,ע��,���ǽ�����ƴ���˴�֮ǰ.

	vector<unordered_set<string>*>* itemSet_Family;			// ���ķ���LR(0)��Ŀ���淶��,vector��Ԫ��,��ΪIn
	//unordered_map<string, I_expr*>* LRI_map;			// ����������ԭʽ,�˴���ӳ��
	//unordered_map<string, int>* LRI_set_Index;			// Ϊ�淶���������

	

	/*--˽�з���--*/
		
	/*���ô���*/
	void new_I(string prod, vector<string>& right, unordered_set<string>& target_set);			//�µ���Ŀ
	void next_I(string lastProd, Item_1& lastI, unordered_set<string>& target_set);				//������Ŀ,��������
	// ����:һ����Ŀ���ָ��
	// ���:����������ͬ��Ŀ��,����NULL,���򷵻ظ���Ŀ���ָ��
	unordered_set<string>* checkExist(unordered_set<string>* NewItem);											//������ɵ���Ŀ���Ƿ��Ѵ���


	//�հ�����,��ĳһ��Ŀ���ıհ�
	//����:I(n)? ����n.
	//����ֵ:��Ŀ�հ��ļ���(ԭʽ)
	//��˽�б���������ԭʽ����Ŀ�ṹ���ӳ��
	//�ر�ע��,����ת����ϵ��ʱ��,����Ҫ��֤In����
	//����,Ҫ�ȴ���In,�ٽ�����1����Ĳ���ʽ����In,�������closure
	unordered_set<string>* closure(int In);							//����:������Ŀ��In�ıհ�
	//unordered_set<string>* closure(int unordered_set<string>& I);	//����:������Ŀ���ıհ�

	// ת�ƺ���,ԭ��ͬ����
	// ����:I(n)?����n. ,��һ������
	unordered_set<string>* _goto(int In, string input);

	// LR0_build����(�ݹ�)
	// �޷���ֵ�Ͳ���
	// ʹ����д�õķ���,������Ŀ��.ע��,�״ε���Inʱ,�����µ�I
	// (���ܻ���ֻ�?)

	void LR0_build(unordered_set<string>&LeftList, int In);

	// �����Ǻ���,��������Ӧ��Token����
	void GetTokens(vector<pair<string,string>>& input);

public:
	LRParser();
	~LRParser();

	bool isEnabled();
	
	// init����:��ʼ��LR������
	// ����:�ķ���ʵ��, �Լ����ʵ�������º���ָ��:
	//		��ȡ����ʽ�б�
	//		��ȡָ������λ�õĲ���ʽ
	//		���ķ���First����
	//		���ķ���Follow����
	bool init(void* grammar,
		pf_pvectorSTR__pvoid function_getAllProd,
		pf_pvectorSTR__pvoid_int _grammar_getProd,
		pf_pusetSTR__pvoid_string _grammar_getFirst,
		pf_pusetSTR__pvoid_string _grammar_getFollow);

	// parse����:��ʼ����
	// ����:�����,����Ǻ���������(vector<string>)
	// ����:�Ǻ���
	// ���:�����������﷨�������õĲ���ʽ,�Լ����ķ�������������Ϣ
	void parse(vector<string>& tokens,ostream& output);
};

