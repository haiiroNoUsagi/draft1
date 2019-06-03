#include "LRParser_2.h"



unordered_set<string>* LRParser_2::closure(unordered_set<string>& Items)
{
	//先计算后判断是否已存在;

	unordered_set<string>* res = new unordered_set<string>();
	// 规则1: 将I中的所有项目加入res
	for (unordered_set<string>::iterator it = Items.begin(); it != Items.end(); ++it)
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
		vector<pair<string, int>> prod2Add;

		for (unordered_set<string>::iterator it = res->begin(); it != res->end(); ++it)
		{
			// 获取它们的项目
			Item& prod = *map_item2Item->at(*it);

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
					if (
						_prod.substr(0, _prod.find("->")) == expect 
						&& res->count(
							_prod.substr(0, _prod.find("->") + 2) + "·" + 
							// 此处注意忽略空串,例如A->~加点后为A->dot
							(_prod.substr(_prod.find("->") + 2) == "~"?"": (_prod.substr(_prod.find("->") + 2)))
						) == 0)
					{
						// 添加到临时列表
						prod2Add.push_back(pair<string, int>(_prod, i));
					}
				}
			}
		}

		int st = res->size();
		for (int i1 = 0; i1 < prod2Add.size(); ++i1)
		{
			res->insert(getOneItem(prod2Add[i1].first,*_grammar_getProd(grammar, prod2Add[i1].second), 0));
		}
		added = st != res->size();
	}
	
	// 完成对res的计算,返回res 或是一个旧指针
	int index = checkExist_In_SISF(*res);
	if (index >= 0)
	{
		delete res;
		return ItemSets->at(index);
	}
	else
	{
		ItemSets->push_back(res);
		return res;
	}
}

unordered_set<string>* LRParser_2::_goto(int In, string input)
{
	// 从映射寻找函数goto是否已经存在
	if (map_gotoFunc_In->count(to_string(In) + input) != 0)
	{
		int index = map_gotoFunc_In->at((to_string(In) + input));
		return ItemSets->at(index);
	}


	//首先创建集合,查看In中是否有可以添加的式子
	unordered_set<Item*> Next;

	unordered_set<string>* target = ItemSets->at(In);

	//遍历In
	for (unordered_set<string>::iterator it0 = target->begin(); it0 != target->end(); ++it0)
	{
		Item& I = *map_item2Item->at(*it0);
		// 输入的symbol如果与点后所指元素相同,那么向NextI添加
		if (I.point < I.prod_NOT_DELETE->size() && I.prod_NOT_DELETE->at(I.point) == input)
		{
			Next.insert(&I);
		}
	}
	
	if (Next.size() > 0)
	{
		// 先将它们的点向后跳一位
		unordered_set<string> NextI;
		for (unordered_set<Item*>::iterator it = Next.begin(); it != Next.end(); ++it)
		{
			// 将它们的点向后移动
			NextI.insert(getOneItem((*it)->raw,*(*it)->prod_NOT_DELETE,(*it)->point+1));
		}
		// 直接返回它的闭包,因为closure可以返回旧的In 
		unordered_set<string>* temp = closure(NextI);
		int index = checkExist_In_SISF(*temp);
		map_gotoFunc_In->insert(pair<string, int>(to_string(In) + input, index == -1 ? ItemSets->size() - 1 : index));
		return temp;
	}
	else
	{
		// 没法生成下一个集合,返回空
		return NULL;
	}
}

int LRParser_2::checkExist_In_SISF(unordered_set<string>& NewItemSet)
{
	int size = ItemSets->size();

	// 现在遍历所有的In,看看是否有存在的
	for (int it0 = 0; it0 < size; ++it0)
	{
		unordered_set<string>& In = *ItemSets->at(it0);
		// 首先大小要相同
		// 其次,元素要相同

		if (NewItemSet.size() == In.size())
		{
			bool found = true;
			for (unordered_set<string>::iterator it1 = In.begin(); it1 != In.end(); ++it1)
			{
				// 存在不相同元素
				if (NewItemSet.count(*it1) == 0)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return it0;
			}
		}
	}

	return -1;
}

string LRParser_2::getOneItem(string raw,vector<string>& rawProd, int point)
{
	// 创建新的项目
	string item = raw.substr(0,raw.find("->")+2);
	int len = rawProd.size();
	int i = 0;
	while (i < point)
	{
		item += rawProd[i++];
	}
	item += "·";
	while (i < len)
	{
		item += rawProd[i++];
	}
	/*bool NOTemptyStr = raw.find("~") == -1;
	if (NOTemptyStr)
	{
		while (i < point)
		{
			item += rawProd[i++];
		}
		item += "·";
		while (i < len)
		{
			item += rawProd[i++];
		}
	}
	else
	{
		item += "·";
	}
*/


	// 若不存在,则添加映射
	if (map_item2Item->count(item) == 0)
	{
		Item* iexpr = new Item();
		iexpr->raw = raw;
		//iexpr->point = NOTemptyStr ? point : 1 ;
		iexpr->point = point ;
		iexpr->prod_NOT_DELETE = &rawProd;

		// 添加映射
		map_item2Item->insert(pair<string, Item*>(item, iexpr));
	}

	// 返回生成的字符串
	// 无论如何,这个字串一定能找到一个Item的实例
	return item;
}

void LRParser_2::LR0_build(unordered_set<string>& TokenList, int In)
{
	int size = TokenList.size();
	// 调用之前,必须有一个左值列表的引用

	// 深度优先:检查对每一个符号的goto结果
	for (unordered_set<string>::iterator it0 = TokenList.begin(); it0 != TokenList.end(); ++it0)
	{
		// 运行前,记录LRI的值
		int before = ItemSets->size();
		unordered_set<string>* res = _goto(In, *it0);
		//产生了新的In
		if (res != NULL && ItemSets->size() > before)
		{
			//对新的In进行递归的检查(-1)
			LR0_build(TokenList, ItemSets->size() - 1);
		}
	}
}

void LRParser_2::parser_build()
{
	

	//外一层循环:遍历项目集
	int size = ItemSets->size();
	for (int it_n = 0; it_n < size; ++it_n)
	{
		unordered_map<string, Operation*>* newAction = new unordered_map<string, Operation*>();
		unordered_map<string, Operation*>* newGoto = new unordered_map<string, Operation*>();
		//it_n即是状态.为每一个状态建立内容
		action_stat_input__act -> insert(pair<int, unordered_map<string, Operation*>*>(it_n,newAction));

		//获取项目集In
		unordered_set<string>& In = *ItemSets->at(it_n);
		//遍历项目集
		for (unordered_set<string>::iterator In_iter = In.begin(); In_iter != In.end(); ++In_iter)
		{
			// 获取必要单个的项目
			string raw = *In_iter;
			Item& I = *map_item2Item->at(raw);

			// 在这里创建新的条目
			Operation* no = new Operation();
			
			// 点后面有记号
			if (I.point < I.prod_NOT_DELETE->size())
			{
				string behind_point = I.prod_NOT_DELETE->at(I.point);

				int g = map_gotoFunc_In->at(to_string(it_n) + behind_point);
				newAction->insert(pair<string, Operation*>(behind_point, no));

				no->operate = g;
				
				// 2.c 点后为非终结符
				if (behind_point[0] >= 'A'&&behind_point[0] <= 'W')
				{
					// 这时候寻找goto
					// goto(Ii,a) = Ij => goto [i,A] = j
					no->type = 999;
				}
				// 2.a 点后为终结符
				else{
					// 将 action[it_n,a] 为sg 即 移进并将状态g入栈
					// goto(Ii,a) = Ij => action[i,a] = sj
					no->type = 1;
				}
			}
			//点已经在式子的末尾
			else// if (I.point == I.prod_NOT_DELETE->size())
			{
				//2.c
				if (raw.substr(0, raw.find("->")) == "S'")
				{
					no->type = 0;
					// 接收结束符$
					newAction->insert(pair<string,Operation*>("$",no));
				}
				else
				{
					// 获取follow集合,并将集合中的每一个终结符a ,置入action[it_n,a]为rj
					// j,是该项目原产生式列表的编号,即raw.substr(0,raw.find("点"));

					unordered_set<string>& follow_I = *_grammar_getFollow(grammar, raw.substr(0, raw.find("->")));

					int j = 0;
					// 点占两位,因此-2
					string rp = raw.substr(0, raw.length() - 2);
					// 处理空串
					//if (rp.length() == raw.find("·"))
					//{
					//	rp += '~';
					//}
					while (_grammar_prodList->at(j) != rp)
					{
						j++;
						/*if (j >= _grammar_prodList->size())
						{
							throw exception("找不到follow集中对应的产生式");
						}*/
					}
					//按产生式规约
					
					for (unordered_set<string>::iterator it_follow = follow_I.begin(); it_follow != follow_I.end(); ++it_follow)
					{
						no->type = 2;
						no->operate = j;
						// 此处注意规约-规约冲突.
						if (newAction->count(*it_follow) != 0)
						{
							throw exception("发生了规约-规约冲突! 这不是SLR(1)文法. 生成失败!");
							exit(-1);
						}
						// 一个operate不应被多个string对应,不利于回收
						newAction->insert(pair<string, Operation*>(*it_follow, no));
						no = new Operation();
					}

					delete no;
				}
				
			}
		}
	}
}


LRParser_2::LRParser_2()
{
	enabled = false;

	ItemSets = new vector<unordered_set<string>*>();

	extendProd_Vector = new vector<string>();

	map_item2Item = new unordered_map<string, Item*>();			//某项目字符串与项目类的映射

	map_gotoFunc_In = new unordered_map<string, int>();			//goto函数的结果会前往的项目集

	action_stat_input__act = new unordered_map<int, unordered_map<string, Operation*>*>();

	Errmsg = new vector<string>();
}


LRParser_2::~LRParser_2()
{
	for (int i = 0; i < ItemSets->size(); ++i)
	{
		delete ItemSets->at(i);
	}
	delete ItemSets;

	for (unordered_map<string, Item*>::iterator it = map_item2Item->begin(); it != map_item2Item->end(); ++it)
	{
		delete it->second;
	}
	delete extendProd_Vector;
	delete map_item2Item;
	delete map_gotoFunc_In;

	for (unordered_map<int, unordered_map<string, Operation*>*>::iterator it = action_stat_input__act->begin(); it != action_stat_input__act->end(); ++it)
	{
		for (unordered_map<string, Operation*>::iterator it2 = it->second->begin(); it2 != it->second->end(); ++it2)
		{
			delete it2->second;
		}
		delete it->second;
	}
	delete action_stat_input__act;

	delete Errmsg;
}

bool LRParser_2::isEnabled()
{
	return enabled;
}


bool LRParser_2::init(
	void* grammar, pf_pvectorSTR__pvoid function_getAllProd,
	pf_pvectorSTR__pvoid_int _grammar_getProd,
	pf_pusetSTR__pvoid_string _grammar_getFirst,
	pf_pusetSTR__pvoid_string _grammar_getFollow
) {
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

	// 首先创建第一个项目集I0
	unordered_set<string>* newI = new unordered_set<string>();
	newI->insert("S'->·" + temphead);
	// 拓展文法的映射,以及对应类的实现
	Item* iexpr = new Item();
	iexpr->raw = "S'->" + temphead;
	iexpr->point = 0;
	extendProd_Vector = new vector<string>();
	iexpr->prod_NOT_DELETE = extendProd_Vector;
	iexpr->prod_NOT_DELETE->push_back(temphead);

	// 添加单个项目的映射
	map_item2Item->insert(pair<string, Item*>("S'->·" + temphead, iexpr));

	// 获取它的闭包,无需返回,因为他已添加即可
	closure(*newI);


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
		vector<string>& right = *_grammar_getProd(grammar, i);
		for (int i1 = right.size() - 1; i1 >= 0; --i1)
		{
			token_list.insert(right[i1]);
		}
	}

	LR0_build(token_list, 0);

	// 执行完毕之后,生成了项目集规范族.现在构造SLR分析表
	parser_build();

	//分析表构造完成,返回true
	enabled = true;
	return true;
}

void LRParser_2::add_ignorableErr(string symbol, string comment)
{
	// 查询整个分析表,找到不接收符号的位置
	for (unordered_map<int, unordered_map<string, Operation*>*>::iterator it = action_stat_input__act->begin(); it != action_stat_input__act->end(); ++it)
	{
		if (it->second->count(symbol) == 0)
		{
			// 向它添加错误处理Operation
			Operation* e = new Operation();
			e->type = 3;
			e->operate = Errmsg->size();
			it->second->insert(pair<string, Operation*>(symbol, e));
		}
	}

	Errmsg->push_back(comment);
}

void LRParser_2::parse(vector<pair<string, string>>& tokens, ostream & output)
{
	if (!enabled)
	{
		output << "不能使用未初始化的分析器,请运行init()" << endl;
		return;
	}
		

	output.flush();

	//创建两个栈
	stack<string> tokenStack;
	stack<int> statStack;
	statStack.push(0);
	int size = tokens.size();
	bool hasErr = false;

	//外层循环:获取下一个标记
	for (int itt = 0; itt < size; ++itt)
	{
		// 对于每个标记,查询分析表!
		// string curr = tokens[itt].second;

		// 此处判断类型
		string curr = tokens[itt].second == "value"?"id": tokens[itt].first;

		unordered_map<string, Operation*>* check = action_stat_input__act->at(statStack.top());
		
		//若无法找到,将会报错.
		if (check->count(curr) == 0)
		{
			// 结束符是比较特殊的,如果提前收到了结束符,那么很可能缺少某些符号
			if (curr == "$")
			{
				// 在当前状态尝试寻找合适的符号进行补齐
				output << "!!错误:过早地出现了结束符." << endl;
				hasErr = true;
				bool found = false;
				for (unordered_map<string, Operation*>::iterator iend = check->begin(); iend != check->end(); ++iend)
				{
					// 提前出现的结束符一定是规约未完成,因此寻找一个可以移进,并且移进之后可以规约的选项
					if (iend->second->type == 1)
					{
						int nextStat = iend->second->operate;
						// 移进之后可以根据$规约
						if (action_stat_input__act->at(nextStat)->count("$")!=0&&action_stat_input__act->at(nextStat)->at("$")->type == 2)
						{
							output << "??????尝试补充符号: " << iend->first << "  继续分析..." << endl;

							// 在这里修改curr
							curr = iend->first;

							// 然后将itt-1,使之继续指向$
							itt--;
							found = true;
							break;
						}
					}
				}

				if (!found)
				{
					output <<"无法尝试添加缺少的符号进行进一步的分析.结论:输入不通过语法分析"<< endl << "===结束分析" << endl;
					return;
				}
			}
			else
			{
				output << "error:发生错误,但无法找到条目:err(" << statStack.top() << "," << curr << "):未知错误类型"
					<< endl << "结论:输入不通过语法分析" << endl;
				output << endl << "===结束分析" << endl;
				return;
			}

		}

		Operation* o = check->at(curr);
		output << "#操作(" << statStack.top() << "," << curr << "):";

		//接下来根据分析表内容进行操作
		switch (o->type)
		{
			//acc 接受,完成分析
		case 0: {
			output << "===收到结束符,完成分析" << endl;
			if (itt == size - 1)
			{
				output << "---结论:输入"<<(hasErr?"语法有错误,但是经过修正可以":"")<<"通过语法分析" << endl;
			}
			else
			{
				//output <<  << endl;
				output << "---结论:分析已成功完成,但是结束符之后有意外的标记" << endl;
			}
			return;
		}; break;

			// 移进
		case 1: {
			tokenStack.push(curr);
			statStack.push(o->operate);
			output << "移进标志: " << curr << "; 移进状态: " << o->operate << endl << endl;
		}break;
			
		case 2: {
			// 规约
			string rule = _grammar_prodList->at(o->operate);
			output << "按照规则 " << rule<< "(产生式" << o->operate << ")进行规约" << endl;
			
			//获取产生式的切分
			vector<string>& prod = *_grammar_getProd(grammar, o->operate);

			string left = rule.substr(0, rule.find("->"));
			//弹栈
			for (int p = prod.size() - 1; p >= 0; --p)
			{
				// 是否需要确认？
				statStack.pop();
				tokenStack.pop();
			}

			//查询goto表,然后加入
			int newStat = action_stat_input__act->at(statStack.top())->at(left)->operate;

			statStack.push(newStat);
			tokenStack.push(left);
			output << "----将产生的非终结符与状态" << newStat << "入栈" << endl << endl;
			// 注意,现在还不能移进,所以itt要减去1
			itt--;
		}break;

			//类型3:错误:忽略标记
		case 3: {
			output << "!!错误:" << Errmsg->at(o->operate)<<endl
				<<"......尝试跳过该符号." << endl << endl;

			hasErr = true;
		}break;

		default:
		{
			output << "出现了不咳能出现的错误:未知操作类型" <<o->type<< endl;
			return;
		}break;
		}

	}
}
