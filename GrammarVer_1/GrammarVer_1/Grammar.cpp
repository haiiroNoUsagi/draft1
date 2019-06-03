#include "Grammar.h"



void Grammar::initFirst()
{
	Graph g;			// 生成集合需要用到有向图作为辅助工具



	/*
	第一轮:先判断条件1和条件2,以确定哪些非终结符拥有空串
	*/

	// 辅助全局变量:有空串的左部集合.因为follow也用到了...
	empty_in_First = new unordered_set<string>();
	unordered_set<string>& FIRST_with_emp = *empty_in_First;

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		string left = it->first;


		unordered_set<string> *first = new unordered_set<string>();
		vector<vector<string>*>* list = it->second;
		FIRSTS->insert(pair<string, unordered_set<string>*>(left, first));

		//在添加表项的同时,也要添加图中的记录
		g.addNode(left, first);

		// 寻找列表中以终结符开头或空串的产生式
		for (int i = 0; i < list->size(); ++i)
		{
			string str = (*list)[i]->at(0);
			if (str == "~")
			{
				first->insert("~");
				FIRST_with_emp.insert(left);
			}
			else if (!(str[0] >= 'A'&&str[0] <= 'W'))
			{
				first->insert(str);
			}
		}
	}

	/*
	第二轮:
	1.生成一个列表,计算添加非终结符之间的关系
	2.判断各个非终结符之间的关系.若有相互加入FIRST的现象,将它们分为一组.
	3.判断到最后,检查规则4,确定是否加入空串
	*/

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		string left = it->first;
		vector<vector<string>*>* list = it->second;
		unordered_set<string>* firsts = FIRSTS->at(left);
		// 内层的局部变量,用于生成已添加关系
		unordered_set<string> rights;


		for (int i = 0; i < list->size(); ++i)
		{
			int j;
			int jsize = list->at(i)->size();

			for (j = 0; j < jsize; ++j)
			{
				string str = list->at(i)->at(j);
				if (str[0] >= 'A'&&str[0] <= 'W')
				{
					//找到非终结符,将它的FIRST加入,并在已添加关系中记录
					rights.insert(str);

					//有加入的关系时,向图中添加边
					g.addVector(left, str);

					unordered_set<string>* temp = FIRSTS->at(str);
					for (unordered_set<string>::iterator itemp = temp->begin(); itemp != temp->end(); ++itemp)
					{
						firsts->insert(*itemp);
					}
					// 如果它的first集中有空串,就可以检查下一位.
					if (FIRST_with_emp.count(str) == 0)
					{
						break;
					}
				}
				else
				{
					//找到终结符,
					//xxxxxxxxxxxxxx不能直接加入;直接退出
					//FIRSTS->at(left)->insert(str);
					break;
				}
			}

			//全都有空串,则根据规则4特例,将空串加入
			if (j == jsize)
			{
				firsts->insert("~");
			}
		}

	}

	//最后运行图的处理函数,获得集合
	g.exec();
}

void Grammar::initFollow()
{
	Graph g;


	unordered_set<string>& FIRST_with_emp = *empty_in_First;

	//辅助变量:加入follow
	unordered_map<string, string> right_add2_left;

	//第一轮循环 
	//1.首先创建各自的对象.由于规则1,将$置于起始符.
	//2.根据规则2,遍历所有左部,并添加FIRST部分
	//3.对于需要添加左部follow的规则3,在这一轮循环先记录,存在上面的辅助变量中

	for (unordered_map<string, vector<vector<string>*>*>::iterator it_0_0 = Product_List->begin(); it_0_0 != Product_List->end(); ++it_0_0)
	{
		string left = it_0_0->first;
		unordered_set<string> *follow = new unordered_set<string>();

		g.addNode(left, follow);

		if (left == startID)
		{
			follow->insert("$");
		}
		FOLLOWS->insert(pair<string, unordered_set<string>*>(left, follow));

		//在所有产生式右部寻找x
		//注意,可能有ABA形式的,因此寻找必须找到底.
		for (unordered_map<string, vector<vector<string>*>*>::iterator it_1_0 = Product_List->begin(); it_1_0 != Product_List->end(); ++it_1_0)
		{
			string left_1 = it_1_0->first;
			vector<vector<string>*>* list = it_1_0->second;
			for (int it2 = 0; it2 < list->size(); ++it2)
			{
				vector<string>* prod = list->at(it2);

				//flag,看看是否找到
				bool found = false;

				for (int it3 = 0; it3 < prod->size(); ++it3)
				{
					string curr = prod->at(it3);
					if (found)
					{
						// 如果是终结符,就添加到follow,并found置0.否则检查FIRST集
						if (!(curr[0] >= 'A'&&curr[0] <= 'W'))
						{
							follow->insert(curr);
							found = false;
						}
						else
						{
							//找FIRST集,并添加
							unordered_set<string>* curr_first = FIRSTS->at(curr);
							for (unordered_set<string>::iterator it_4_0 = curr_first->begin(); it_4_0 != curr_first->end(); ++it_4_0)
							{
								// 注意忽略空串
								if ((*it_4_0 != "~"))
								{
									follow->insert(*it_4_0);
								}

							}

							//如果有空串,则继续寻找下一位
							if (FIRST_with_emp.count(curr) == 0)
							{
								found = false;
							}
						}
					}
					else
					{
						// 找到了,下一个就开始检查
						if (left == curr)
						{
							found = true;
						}
					}
				}
				//不论如何,只要找到了,就满足条件3,因此添加记录
				if (found)
				{
					right_add2_left.insert(pair<string, string>(left, left_1));

				}
			}
		}
	}

	//根据上面获得的添加关系,向图中添加边
	for (unordered_map<string, string>::iterator it = right_add2_left.begin(); it != right_add2_left.end(); ++it)
	{
		g.addVector(it->first, it->second);
	}


	// 运行图,获得follow集合
	g.exec();
}

unordered_set<string>* Grammar::getFirstOfExpr(string expr)
{
	if (temp_sets_first->count(expr) != 0)
	{
		return temp_sets_first->at(expr);
	}

	int len = expr.length();
	unordered_set<string>* res = new unordered_set<string>();
	temp_sets_first->insert(pair<string,unordered_set<string>*>(expr,res));
	int i = 0;
	for (; i < len; ++i)
	{
		char curr = expr[i];

		//遇到了空串,这是在搞事?
		if (curr == '~')
		{
			break;
		}
		//遇到了终结符,它的first就是它本身,并且可以直接返回了
		else if (!(curr>='A'&&curr<='W'))
		{
			string sb = "";
			sb += curr;
			res->insert(curr=='i'?"id":sb);
			break;
		}
		//遇到非终结符,将它的FIRST加入到res中
		else
		{
			string temp="";
			temp += curr;
			if (i < len - 1)
			{
				if (expr[i + 1] == '\'')
				{
					temp += '\'';
					i++;
				}
			}
			bool hasE = false;
			for (unordered_set<string>::iterator it = FIRSTS->at(temp)->begin(); it != FIRSTS->at(temp)->end(); ++it)
			{
				if (*it != "~")
				{
					res->insert(*it);
				}
				else
				{
					hasE = true;
				}
			}

			// 如果没空,就到此为止了
			if (!hasE)
			{
				break;
			}
		}
	}

	// 所有的都有空串,则把空串也加入res
	if (i == len)
	{
		res->insert("~");
	}
	return res;
}


Grammar::Grammar()
{
	enabled = false;
	empty_in_First = new unordered_set<string>();
	temp_sets_first = new unordered_map<string,unordered_set<string>*>();
	Product_List = new unordered_map<string, vector<vector<string>*>*>();
	index = new unordered_map<int, pair<string, int>>();
	rawList = new vector<string>();
	FIRSTS = new unordered_map<string, unordered_set<string>*>();
	FOLLOWS = new unordered_map<string, unordered_set<string>*>();
}


Grammar::~Grammar()
{
	delete empty_in_First;

	for (unordered_map<string, unordered_set<string>*>::iterator it = temp_sets_first->begin(); it != temp_sets_first->end(); ++it)
	{
		delete it->second;
	}
	delete temp_sets_first;

	delete rawList;

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		vector<vector<string>*>* item = it->second;
		for (int i = 0; i < item->size(); ++i)
		{
			delete item->at(i);
		}
		delete item;
	}
	delete Product_List;

	delete index;

	for (unordered_map<string, unordered_set<string>*>::iterator it = FIRSTS->begin(); it != FIRSTS->end(); ++it)
	{
		delete it->second;
	}
	delete FIRSTS;

	for (unordered_map<string, unordered_set<string>*>::iterator it = FOLLOWS->begin(); it != FOLLOWS->end(); ++it)
	{
		delete it->second;
	}
	delete FOLLOWS;
}

bool Grammar::init(string filePath)
{
	if (enabled)
		return false;
	ifstream ifs(filePath, ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	// 开始生成文法
	int count = 0;
	string last = "";
	vector<vector<string>*>* right = NULL;
	while (ifs.good())
	{
		char buffer[64];
		ifs.getline(buffer, 64);
		string str = buffer;

		// 切分语句,采用自动机的方法
		int start;
		start = str.find('-');

		string left = str.substr(0, start);
		string raw = left + "->";
	

		if (rawList->size()==0)
		{
			startID = left;
		}

		if (last == left)
		{
			count += 1;
		}
		else
		{	// 意味着这是一个新的产生式左部,需要新的righT 否则就不需要,在原有的基础上添加即可
			count = 0;
			right = new vector<vector<string>*>();
			Product_List->insert(pair<string, vector<vector<string>*>*>(left, right));
		}
		last = left;

		
		vector<string>* product_right = new vector<string>();
		start += 2;
		int len = str.length();
		while (start < len)
		{
			char cur = str[start];
			if (cur >= 'A'&&cur <= 'Z')
			{
				if (start == len - 1 || str[start + 1] != '\'')
				{
					product_right->push_back(str.substr(start, 1));
					raw += cur;
				}
				else
				{
					product_right->push_back(str.substr(start, 2));
					raw += str.substr(start, 2);
					start++;
				}
			}
			else if (cur == '|')
			{
				// 在index添加它的记录

				index->insert(pair<int, pair<string, int>>(rawList->size(), pair<string,int>(left, count)));
				count++;

				// 完成一个产生式的解析后,将切分后的产生式添加到列表中.
				rawList->push_back(raw);
				right->push_back(product_right);

				product_right = new vector<string>();

				raw = left + "->";
			}
			else if (cur == 'i')
			{
				product_right->push_back("id");
				raw += "id";
				start++;
			}
			else
			{
				product_right->push_back(str.substr(start, 1));
				raw += cur;
			}

			start++;

		}

		// 完成一个产生式的解析后,在index添加它的记录
		index->insert(pair<int, pair<string, int>>(rawList->size(), pair<string, int>(left, count)));
		
		// 然后将切分后的产生式添加到列表中.
		rawList->push_back(raw);
		right->push_back(product_right);

	}

	ifs.close();
	enabled = true;

	// 在生成结束后,初始化First集和Follow集
	initFirst();
	initFollow();

	return true;
}

bool Grammar::isEnabled()
{
	return enabled;
}

vector<string>* Grammar::getProductions()
{
	return rawList;
}

string Grammar::getProdLeft(int _index)
{
	return this->index->at(_index).first;
}

vector<string>* Grammar::getProdItems(int _index)
{
	string left = index->at(_index).first;
	int num = index->at(_index).second;
	return Product_List->at(left)->at(num);
}

unordered_set<string>* Grammar::getFirst(string input)
{
	// 若找不到,可能输入了一个表达式
	if (FIRSTS->count(input) == 0)
	{
		return getFirstOfExpr(input);
	}

	return FIRSTS->at(input);
}

unordered_set<string>* Grammar::getFollow(string input)
{
	if (FOLLOWS->count(input) == 0)
	{
		return NULL;
	}

	return FOLLOWS->at(input);
}

