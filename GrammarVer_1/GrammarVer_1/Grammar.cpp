#include "Grammar.h"



void Grammar::initFirst()
{
	Graph g;			// ���ɼ�����Ҫ�õ�����ͼ��Ϊ��������



	/*
	��һ��:���ж�����1������2,��ȷ����Щ���ս��ӵ�пմ�
	*/

	// ����ȫ�ֱ���:�пմ����󲿼���.��ΪfollowҲ�õ���...
	empty_in_First = new unordered_set<string>();
	unordered_set<string>& FIRST_with_emp = *empty_in_First;

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		string left = it->first;


		unordered_set<string> *first = new unordered_set<string>();
		vector<vector<string>*>* list = it->second;
		FIRSTS->insert(pair<string, unordered_set<string>*>(left, first));

		//����ӱ����ͬʱ,ҲҪ���ͼ�еļ�¼
		g.addNode(left, first);

		// Ѱ���б������ս����ͷ��մ��Ĳ���ʽ
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
	�ڶ���:
	1.����һ���б�,������ӷ��ս��֮��Ĺ�ϵ
	2.�жϸ������ս��֮��Ĺ�ϵ.�����໥����FIRST������,�����Ƿ�Ϊһ��.
	3.�жϵ����,������4,ȷ���Ƿ����մ�
	*/

	for (unordered_map<string, vector<vector<string>*>*>::iterator it = Product_List->begin(); it != Product_List->end(); ++it)
	{
		string left = it->first;
		vector<vector<string>*>* list = it->second;
		unordered_set<string>* firsts = FIRSTS->at(left);
		// �ڲ�ľֲ�����,������������ӹ�ϵ
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
					//�ҵ����ս��,������FIRST����,��������ӹ�ϵ�м�¼
					rights.insert(str);

					//�м���Ĺ�ϵʱ,��ͼ����ӱ�
					g.addVector(left, str);

					unordered_set<string>* temp = FIRSTS->at(str);
					for (unordered_set<string>::iterator itemp = temp->begin(); itemp != temp->end(); ++itemp)
					{
						firsts->insert(*itemp);
					}
					// �������first�����пմ�,�Ϳ��Լ����һλ.
					if (FIRST_with_emp.count(str) == 0)
					{
						break;
					}
				}
				else
				{
					//�ҵ��ս��,
					//xxxxxxxxxxxxxx����ֱ�Ӽ���;ֱ���˳�
					//FIRSTS->at(left)->insert(str);
					break;
				}
			}

			//ȫ���пմ�,����ݹ���4����,���մ�����
			if (j == jsize)
			{
				firsts->insert("~");
			}
		}

	}

	//�������ͼ�Ĵ�����,��ü���
	g.exec();
}

void Grammar::initFollow()
{
	Graph g;


	unordered_set<string>& FIRST_with_emp = *empty_in_First;

	//��������:����follow
	unordered_map<string, string> right_add2_left;

	//��һ��ѭ�� 
	//1.���ȴ������ԵĶ���.���ڹ���1,��$������ʼ��.
	//2.���ݹ���2,����������,�����FIRST����
	//3.������Ҫ�����follow�Ĺ���3,����һ��ѭ���ȼ�¼,��������ĸ���������

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

		//�����в���ʽ�Ҳ�Ѱ��x
		//ע��,������ABA��ʽ��,���Ѱ�ұ����ҵ���.
		for (unordered_map<string, vector<vector<string>*>*>::iterator it_1_0 = Product_List->begin(); it_1_0 != Product_List->end(); ++it_1_0)
		{
			string left_1 = it_1_0->first;
			vector<vector<string>*>* list = it_1_0->second;
			for (int it2 = 0; it2 < list->size(); ++it2)
			{
				vector<string>* prod = list->at(it2);

				//flag,�����Ƿ��ҵ�
				bool found = false;

				for (int it3 = 0; it3 < prod->size(); ++it3)
				{
					string curr = prod->at(it3);
					if (found)
					{
						// ������ս��,����ӵ�follow,��found��0.������FIRST��
						if (!(curr[0] >= 'A'&&curr[0] <= 'W'))
						{
							follow->insert(curr);
							found = false;
						}
						else
						{
							//��FIRST��,�����
							unordered_set<string>* curr_first = FIRSTS->at(curr);
							for (unordered_set<string>::iterator it_4_0 = curr_first->begin(); it_4_0 != curr_first->end(); ++it_4_0)
							{
								// ע����Կմ�
								if ((*it_4_0 != "~"))
								{
									follow->insert(*it_4_0);
								}

							}

							//����пմ�,�����Ѱ����һλ
							if (FIRST_with_emp.count(curr) == 0)
							{
								found = false;
							}
						}
					}
					else
					{
						// �ҵ���,��һ���Ϳ�ʼ���
						if (left == curr)
						{
							found = true;
						}
					}
				}
				//�������,ֻҪ�ҵ���,����������3,�����Ӽ�¼
				if (found)
				{
					right_add2_left.insert(pair<string, string>(left, left_1));

				}
			}
		}
	}

	//���������õ���ӹ�ϵ,��ͼ����ӱ�
	for (unordered_map<string, string>::iterator it = right_add2_left.begin(); it != right_add2_left.end(); ++it)
	{
		g.addVector(it->first, it->second);
	}


	// ����ͼ,���follow����
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

		//�����˿մ�,�����ڸ���?
		if (curr == '~')
		{
			break;
		}
		//�������ս��,����first����������,���ҿ���ֱ�ӷ�����
		else if (!(curr>='A'&&curr<='W'))
		{
			string sb = "";
			sb += curr;
			res->insert(curr=='i'?"id":sb);
			break;
		}
		//�������ս��,������FIRST���뵽res��
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

			// ���û��,�͵���Ϊֹ��
			if (!hasE)
			{
				break;
			}
		}
	}

	// ���еĶ��пմ�,��ѿմ�Ҳ����res
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

	// ��ʼ�����ķ�
	int count = 0;
	string last = "";
	vector<vector<string>*>* right = NULL;
	while (ifs.good())
	{
		char buffer[64];
		ifs.getline(buffer, 64);
		string str = buffer;

		// �з����,�����Զ����ķ���
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
		{	// ��ζ������һ���µĲ���ʽ��,��Ҫ�µ�righT ����Ͳ���Ҫ,��ԭ�еĻ�������Ӽ���
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
				// ��index������ļ�¼

				index->insert(pair<int, pair<string, int>>(rawList->size(), pair<string,int>(left, count)));
				count++;

				// ���һ������ʽ�Ľ�����,���зֺ�Ĳ���ʽ��ӵ��б���.
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

		// ���һ������ʽ�Ľ�����,��index������ļ�¼
		index->insert(pair<int, pair<string, int>>(rawList->size(), pair<string, int>(left, count)));
		
		// Ȼ���зֺ�Ĳ���ʽ��ӵ��б���.
		rawList->push_back(raw);
		right->push_back(product_right);

	}

	ifs.close();
	enabled = true;

	// �����ɽ�����,��ʼ��First����Follow��
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
	// ���Ҳ���,����������һ�����ʽ
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

