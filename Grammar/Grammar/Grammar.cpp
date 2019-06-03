#include "Grammar.h"



void Grammar::initFirstFollow()
{
	// ���ȼ���FIRST��
	initFirst();
	initFollow();
}

void Grammar::initFirst()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����:Graph
	Graph g;



	/*
	��һ��:���ж�����1������2,��ȷ����Щ���ս��ӵ�пմ�
	*/

	// ����ȫ�ֱ���:�пմ����󲿼���.��ΪfollowҲ�õ���...
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

	//�����ֲ�����:�ѳ�������ӹ�ϵ
	//unordered_map<string, unordered_set<string>> right_add2_left;

	


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
			int jsize = list->at(i)->size()-1;

			// ע��˴�!!-1!!!!!
			for (j = 0; j < jsize; ++j)
			{
				string str = list->at(i)->at(j);
				if (str[0] >= 'A'&&str[0] <= 'W')
				{
					//�ҵ����ս��,������FIRST����,��������ӹ�ϵ�м�¼
					rights.insert(str);

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		// ���������ӹ�ϵ
		//right_add2_left.insert(pair<string, unordered_set<string>>(left, rights));

	}


	/*
	���մ���:
	1.�ڵڶ�����,����Ϊһ��ķ��ս��,����Ӧ�໥��ȫȱ�ٵ��ս��.
	2.��ͨ�������б�2.1,���һ��.
	*/
	
	//////////////////////////////////
	g.exec();

	////���ڱ������е���ӹ�ϵ,�������һ�μ��.
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
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

				//�ٴ�ע�������-1
				for (int it3 = 0; it3 < prod->size()-1; ++it3)
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

	//////////////////////////////////////////////////////////////////////////////////////////
	for (unordered_map<string, string>::iterator it = right_add2_left.begin(); it != right_add2_left.end(); ++it)
	{
		g.addVector(it->first, it->second);
	}


	// �Ѿ�����˲�ͬfollow֮��ļ����ϵ,����Ӧ��������
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

	// ��ʼ�����ķ�
	while (ifs.good())
	{

		char buffer[64];
		ifs.getline(buffer, 64);
		string str = buffer;

		// �з����,�����Զ����ķ���
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

	// �����ɽ�����,����˽�з�������FIRST/FOLLOW
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

