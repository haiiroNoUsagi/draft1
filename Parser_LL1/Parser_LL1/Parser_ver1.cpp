#include "Parser_ver1.h"



Parser_ver1::Parser_ver1()
{
	table = new unordered_map<string, unordered_map<string, int>*>();
	product_items = new vector<vector<string>*>();
}

Parser_ver1::Parser_ver1(const char * grammar_file)
{
	Parser_ver1();
	initGrammar(grammar_file);
}

void Parser_ver1::initGrammar(const char * program_file)
{
	void* grammar_inst = _Grammar_getInstance(program_file);		// ��ȡ�ķ�ʵ��.ע��,������Ԥ�������֮��,�ķ��Ϳ���ɾ����.
																	// ���,����Ϊ��ʱ����
	products = new vector<string>(*_Grammar_getProductions(grammar_inst));			// �����ķ��еĲ���ʽ�б�

	// TODO:�˴���ӳ�ʼ�����ս�����ս���б�ķ���
	for (int it0 = 0; it0 < products->size(); ++it0)			// ���ѭ��:��ÿһ������ʽ A->��
	{
		// ˳���ʼ������ʽ�ļǺŴ�
		product_items->push_back(new vector<string>(*_Grammar_getProdItems(grammar_inst, it0)));
		// ������table��������ʽ�ı���
		string curr = products->at(it0);
		string left = curr.substr(0, curr.find("->"));
		string right = curr.substr(curr.find(">") + 1);
		if (table->count(left) == 0)
		{
			table->insert(pair<string, unordered_map<string,int>*>(left, new unordered_map<string, int>()));
		}
		unordered_set<string>& first_alpha = *_Grammar_getFirst(grammar_inst, right);
		bool hasEmpty = false;
		for (unordered_set<string>::iterator it1 = first_alpha.begin(); it1 != first_alpha.end(); ++it1)	// ��ÿһ���ս����a,������ʽ����[A, a]
		{
			if (*it1 != "~")						// ע��,����ķ�����LL1��,���Ḳ��,����������
			{
				table->at(left)->insert(pair<string,int>(*it1, it0));
			}
			else
			{
				hasEmpty = true;										// �пմ�,��ִ����һ��:
			}
		}

		if (hasEmpty)
		{
			unordered_set<string>& follow_A = *_Grammar_getFollow(grammar_inst, left);			// ע��,�ǰ�A->��ȫ������,������ָ��һ��
			for (unordered_set<string>::iterator it1 = follow_A.begin(); it1 != follow_A.end(); ++it1)
			{
				table->at(left)->insert(pair<string, int>(*it1, it0));
			}
		}
	}
	delete grammar_inst;
}


Parser_ver1::~Parser_ver1()
{
	if (products)
		delete products;

	for (unordered_map<string, unordered_map<string, int>*>::iterator it0 = table->begin(); it0 != table->end(); ++it0)
	{
		delete it0->second;
	}
	delete table;

	for (int it0 = 0; it0 < product_items->size(); ++it0)
	{
		delete product_items->at(it0);
	}
	delete product_items;
}

// �ص�����,��ȫ�ֱ�����ӱ��
void getTokens(const char* token, int type)
{
	if (type == 0)
		tokens->push_back("id");
	else
		tokens->push_back(token);
}


void Parser_ver1::parse(const char * fileName, bool analyze, ostream& output)
{
	// �ڿ�ʼ֮ǰ,������һ��Ǻ�
	tokens = new vector<string>();
	FILE* in;
	if (fileName == NULL)		// ��ָ��,�����û�����
	{
		in = _lex_getInput();
		
	}
	else
	{
		in = fopen(fileName, "r");
	}
	_lex_enable(in, NULL, getTokens);
	// ջ:���Ƚ���һ���󲿺��ս����ջ
	stack<string> analyze_stack;
	analyze_stack.push("$");
	string s = products->at(0);
	analyze_stack.push(s.substr(0,s.find("->")));

	int pi = 0;	// ָ��Ǻ���
	while (!analyze_stack.empty())
	{
		string staTop = analyze_stack.top();
		analyze_stack.pop();
		string currIn = (*tokens)[pi];

		if (currIn == staTop)		// ����Ϊջ���ս��
		{
			pi++;
			output << "����:" << currIn << endl;
		}
		else                          // �����ս��
		{
			int pos = 0;
			try {
				pos = (*table)[staTop]->at(currIn);
			}
			catch (exception& e)
			{
				output << "�﷨����:δ֪���� " << currIn << endl;
				break;
			}
			
			vector<string>& to_add = *product_items->at(pos);
			for (int it0 = to_add.size() - 1; it0 >= 0; it0--)
			{
				analyze_stack.push(to_add[it0]);		// �������
			}
			output << "����:" << products->at(pos) << endl;
		}
	}

	delete tokens;
	if (fileName)
		fclose(in);
}
