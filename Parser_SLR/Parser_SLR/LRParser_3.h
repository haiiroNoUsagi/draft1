#pragma once
/*SLR������3
	��֮ǰ�ɹ��Ļ�����,�޸Ľṹʹ�ø�ǿ��Ĵʷ���������
	ע��,���п�Ҫ�趨Ϊ/MD
	*/
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

// ���ϴʷ�����,����������dll
#pragma comment(lib,"Scanner.lib")
extern "C" __declspec(dllimport)int _lex_enable(FILE* input, FILE* output, void(*callback_func)(const char*, int));
extern"C" __declspec(dllimport)FILE* _lex_getInput();

//#pragma comment(lib,"Grammar.lib")
//extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
//extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
//extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
//extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance, string expr);

// �ʷ�������������ṹ���ȫ�ֱ���,����ص�����
struct Token
{
	string token;			// ԭʽ
	string arg;				// �Զ����һ���ַ���
};

static vector<Token*>* tokens;

static void callback(const char* str, int type)	// �ص�����
{
	Token* new_token = new Token;
	new_token->token = type == 0 ? "id" : str;		//�ڷ���ʱ,��ʱ����ʶ��Ϊid
	new_token->arg = str;							//��ʵ��ֵ��arg�д洢

	tokens->push_back(new_token);
}

// ��̬����,���ַ���תΪ����
template<typename T>
static T string2V(string input)
{
	double res = 0;
	int len = input.length();
	if (len == 0)
		throw exception("����������ʲô!");
	bool isFushu = input[0] == '-';			// ����Ǹ���
	int point = input.find('.');
	if (point < 0)
	{
		return (T)(atoi(input.c_str())* (isFushu?-1:1));
	}
	int zhengshu = atoi(input.substr((isFushu ? 1 : 0), point).c_str());
	res += (double) zhengshu;
	string small = input.substr(point + 1);
	double xiaoshu = (double)(atoi(small.c_str()));
	double beilv = pow(0.1, (double)(small.length()));		// ע��˫���Ȼ�����ͷ
	res += (double)(xiaoshu * beilv);
	return (T)(res* (isFushu ? -1 : 1));
}

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

// �����������
template<typename T>
struct Semantic
{
	int pos1;					// Ҫ��������λ��,�Լ���������
	int pos2;
	T(*opration)(T, T);
};

template<typename T>
class LRParser_3
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

	unordered_set<string>* init_values;						//�ڷ�����,��Ҫ��ȡ��ֵ�ķ��ս���б�

	unordered_map<int, Semantic<T>*>* semanticRules;		//�ڹ�Լʱ,�������õĲ���ʽ,ִ�д˱��еĶ���


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

	string getOneItem(string raw, vector<string>& rawProd, int point);

	/*�������ɱ�׼��Ŀ��ĵݹ麯��*/
	void LR0_build(unordered_set<string>&TokenList, int In);

	void parser_build();

public:
	LRParser_3();
	~LRParser_3();

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
	void add_ignorableErr(string symbol, string comment);

	// ���Ҫʶ����ս��
	void add_initVals(string symbol);

	// ��Ӳ���ʽ��Ӧ���﷨�Ƶ�����
	// ����: ����ʽ������, Ҫ����Ĳ���ʽ��Ŀ, ��Ŀ���㺯��ָ��

	void add_semanticRules(int prod_index, vector<int>& prodItem_index,T(*opration)(T, T));

	// parse����:��ʼ����
	// ����:�����,����Ǻ���������(vector<string>)
	// ����:�Ǻ���
	// ���:�����������﷨�������õĲ���ʽ,�Լ����ķ�������������Ϣ
	void parse(const char * fileName, bool analyze, ostream& output);
};

