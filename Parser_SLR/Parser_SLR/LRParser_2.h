#pragma once

#include<stack>
#include<vector>
#include<string>
#include<stdarg.h>
#include<iostream>
#include<unordered_set>
#include<unordered_map>

using namespace std;

typedef vector<string>*(*pf_pvectorSTR__pvoid)(void*);
typedef vector<string>*(*pf_pvectorSTR__pvoid_int)(void*, int);
typedef vector<string>*(*pf_pvectorSTR__pvoid_string)(void*, string);
typedef unordered_set<string>*(*pf_pusetSTR__pvoid_string)(void*, string);


struct Item
{
	string raw;								//ԭ����ʽ,�������
	vector<string>* prod_NOT_DELETE;		//���ķ��л�ȡ����ʽ�Ҳ�������,����ɾ��!
	int point;								//���λ��,����prod->at(point)�����
};


//  �����������
struct Operation
{
	int type;					//��������
								/*
								0 acc,��ʾ����
								1 �ƽ�
								2 ��Լ
								3 ���� - ��������
								999 goto (������ʵ�,��Ϊ��ֻ������ڹ�Լ֮��)
								*/

	int operate;
};

class LRParser_2
{
private:
	void* grammar;
	vector<string>* _grammar_prodList;					//����ʽ�б�
	pf_pvectorSTR__pvoid_int _grammar_getProd;			//����������ȡ����ʽ�Ҳ�
	pf_pusetSTR__pvoid_string _grammar_getFirst;		//First����
	pf_pusetSTR__pvoid_string _grammar_getFollow;		//Follow����

	bool enabled;
	string extendProd;									//��չ�ķ�����ʼ��
	vector<string>* extendProd_Vector;					//��չ�ķ�������
	
	vector<string>* Errmsg;								//������Ϣ
	/*�����Ƿ������action��goto*/
	unordered_map<int, unordered_map<string, Operation*>*>* action_stat_input__act;

	/*��һ����������,���ܴ��ںܶ���Ŀ��.
	ItemSets�洢����������е�������Ŀ��In
	���г�פ����Ŀ����������,�����������
	���Ƿǳ�פ����Ŀ����Ҫ���д���(����closure�Ĳ���)
	���ǻ���Ҫ����һЩӳ��,ʹ��Щ��Ŀ������������������

	*/
	vector<unordered_set<string>*>* ItemSets;


	unordered_map<string, Item*>* map_item2Item;			//ĳ��Ŀ�ַ�������Ŀ���ӳ��

	unordered_map<string, int>* map_gotoFunc_In;			//goto�����Ľ����ǰ������Ŀ��

	//closure����
	/*
	ÿ����һ��closure����,�ͻ�����һ���µ���Ŀ��,���I0֮�����Ŀ�����������closure��˳���й�
	*/
	unordered_set<string>* closure(unordered_set<string>& Items);

	//goto����
	/*goto������Ҫ����closure,�����Ӧ�÷���һ��closure������ʵ��
	ͬʱ,goto���ܻᵽ���Ѵ��ڵ���Ŀ��,�����������Ĵ���Ӧ�ý���closure����
	�����ܻ᷵��һ���ɵ�ָ��.
	*/
	unordered_set<string>* _goto(int In, string input);

	/*checkExist_In_SISF
	������ȷ�ϱ�׼��Ŀ�������Ƿ������������ͬ����Ŀ��
	�������,�᷵��!!!In������!!!
	���������,�᷵��-1;
	*/
	int checkExist_In_SISF(unordered_set<string>& NewItemSet);

	string getOneItem(string raw,vector<string>& rawProd, int point);

	/*�������ɱ�׼��Ŀ��ĵݹ麯��*/
	void LR0_build(unordered_set<string>&TokenList, int In);

	void parser_build();

public:
	LRParser_2();
	~LRParser_2();

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


	/*һЩ�����Ĵ�������:
		����ƥ��
			���ܵĴ�������:
				�ڲ����������ŵ�״̬�½�����������
					ԭ��:���Ų�ƥ��,��������һ������
					����:�������������,����������

				�ڲ����������ŵ�״̬�½�����������
					ԭ��: Symbol_next�в�����������:
						��һ����Ǻ����������.-----��˲��ý��������ŵ����Ӧ����Symbol_next����

				��Ҫ����������,�����ѵ�����ʽ���


		����ı��
			���㷽��:����Ϊ�������ı���б�
				���� id�����id��(�����Ը���
					��ô���������id Ҫôȱ�������,Ҫô��������һ��id
					���������������,Ҫô��ȱ�������,Ҫô����������������
		
					����һ���޷�ȷ��ȱ�ٵ���ʲô�����,��˲�ȡ�Ĳ����Ǻ���



	*/
	
	// ��ӿɺ��ӵĴ������->δƥ���������,��Ӻŵ������
	void add_ignorableErr(string symbol,string comment);

	// parse����:��ʼ����
	// ����:�����,����Ǻ���������(vector<string>)
	// ����:�Ǻ���
	// ���:�����������﷨�������õĲ���ʽ,�Լ����ķ�������������Ϣ
	void parse(vector<pair<string,string>>& tokens,ostream& output);
};

