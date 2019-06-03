#include "LRParser.h"



void LRParser::new_I(string prod,vector<string>& right,unordered_set<string>& target_set)
{
	
	string left = prod.substr(0, prod.find("->"));
	
	string r = prod.substr(prod.find("->") + 2);

	string I = left + "->·" + (r=="~"?"":r);

	// 若不存在,则添加
	if (I_map->count(I) == 0)
	{
		Item_1* iexpr = new Item_1();
		iexpr->raw = prod;
		iexpr->point = 0;
		iexpr->prod_NOT_DELETE = &right;

		// 添加映射
		I_map->insert(pair<string, Item_1*>(I, iexpr));
	}

	target_set.insert(I);
}

void LRParser::next_I(string lastProd, Item_1& lastI, unordered_set<string>& target_set)
{
	int size = lastI.prod_NOT_DELETE->size();
	if (size == 0)
		return;
	// 注意!!!黑点占了2个字节
	int iPre = lastProd.find("->") + 2;
	string prefix = lastProd.substr(0, iPre); 
	int pos = 0;
	while (pos <= lastI.point)
	{
		prefix += lastI.prod_NOT_DELETE->at(pos);
		pos++;
	}
	prefix += "·";
	while (pos < size)
	{
		prefix += lastI.prod_NOT_DELETE->at(pos);
		pos++;
	}

	if (I_map->count(prefix) == 0)
	{
		Item_1* iexpr = new Item_1();
		iexpr->raw =prefix;
		iexpr->point = lastI.point + 1;
		iexpr->prod_NOT_DELETE = lastI.prod_NOT_DELETE;

		// 添加映射
		I_map->insert(pair<string, Item_1*>(prefix, iexpr));
	}
	
	target_set.insert(prefix);
}

unordered_set<string>* LRParser::checkExist(unordered_set<string>* NewItem)
{
	int size = itemSet_Family->size();

	// 现在遍历所有的In,看看是否有存在的
	for (int it0 = 0; it0 < size; ++it0)
	{
		unordered_set<string>& In = *itemSet_Family->at(it0);
		// 首先大小要相同
		// 其次,元素要相同
		
		if (NewItem->size() == In.size())
		{
			bool found = true;
			for (unordered_set<string>::iterator it1 = In.begin(); it1 != In.end(); ++it1)
			{
				// 存在不相同元素
				if (NewItem->count(*it1) == 0)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return &In;
			}
		}
	}
	return NULL;
}

unordered_set<string>* LRParser::closure(int In)
{
	if (closures->count(In) != 0)
	{
		return closures->at(In);
	}
	unordered_set<string>& I = *itemSet_Family->at(In);
	unordered_set<string>* res = new unordered_set<string>();

	// 规则1: 将I中的所有项目加入res
	for (unordered_set<string>::iterator it = I.begin(); it != I.end(); ++it)
	{
		res->insert(*it);
	}

	// 规则2: 检查I中的产生式,是否跟着非终结符?
	bool added = true;
	// 外层循环:不断寻找符合条件的对象,直到不能再添加为止.
	while (added)
	{
		added = false;
		// 临时变量:存在于点之后,且没有加入res的非终结符,它的产生式原式
		vector<pair<string,int>> prod2Add;

		for (unordered_set<string>::iterator it = res->begin(); it != res->end(); ++it)
		{
			// 获取它们的项目
			Item_1& prod = *I_map->at(*it);

			//到了最后
			if (prod.point == prod.prod_NOT_DELETE->size())
				continue;

			string expect = prod.prod_NOT_DELETE->at(prod.point);
			// 如果点后方存在非终结符
			if (expect[0] >= 'A'&&expect[0] <= 'W')
			{
				// 遍历产生式列表,由他产生的都添加到res中
				for (int i = 0; i < _grammar_prodList->size(); ++i)
				{
					string _prod = _grammar_prodList->at(i);

					// 由它产生,且不存在于res中的
					if (_prod.substr(0, _prod.find("->")) == expect&&res->count(_prod.substr(0, _prod.find("->") + 2)+"·"+ _prod.substr(_prod.find("->")+2))==0)
					{
						// 添加到临时列表
						prod2Add.push_back(pair<string,int>(_prod,i));
					}
				}
			}

			for (int i1 = 0; i1 < prod2Add.size(); ++i1)
			{
				added = true;
				new_I(prod2Add[i1].first, *_grammar_getProd(grammar, prod2Add[i1].second), *res);
			}
		}
	}
	

	closures->insert(pair<int, unordered_set<string>*>(In, res));
	return res;
}

unordered_set<string>* LRParser::_goto(int In, string input)
{
	if (gotos->count(to_string(In)+input) != 0)
	{
		return gotos->at((to_string(In) + input));
	}


	//首先创建集合,查看In中是否有可以添加的式子
	unordered_set<string>* NextI = new unordered_set<string>();
	
	unordered_set<string>* target = itemSet_Family->at(In);

	//遍历In
	for (unordered_set<string>::iterator it0 = target->begin(); it0 != target->end(); ++it0)
	{
		Item_1& I = *I_map->at(*it0);
		// 输入的symbol如果与点后所指元素相同,那么向NextI添加
		if (I.point< I.prod_NOT_DELETE->size()&&I.prod_NOT_DELETE->at(I.point) == input)
		{
			next_I(*it0,I,*NextI);
		}
	}
	
	// 若存在Goto
	if (NextI->size() > 0)
	{
		///////////////////////////////////////-----再次核对此处的逻辑
		// 这里有一点,由于某些输入会使它回到之前的某种状态,因此先从已有的状态LRI寻找是否存在完全相同的状态
		//////////////////////////////////////////////////////////////////////////////问题所在
		/*
		NextI返回的项目并未求过闭包,所以same返回NULL
		但是求过闭包之后,NextI可能会与之前的某个In重复
		
		*/
		
		// 向LRI加入NextI,然后求闭包
		itemSet_Family->push_back(NextI);
		unordered_set<string>* c = closure(itemSet_Family->size() - 1);


		unordered_set<string>* same = checkExist(NextI);

		if (same == NULL)
		{
			// 先加入,然后求它的闭包,并全部加入NextI中
			gotos->insert(pair<string, unordered_set<string>*>(to_string(In) + input, NextI));
			
			for (unordered_set<string>::iterator it0 = c->begin(); it0 != c->end(); ++it0)
			{
				if (NextI->count(*it0) == 0)
				{
					NextI->insert(*it0);
				}
			}
		}
		// 如果有相同的,删除多余量,并返回旧指针
		else
		{
			gotos->insert(pair<string, unordered_set<string>*>(to_string((itemSet_Family->size())) + input, same));
			delete NextI;
			return same;
		}
	}
	else
	{
		// 不存在,则删除NextI,并退出
		delete NextI;
		return NULL;
	}
	

	return NextI;
}

void LRParser::LR0_build(unordered_set<string>&TokenList,int In)
{
	int size = TokenList.size();
	// 调用之前,必须有一个左值列表的引用

	// 深度优先:检查对每一个符号的goto结果
	for (unordered_set<string>::iterator it0 = TokenList.begin();it0!=TokenList.end();++it0)
	{
		// 运行前,记录LRI的值
		int before = itemSet_Family->size();
		unordered_set<string>* res = _goto(In, *it0);
		//产生了新的In
		if (res != NULL && itemSet_Family->size() > before)
		{
			//对新的In进行递归的检查(-1)
			LR0_build(TokenList, itemSet_Family->size()-1);
		}
	}
}

LRParser::LRParser()
{
	enabled = false;
	extendProd_Vector = new vector<string>();
	I_map = new unordered_map<string, Item_1*>();
	itemSet_Family = new vector<unordered_set<string>*>();
	closures = new unordered_map<int, unordered_set<string>*>();
	gotos = new unordered_map<string, unordered_set<string>*>();
}


LRParser::~LRParser()
{
	delete extendProd_Vector;
	for (unordered_map<string, Item_1*>::iterator it = I_map->begin(); it != I_map->end(); ++it)
	{
		delete it->second;
	}
	delete I_map;

	for (int it = 0; it < itemSet_Family->size(); ++it)
	{
		delete itemSet_Family->at(it);
	}
	delete itemSet_Family;

	for (unordered_map<int, unordered_set<string>*>::iterator it = closures->begin(); it != closures->end(); ++it)
	{
		delete it->second;
	}
	delete closures;

	for (unordered_map<string, unordered_set<string>*>::iterator it = gotos->begin(); it != gotos->end(); ++it)
	{
		delete it->second;
	}
	delete gotos;
}

bool LRParser::isEnabled()
{
	return enabled;
}

bool LRParser:: init(
	void* grammar,pf_pvectorSTR__pvoid function_getAllProd, 
	pf_pvectorSTR__pvoid_int _grammar_getProd,
	pf_pusetSTR__pvoid_string _grammar_getFirst,
	pf_pusetSTR__pvoid_string _grammar_getFollow
){
	this->grammar = grammar;
	// 首先需要得出文法的所有产生式
	// 注意,这个动作将会率先检验参数是否合法
	try {
		_grammar_prodList = function_getAllProd(grammar);
		if (_grammar_prodList->size() == 0)
		{
			return false;
		}
	}
	catch (exception& e)
	{
		return false;
	}

	// 初始化外部变量
	this->_grammar_getProd = _grammar_getProd;
	this->_grammar_getFirst = _grammar_getFirst;
	this->_grammar_getFollow = _grammar_getFollow;

	// 拓展采用"S'->起始符"
	string temp = _grammar_prodList->at(0);
	string temphead = temp.substr(0, temp.find("->"));
	extendProd = "S'->" + temphead;

	// 首先创建第一个项目集I0
	unordered_set<string>* I0 = new unordered_set<string>();
	I0->insert("S'->·" + temphead);
	// 拓展文法的映射,以及对应类的实现
	Item_1* iexpr = new Item_1();
	iexpr->raw = extendProd;
	iexpr->point = 0;
	extendProd_Vector = new vector<string>();
	iexpr->prod_NOT_DELETE = extendProd_Vector;
	iexpr->prod_NOT_DELETE -> push_back(temphead);

	// 添加单个项目的映射
	I_map->insert(pair<string, Item_1*>("S'->·" + temphead, iexpr));

	// 添加I0到LRI中
	itemSet_Family->push_back(I0);
	
	// 将I0的闭包也添加进来
	unordered_set<string>* c = closure(0);
	for (unordered_set<string>::iterator it0 = c->begin(); it0 != c->end(); ++it0)
	{
		if (I0->count(*it0) == 0)
		{
			I0->insert(*it0);
		}
	}


	//-----------------------
	//至此,已生成I0 接下来要生成项目集规范族I1`In
	//这部分应当采用递归的方法
	//-----------------------	

	//获取一个符号的列表
	unordered_set<string> token_list;
	string last = "";
	for (int i = 0; i < _grammar_prodList->size(); ++i)
	{
		string cur = _grammar_prodList->at(i);
		string sub = cur.substr(0, cur.find("->"));
		if (sub != last)
		{
			token_list.insert(sub);
			last = sub;
		}
		// 对于每一个产生式,读取所有记号
		vector<string>& right = *_grammar_getProd(grammar,i);
		for (int i1 = right.size() - 1; i1 >= 0; --i1)
		{
			token_list.insert(right[i1]);
		}
	}

	LR0_build(token_list, 0);

	return true;
}

void LRParser::parse(vector<string>& tokens, ostream & output)
{
	if (!enabled)
		return;
}
