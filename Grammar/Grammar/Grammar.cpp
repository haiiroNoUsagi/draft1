#include "Grammar.h"



void Grammar::initFirstFollow()
{
	// 首先计算FIRST集
	initFirst();
	initFollow();
}

void Grammar::initFirst()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////测试:Graph
	Graph g;



	/*
	第一轮:先判断条件1和条件2,以确定哪些非终结符拥有空串
	*/

	// 辅助全局变量:有空串的左部集合.因为follow也用到了...
	x = new unordered_set<string>();
	unordered_set<string>& FIRST_with_emp = *x;

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		string left = it->first;


		unordered_set<string> *first = new unordered_set<string>();
		vector<vector<string>*>* list = it->second;
		FIRSTS->insert(pair<string, unordered_set<string>*>(left, first));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	//辅助局部变量:已成立的添加关系
	//unordered_map<string, unordered_set<string>> right_add2_left;

	


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
			int jsize = list->at(i)->size()-1;

			// 注意此处!!-1!!!!!
			for (j = 0; j < jsize; ++j)
			{
				string str = list->at(i)->at(j);
				if (str[0] >= 'A'&&str[0] <= 'W')
				{
					//找到非终结符,将它的FIRST加入,并在已添加关系中记录
					rights.insert(str);

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		// 更新这个添加关系
		//right_add2_left.insert(pair<string, unordered_set<string>>(left, rights));

	}


	/*
	最终处理:
	1.在第二轮中,被分为一组的非终结符,它们应相互补全缺少的终结符.
	2.再通过处理列表2.1,检查一遍.
	*/
	
	//////////////////////////////////
	g.exec();

	////现在遍历所有的添加关系,进行最后一次检查.
	//for (unordered_map<string, unordered_set<string>>::iterator itr = right_add2_left.begin(); itr != right_add2_left.end(); ++itr)
	//{
	//	unordered_set<string>& target = itr->second;
	//	unordered_set<string>* first = FIRSTS->at(itr->first);
	//	for (unordered_set<string>::iterator itemp = target.begin(); itemp != target.end(); ++itemp)
	//	{
	//		unordered_set<string>* target_first = FIRSTS->at(*itemp);
	//		for (unordered_set<string>::iterator itemp2 = target_first->begin(); itemp2 != target_first->end(); ++itemp2)
	//		{
	//			first->insert(*itemp2);
	//		}
	//	}
	//}
}

void Grammar::initFollow()
{
	//////////////////////////////////////////
	Graph g;


	unordered_set<string>& FIRST_with_emp = *x;

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
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

				//再次注意这里的-1
				for (int it3 = 0; it3 < prod->size()-1; ++it3)
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

	//////////////////////////////////////////////////////////////////////////////////////////
	for (unordered_map<string, string>::iterator it = right_add2_left.begin(); it != right_add2_left.end(); ++it)
	{
		g.addVector(it->first, it->second);
	}


	// 已经获得了不同follow之间的加入关系,现在应该生成树
	/////////////////////////////////////////////////////////////////////////////
	g.exec();
}

Grammar::Grammar()
{
	Product_List = new unordered_map<string, vector<vector<string>*>*>();
	FIRSTS = new unordered_map<string, unordered_set<string>*>();
	FOLLOWS = new unordered_map<string, unordered_set<string>*>();

	startID = "";
}


Grammar::~Grammar()
{
	delete x;

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

bool Grammar::analyzeGrammarFile(string filePath)
{
	if (enabled)
		return false;
	ifstream ifs(filePath,ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	// 开始生成文法
	while (ifs.good())
	{

		char buffer[64];
		ifs.getline(buffer, 64);
		string str = buffer;

		// 切分语句,采用自动机的方法
		int start;
		start = str.find('-');
		
		string left = str.substr(0,start);
		string raw = left + "->";

		if (startID.length() == 0)
		{
			startID = left;
		}

		vector<vector<string>*>* right = new vector<vector<string>*>();
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
				product_right->push_back(raw);
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
		product_right->push_back(raw);
		right->push_back(product_right);

		Product_List->insert(pair<string, vector<vector<string>*>*>(left,right));
	}

	ifs.close();
	enabled = true;

	// 在生成结束后,调用私有方法生成FIRST/FOLLOW
	initFirstFollow();

	return true;
}

int Grammar::getListCount(string left)
{
	return Product_List->at(left)->size();
}

int Grammar::getItemsCount(string left, int index)
{
	return Product_List->at(left)->at(index)->size() - 1;
}


string Grammar::getProductItem(string left, int index1, int index2)
{
	return Product_List->at(left)->at(index1)->at(index2);
}

string Grammar::getRawProduct(string left, int index)
{
	vector<string>* res = Product_List->at(left)->at(index);
	return res->at(res->size()-1);
}

const unordered_set<string>& Grammar::FIRST(string left)
{
	return *FIRSTS->at(left);
}

const unordered_set<string>& Grammar::FOLLOW(string left)
{
	return *FOLLOWS->at(left);
}

