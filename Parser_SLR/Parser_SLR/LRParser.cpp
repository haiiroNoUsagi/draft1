#include "LRParser.h"



void LRParser::new_I(string prod,vector<string>& right,unordered_set<string>& target_set)
{
	
	string left = prod.substr(0, prod.find("->"));
	
	string r = prod.substr(prod.find("->") + 2);

	string I = left + "->��" + (r=="~"?"":r);

	// ��������,�����
	if (I_map->count(I) == 0)
	{
		Item_1* iexpr = new Item_1();
		iexpr->raw = prod;
		iexpr->point = 0;
		iexpr->prod_NOT_DELETE = &right;

		// ���ӳ��
		I_map->insert(pair<string, Item_1*>(I, iexpr));
	}

	target_set.insert(I);
}

void LRParser::next_I(string lastProd, Item_1& lastI, unordered_set<string>& target_set)
{
	int size = lastI.prod_NOT_DELETE->size();
	if (size == 0)
		return;
	// ע��!!!�ڵ�ռ��2���ֽ�
	int iPre = lastProd.find("->") + 2;
	string prefix = lastProd.substr(0, iPre); 
	int pos = 0;
	while (pos <= lastI.point)
	{
		prefix += lastI.prod_NOT_DELETE->at(pos);
		pos++;
	}
	prefix += "��";
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

		// ���ӳ��
		I_map->insert(pair<string, Item_1*>(prefix, iexpr));
	}
	
	target_set.insert(prefix);
}

unordered_set<string>* LRParser::checkExist(unordered_set<string>* NewItem)
{
	int size = itemSet_Family->size();

	// ���ڱ������е�In,�����Ƿ��д��ڵ�
	for (int it0 = 0; it0 < size; ++it0)
	{
		unordered_set<string>& In = *itemSet_Family->at(it0);
		// ���ȴ�СҪ��ͬ
		// ���,Ԫ��Ҫ��ͬ
		
		if (NewItem->size() == In.size())
		{
			bool found = true;
			for (unordered_set<string>::iterator it1 = In.begin(); it1 != In.end(); ++it1)
			{
				// ���ڲ���ͬԪ��
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

	// ����1: ��I�е�������Ŀ����res
	for (unordered_set<string>::iterator it = I.begin(); it != I.end(); ++it)
	{
		res->insert(*it);
	}

	// ����2: ���I�еĲ���ʽ,�Ƿ���ŷ��ս��?
	bool added = true;
	// ���ѭ��:����Ѱ�ҷ��������Ķ���,ֱ�����������Ϊֹ.
	while (added)
	{
		added = false;
		// ��ʱ����:�����ڵ�֮��,��û�м���res�ķ��ս��,���Ĳ���ʽԭʽ
		vector<pair<string,int>> prod2Add;

		for (unordered_set<string>::iterator it = res->begin(); it != res->end(); ++it)
		{
			// ��ȡ���ǵ���Ŀ
			Item_1& prod = *I_map->at(*it);

			//�������
			if (prod.point == prod.prod_NOT_DELETE->size())
				continue;

			string expect = prod.prod_NOT_DELETE->at(prod.point);
			// �����󷽴��ڷ��ս��
			if (expect[0] >= 'A'&&expect[0] <= 'W')
			{
				// ��������ʽ�б�,���������Ķ���ӵ�res��
				for (int i = 0; i < _grammar_prodList->size(); ++i)
				{
					string _prod = _grammar_prodList->at(i);

					// ��������,�Ҳ�������res�е�
					if (_prod.substr(0, _prod.find("->")) == expect&&res->count(_prod.substr(0, _prod.find("->") + 2)+"��"+ _prod.substr(_prod.find("->")+2))==0)
					{
						// ��ӵ���ʱ�б�
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


	//���ȴ�������,�鿴In���Ƿ��п�����ӵ�ʽ��
	unordered_set<string>* NextI = new unordered_set<string>();
	
	unordered_set<string>* target = itemSet_Family->at(In);

	//����In
	for (unordered_set<string>::iterator it0 = target->begin(); it0 != target->end(); ++it0)
	{
		Item_1& I = *I_map->at(*it0);
		// �����symbol���������ָԪ����ͬ,��ô��NextI���
		if (I.point< I.prod_NOT_DELETE->size()&&I.prod_NOT_DELETE->at(I.point) == input)
		{
			next_I(*it0,I,*NextI);
		}
	}
	
	// ������Goto
	if (NextI->size() > 0)
	{
		///////////////////////////////////////-----�ٴκ˶Դ˴����߼�
		// ������һ��,����ĳЩ�����ʹ���ص�֮ǰ��ĳ��״̬,����ȴ����е�״̬LRIѰ���Ƿ������ȫ��ͬ��״̬
		//////////////////////////////////////////////////////////////////////////////��������
		/*
		NextI���ص���Ŀ��δ����հ�,����same����NULL
		��������հ�֮��,NextI���ܻ���֮ǰ��ĳ��In�ظ�
		
		*/
		
		// ��LRI����NextI,Ȼ����հ�
		itemSet_Family->push_back(NextI);
		unordered_set<string>* c = closure(itemSet_Family->size() - 1);


		unordered_set<string>* same = checkExist(NextI);

		if (same == NULL)
		{
			// �ȼ���,Ȼ�������ıհ�,��ȫ������NextI��
			gotos->insert(pair<string, unordered_set<string>*>(to_string(In) + input, NextI));
			
			for (unordered_set<string>::iterator it0 = c->begin(); it0 != c->end(); ++it0)
			{
				if (NextI->count(*it0) == 0)
				{
					NextI->insert(*it0);
				}
			}
		}
		// �������ͬ��,ɾ��������,�����ؾ�ָ��
		else
		{
			gotos->insert(pair<string, unordered_set<string>*>(to_string((itemSet_Family->size())) + input, same));
			delete NextI;
			return same;
		}
	}
	else
	{
		// ������,��ɾ��NextI,���˳�
		delete NextI;
		return NULL;
	}
	

	return NextI;
}

void LRParser::LR0_build(unordered_set<string>&TokenList,int In)
{
	int size = TokenList.size();
	// ����֮ǰ,������һ����ֵ�б������

	// �������:����ÿһ�����ŵ�goto���
	for (unordered_set<string>::iterator it0 = TokenList.begin();it0!=TokenList.end();++it0)
	{
		// ����ǰ,��¼LRI��ֵ
		int before = itemSet_Family->size();
		unordered_set<string>* res = _goto(In, *it0);
		//�������µ�In
		if (res != NULL && itemSet_Family->size() > before)
		{
			//���µ�In���еݹ�ļ��(-1)
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
	// ������Ҫ�ó��ķ������в���ʽ
	// ע��,��������������ȼ�������Ƿ�Ϸ�
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

	// ��ʼ���ⲿ����
	this->_grammar_getProd = _grammar_getProd;
	this->_grammar_getFirst = _grammar_getFirst;
	this->_grammar_getFollow = _grammar_getFollow;

	// ��չ����"S'->��ʼ��"
	string temp = _grammar_prodList->at(0);
	string temphead = temp.substr(0, temp.find("->"));
	extendProd = "S'->" + temphead;

	// ���ȴ�����һ����Ŀ��I0
	unordered_set<string>* I0 = new unordered_set<string>();
	I0->insert("S'->��" + temphead);
	// ��չ�ķ���ӳ��,�Լ���Ӧ���ʵ��
	Item_1* iexpr = new Item_1();
	iexpr->raw = extendProd;
	iexpr->point = 0;
	extendProd_Vector = new vector<string>();
	iexpr->prod_NOT_DELETE = extendProd_Vector;
	iexpr->prod_NOT_DELETE -> push_back(temphead);

	// ��ӵ�����Ŀ��ӳ��
	I_map->insert(pair<string, Item_1*>("S'->��" + temphead, iexpr));

	// ���I0��LRI��
	itemSet_Family->push_back(I0);
	
	// ��I0�ıհ�Ҳ��ӽ���
	unordered_set<string>* c = closure(0);
	for (unordered_set<string>::iterator it0 = c->begin(); it0 != c->end(); ++it0)
	{
		if (I0->count(*it0) == 0)
		{
			I0->insert(*it0);
		}
	}


	//-----------------------
	//����,������I0 ������Ҫ������Ŀ���淶��I1`In
	//�ⲿ��Ӧ�����õݹ�ķ���
	//-----------------------	

	//��ȡһ�����ŵ��б�
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
		// ����ÿһ������ʽ,��ȡ���мǺ�
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
