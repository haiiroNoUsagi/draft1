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
	void* grammar_inst = _Grammar_getInstance(program_file);		// 获取文法实例.注意,在生成预测分析表之后,文法就可以删除了.
																	// 因此,仅作为临时变量
	products = new vector<string>(*_Grammar_getProductions(grammar_inst));			// 拷贝文法中的产生式列表

	// TODO:此处添加初始化非终结符和终结符列表的方法
	for (int it0 = 0; it0 < products->size(); ++it0)			// 外层循环:对每一个产生式 A->α
	{
		// 顺便初始化产生式的记号串
		product_items->push_back(new vector<string>(*_Grammar_getProdItems(grammar_inst, it0)));
		// 首先在table创建产生式的表项
		string curr = products->at(it0);
		string left = curr.substr(0, curr.find("->"));
		string right = curr.substr(curr.find(">") + 1);
		if (table->count(left) == 0)
		{
			table->insert(pair<string, unordered_map<string,int>*>(left, new unordered_map<string, int>()));
		}
		unordered_set<string>& first_alpha = *_Grammar_getFirst(grammar_inst, right);
		bool hasEmpty = false;
		for (unordered_set<string>::iterator it1 = first_alpha.begin(); it1 != first_alpha.end(); ++it1)	// 对每一个终结符都a,将产生式加入[A, a]
		{
			if (*it1 != "~")						// 注意,如果文法不是LL1的,将会覆盖,并给出警告
			{
				table->at(left)->insert(pair<string,int>(*it1, it0));
			}
			else
			{
				hasEmpty = true;										// 有空串,则执行下一条:
			}
		}

		if (hasEmpty)
		{
			unordered_set<string>& follow_A = *_Grammar_getFollow(grammar_inst, left);			// 注意,是把A->α全部加入,不单单指这一条
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

// 回调函数,向全局变量添加标记
void getTokens(const char* token, int type)
{
	if (type == 0)
		tokens->push_back("id");
	else
		tokens->push_back(token);
}


void Parser_ver1::parse(const char * fileName, bool analyze, ostream& output)
{
	// 在开始之前,必须获得一组记号
	tokens = new vector<string>();
	FILE* in;
	if (fileName == NULL)		// 不指定,则由用户输入
	{
		in = _lex_getInput();
		
	}
	else
	{
		in = fopen(fileName, "r");
	}
	_lex_enable(in, NULL, getTokens);
	// 栈:首先将第一个左部和终结符入栈
	stack<string> analyze_stack;
	analyze_stack.push("$");
	string s = products->at(0);
	analyze_stack.push(s.substr(0,s.find("->")));

	int pi = 0;	// 指向记号流
	while (!analyze_stack.empty())
	{
		string staTop = analyze_stack.top();
		analyze_stack.pop();
		string currIn = (*tokens)[pi];

		if (currIn == staTop)		// 输入为栈顶终结符
		{
			pi++;
			output << "接收:" << currIn << endl;
		}
		else                          // 不是终结符
		{
			int pos = 0;
			try {
				pos = (*table)[staTop]->at(currIn);
			}
			catch (exception& e)
			{
				output << "语法错误:未知符号 " << currIn << endl;
				break;
			}
			
			vector<string>& to_add = *product_items->at(pos);
			for (int it0 = to_add.size() - 1; it0 >= 0; it0--)
			{
				analyze_stack.push(to_add[it0]);		// 反向加入
			}
			output << "规则:" << products->at(pos) << endl;
		}
	}

	delete tokens;
	if (fileName)
		fclose(in);
}
