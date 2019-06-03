#include "LRParser_2.h"



unordered_set<string>* LRParser_2::closure(unordered_set<string>& Items)
{
	//�ȼ�����ж��Ƿ��Ѵ���;

	unordered_set<string>* res = new unordered_set<string>();
	// ����1: ��I�е�������Ŀ����res
	for (unordered_set<string>::iterator it = Items.begin(); it != Items.end(); ++it)
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
		vector<pair<string, int>> prod2Add;

		for (unordered_set<string>::iterator it = res->begin(); it != res->end(); ++it)
		{
			// ��ȡ���ǵ���Ŀ
			Item& prod = *map_item2Item->at(*it);

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
					if (
						_prod.substr(0, _prod.find("->")) == expect 
						&& res->count(
							_prod.substr(0, _prod.find("->") + 2) + "��" + 
							// �˴�ע����Կմ�,����A->~�ӵ��ΪA->dot
							(_prod.substr(_prod.find("->") + 2) == "~"?"": (_prod.substr(_prod.find("->") + 2)))
						) == 0)
					{
						// ��ӵ���ʱ�б�
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
	
	// ��ɶ�res�ļ���,����res ����һ����ָ��
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
	// ��ӳ��Ѱ�Һ���goto�Ƿ��Ѿ�����
	if (map_gotoFunc_In->count(to_string(In) + input) != 0)
	{
		int index = map_gotoFunc_In->at((to_string(In) + input));
		return ItemSets->at(index);
	}


	//���ȴ�������,�鿴In���Ƿ��п�����ӵ�ʽ��
	unordered_set<Item*> Next;

	unordered_set<string>* target = ItemSets->at(In);

	//����In
	for (unordered_set<string>::iterator it0 = target->begin(); it0 != target->end(); ++it0)
	{
		Item& I = *map_item2Item->at(*it0);
		// �����symbol���������ָԪ����ͬ,��ô��NextI���
		if (I.point < I.prod_NOT_DELETE->size() && I.prod_NOT_DELETE->at(I.point) == input)
		{
			Next.insert(&I);
		}
	}
	
	if (Next.size() > 0)
	{
		// �Ƚ����ǵĵ������һλ
		unordered_set<string> NextI;
		for (unordered_set<Item*>::iterator it = Next.begin(); it != Next.end(); ++it)
		{
			// �����ǵĵ�����ƶ�
			NextI.insert(getOneItem((*it)->raw,*(*it)->prod_NOT_DELETE,(*it)->point+1));
		}
		// ֱ�ӷ������ıհ�,��Ϊclosure���Է��ؾɵ�In 
		unordered_set<string>* temp = closure(NextI);
		int index = checkExist_In_SISF(*temp);
		map_gotoFunc_In->insert(pair<string, int>(to_string(In) + input, index == -1 ? ItemSets->size() - 1 : index));
		return temp;
	}
	else
	{
		// û��������һ������,���ؿ�
		return NULL;
	}
}

int LRParser_2::checkExist_In_SISF(unordered_set<string>& NewItemSet)
{
	int size = ItemSets->size();

	// ���ڱ������е�In,�����Ƿ��д��ڵ�
	for (int it0 = 0; it0 < size; ++it0)
	{
		unordered_set<string>& In = *ItemSets->at(it0);
		// ���ȴ�СҪ��ͬ
		// ���,Ԫ��Ҫ��ͬ

		if (NewItemSet.size() == In.size())
		{
			bool found = true;
			for (unordered_set<string>::iterator it1 = In.begin(); it1 != In.end(); ++it1)
			{
				// ���ڲ���ͬԪ��
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
	// �����µ���Ŀ
	string item = raw.substr(0,raw.find("->")+2);
	int len = rawProd.size();
	int i = 0;
	while (i < point)
	{
		item += rawProd[i++];
	}
	item += "��";
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
		item += "��";
		while (i < len)
		{
			item += rawProd[i++];
		}
	}
	else
	{
		item += "��";
	}
*/


	// ��������,�����ӳ��
	if (map_item2Item->count(item) == 0)
	{
		Item* iexpr = new Item();
		iexpr->raw = raw;
		//iexpr->point = NOTemptyStr ? point : 1 ;
		iexpr->point = point ;
		iexpr->prod_NOT_DELETE = &rawProd;

		// ���ӳ��
		map_item2Item->insert(pair<string, Item*>(item, iexpr));
	}

	// �������ɵ��ַ���
	// �������,����ִ�һ�����ҵ�һ��Item��ʵ��
	return item;
}

void LRParser_2::LR0_build(unordered_set<string>& TokenList, int In)
{
	int size = TokenList.size();
	// ����֮ǰ,������һ����ֵ�б������

	// �������:����ÿһ�����ŵ�goto���
	for (unordered_set<string>::iterator it0 = TokenList.begin(); it0 != TokenList.end(); ++it0)
	{
		// ����ǰ,��¼LRI��ֵ
		int before = ItemSets->size();
		unordered_set<string>* res = _goto(In, *it0);
		//�������µ�In
		if (res != NULL && ItemSets->size() > before)
		{
			//���µ�In���еݹ�ļ��(-1)
			LR0_build(TokenList, ItemSets->size() - 1);
		}
	}
}

void LRParser_2::parser_build()
{
	

	//��һ��ѭ��:������Ŀ��
	int size = ItemSets->size();
	for (int it_n = 0; it_n < size; ++it_n)
	{
		unordered_map<string, Operation*>* newAction = new unordered_map<string, Operation*>();
		unordered_map<string, Operation*>* newGoto = new unordered_map<string, Operation*>();
		//it_n����״̬.Ϊÿһ��״̬��������
		action_stat_input__act -> insert(pair<int, unordered_map<string, Operation*>*>(it_n,newAction));

		//��ȡ��Ŀ��In
		unordered_set<string>& In = *ItemSets->at(it_n);
		//������Ŀ��
		for (unordered_set<string>::iterator In_iter = In.begin(); In_iter != In.end(); ++In_iter)
		{
			// ��ȡ��Ҫ��������Ŀ
			string raw = *In_iter;
			Item& I = *map_item2Item->at(raw);

			// �����ﴴ���µ���Ŀ
			Operation* no = new Operation();
			
			// ������мǺ�
			if (I.point < I.prod_NOT_DELETE->size())
			{
				string behind_point = I.prod_NOT_DELETE->at(I.point);

				int g = map_gotoFunc_In->at(to_string(it_n) + behind_point);
				newAction->insert(pair<string, Operation*>(behind_point, no));

				no->operate = g;
				
				// 2.c ���Ϊ���ս��
				if (behind_point[0] >= 'A'&&behind_point[0] <= 'W')
				{
					// ��ʱ��Ѱ��goto
					// goto(Ii,a) = Ij => goto [i,A] = j
					no->type = 999;
				}
				// 2.a ���Ϊ�ս��
				else{
					// �� action[it_n,a] Ϊsg �� �ƽ�����״̬g��ջ
					// goto(Ii,a) = Ij => action[i,a] = sj
					no->type = 1;
				}
			}
			//���Ѿ���ʽ�ӵ�ĩβ
			else// if (I.point == I.prod_NOT_DELETE->size())
			{
				//2.c
				if (raw.substr(0, raw.find("->")) == "S'")
				{
					no->type = 0;
					// ���ս�����$
					newAction->insert(pair<string,Operation*>("$",no));
				}
				else
				{
					// ��ȡfollow����,���������е�ÿһ���ս��a ,����action[it_n,a]Ϊrj
					// j,�Ǹ���Ŀԭ����ʽ�б�ı��,��raw.substr(0,raw.find("��"));

					unordered_set<string>& follow_I = *_grammar_getFollow(grammar, raw.substr(0, raw.find("->")));

					int j = 0;
					// ��ռ��λ,���-2
					string rp = raw.substr(0, raw.length() - 2);
					// ����մ�
					//if (rp.length() == raw.find("��"))
					//{
					//	rp += '~';
					//}
					while (_grammar_prodList->at(j) != rp)
					{
						j++;
						/*if (j >= _grammar_prodList->size())
						{
							throw exception("�Ҳ���follow���ж�Ӧ�Ĳ���ʽ");
						}*/
					}
					//������ʽ��Լ
					
					for (unordered_set<string>::iterator it_follow = follow_I.begin(); it_follow != follow_I.end(); ++it_follow)
					{
						no->type = 2;
						no->operate = j;
						// �˴�ע���Լ-��Լ��ͻ.
						if (newAction->count(*it_follow) != 0)
						{
							throw exception("�����˹�Լ-��Լ��ͻ! �ⲻ��SLR(1)�ķ�. ����ʧ��!");
							exit(-1);
						}
						// һ��operate��Ӧ�����string��Ӧ,�����ڻ���
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

	map_item2Item = new unordered_map<string, Item*>();			//ĳ��Ŀ�ַ�������Ŀ���ӳ��

	map_gotoFunc_In = new unordered_map<string, int>();			//goto�����Ľ����ǰ������Ŀ��

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

	// ���ȴ�����һ����Ŀ��I0
	unordered_set<string>* newI = new unordered_set<string>();
	newI->insert("S'->��" + temphead);
	// ��չ�ķ���ӳ��,�Լ���Ӧ���ʵ��
	Item* iexpr = new Item();
	iexpr->raw = "S'->" + temphead;
	iexpr->point = 0;
	extendProd_Vector = new vector<string>();
	iexpr->prod_NOT_DELETE = extendProd_Vector;
	iexpr->prod_NOT_DELETE->push_back(temphead);

	// ��ӵ�����Ŀ��ӳ��
	map_item2Item->insert(pair<string, Item*>("S'->��" + temphead, iexpr));

	// ��ȡ���ıհ�,���践��,��Ϊ������Ӽ���
	closure(*newI);


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
		vector<string>& right = *_grammar_getProd(grammar, i);
		for (int i1 = right.size() - 1; i1 >= 0; --i1)
		{
			token_list.insert(right[i1]);
		}
	}

	LR0_build(token_list, 0);

	// ִ�����֮��,��������Ŀ���淶��.���ڹ���SLR������
	parser_build();

	//�����������,����true
	enabled = true;
	return true;
}

void LRParser_2::add_ignorableErr(string symbol, string comment)
{
	// ��ѯ����������,�ҵ������շ��ŵ�λ��
	for (unordered_map<int, unordered_map<string, Operation*>*>::iterator it = action_stat_input__act->begin(); it != action_stat_input__act->end(); ++it)
	{
		if (it->second->count(symbol) == 0)
		{
			// ������Ӵ�����Operation
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
		output << "����ʹ��δ��ʼ���ķ�����,������init()" << endl;
		return;
	}
		

	output.flush();

	//��������ջ
	stack<string> tokenStack;
	stack<int> statStack;
	statStack.push(0);
	int size = tokens.size();
	bool hasErr = false;

	//���ѭ��:��ȡ��һ�����
	for (int itt = 0; itt < size; ++itt)
	{
		// ����ÿ�����,��ѯ������!
		// string curr = tokens[itt].second;

		// �˴��ж�����
		string curr = tokens[itt].second == "value"?"id": tokens[itt].first;

		unordered_map<string, Operation*>* check = action_stat_input__act->at(statStack.top());
		
		//���޷��ҵ�,���ᱨ��.
		if (check->count(curr) == 0)
		{
			// �������ǱȽ������,�����ǰ�յ��˽�����,��ô�ܿ���ȱ��ĳЩ����
			if (curr == "$")
			{
				// �ڵ�ǰ״̬����Ѱ�Һ��ʵķ��Ž��в���
				output << "!!����:����س����˽�����." << endl;
				hasErr = true;
				bool found = false;
				for (unordered_map<string, Operation*>::iterator iend = check->begin(); iend != check->end(); ++iend)
				{
					// ��ǰ���ֵĽ�����һ���ǹ�Լδ���,���Ѱ��һ�������ƽ�,�����ƽ�֮����Թ�Լ��ѡ��
					if (iend->second->type == 1)
					{
						int nextStat = iend->second->operate;
						// �ƽ�֮����Ը���$��Լ
						if (action_stat_input__act->at(nextStat)->count("$")!=0&&action_stat_input__act->at(nextStat)->at("$")->type == 2)
						{
							output << "??????���Բ������: " << iend->first << "  ��������..." << endl;

							// �������޸�curr
							curr = iend->first;

							// Ȼ��itt-1,ʹ֮����ָ��$
							itt--;
							found = true;
							break;
						}
					}
				}

				if (!found)
				{
					output <<"�޷��������ȱ�ٵķ��Ž��н�һ���ķ���.����:���벻ͨ���﷨����"<< endl << "===��������" << endl;
					return;
				}
			}
			else
			{
				output << "error:��������,���޷��ҵ���Ŀ:err(" << statStack.top() << "," << curr << "):δ֪��������"
					<< endl << "����:���벻ͨ���﷨����" << endl;
				output << endl << "===��������" << endl;
				return;
			}

		}

		Operation* o = check->at(curr);
		output << "#����(" << statStack.top() << "," << curr << "):";

		//���������ݷ��������ݽ��в���
		switch (o->type)
		{
			//acc ����,��ɷ���
		case 0: {
			output << "===�յ�������,��ɷ���" << endl;
			if (itt == size - 1)
			{
				output << "---����:����"<<(hasErr?"�﷨�д���,���Ǿ�����������":"")<<"ͨ���﷨����" << endl;
			}
			else
			{
				//output <<  << endl;
				output << "---����:�����ѳɹ����,���ǽ�����֮��������ı��" << endl;
			}
			return;
		}; break;

			// �ƽ�
		case 1: {
			tokenStack.push(curr);
			statStack.push(o->operate);
			output << "�ƽ���־: " << curr << "; �ƽ�״̬: " << o->operate << endl << endl;
		}break;
			
		case 2: {
			// ��Լ
			string rule = _grammar_prodList->at(o->operate);
			output << "���չ��� " << rule<< "(����ʽ" << o->operate << ")���й�Լ" << endl;
			
			//��ȡ����ʽ���з�
			vector<string>& prod = *_grammar_getProd(grammar, o->operate);

			string left = rule.substr(0, rule.find("->"));
			//��ջ
			for (int p = prod.size() - 1; p >= 0; --p)
			{
				// �Ƿ���Ҫȷ�ϣ�
				statStack.pop();
				tokenStack.pop();
			}

			//��ѯgoto��,Ȼ�����
			int newStat = action_stat_input__act->at(statStack.top())->at(left)->operate;

			statStack.push(newStat);
			tokenStack.push(left);
			output << "----�������ķ��ս����״̬" << newStat << "��ջ" << endl << endl;
			// ע��,���ڻ������ƽ�,����ittҪ��ȥ1
			itt--;
		}break;

			//����3:����:���Ա��
		case 3: {
			output << "!!����:" << Errmsg->at(o->operate)<<endl
				<<"......���������÷���." << endl << endl;

			hasErr = true;
		}break;

		default:
		{
			output << "�����˲����ܳ��ֵĴ���:δ֪��������" <<o->type<< endl;
			return;
		}break;
		}

	}
}
