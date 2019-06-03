#include "Graph.h"

void Graph::check(Node* target,unordered_set<Node*>&checked, stack<Node*>& route)
{
	unordered_set<Node*>& outs = *(target->out);
	//·����ջ
	route.push(target);
	for (unordered_set<Node*>::iterator it = outs.begin(); it != outs.end(); ++it)
	{
		Node* out = *it;

		//���·��
		stack<Node*> temp;
		while (!route.empty())
		{
			temp.push(route.top());
			route.pop();
		}

		bool hasLoop = false;
		while (!temp.empty())
		{
			if (out != temp.top())
			{
				route.push(temp.top());
				temp.pop();
			}
			//���ֻ�,��֮��ĵ�ȫ������loop
			//���ǲ����ƻ����нṹ
			else
			{
				hasLoop = true;
				//�������������,�����������ɿ���ͬһ��
				//�������Ѵ��ڵĻ��д�����Щ��,���뼴��
				unordered_set<Node*>* loop = new unordered_set<Node*>();

				//bool �鿴�Ƿ��л�����
				bool linked = false;
				while (!temp.empty())
				{
					Node* item = temp.top();
					route.push(temp.top());

					temp.pop();

					if (!linked)
					{
						//����Ѵ��ڵĻ�
						for (unordered_map<Node*, unordered_set<Node*>*>::iterator it2 = loops->begin(); it2 != loops->end(); ++it2)
						{
							unordered_set<Node*>* existedLoop = it2->second;
							//�Ѵ���
							if (existedLoop->count(item) != 0)
							{
								linked = true;
								//��loop���Ѵ��ڵ�ȫ����ӵ��Ѵ��ڵĻ���
								for (unordered_set<Node*>::iterator it3 = loop->begin(); it3 != loop->end(); ++it3)
								{
									existedLoop->insert(*it3);
								}
								delete loop;
								loop = existedLoop;
							}
						}
					}

					//��ӵ�loop(�������µĻ��Ǿɵ�)
					loop->insert(item);
					
				}

				//������µ�,�ʹ���һ��
				if (!linked)
				{
					loops->insert(pair<Node*, unordered_set<Node*>*>(out,loop));
				}
			}

		}

		// δ���ֻ�,���������
		if (!hasLoop)
		{
			check(out, checked, route);
		}
	}
	//·����ջ
	route.pop();
	//����·����������,�Ż����checked������
	checked.insert(target);
}

void Graph::findLoop()
{
	//���ѭ��:����ÿ���ڵ�
	//ÿ��ѭ����鵱ǰ�ڵ��Ƿ��Ѿ����,����������,����
	unordered_set<Node*> checkedNodes;
	for (unordered_map<string, Node*>::iterator it0 = nodes->begin(); it0 != nodes->end(); ++it0)
	{
		Node* currNode = it0->second;
		// �����δ�����,�ʹ�����ڵ����,��ȡ����ÿһ��·��
		// ʹ������ջ,һ����������·��,һ������ǰ��
		if (checkedNodes.count(currNode) == 0)
		{
			stack<Node*> route;
			//��ʼ����ÿһ��·��
			check(currNode,checkedNodes,route);
		}
	}
}

void Graph::analyzeLoop()
{
	//����Ŀ��:
	/*
	���˱�־Ԫ��,����Ԫ�ص�in��out�Ĵ�С��ӦΪ0
	��־Ԫ��in��out���Ǵ��ⲿ��,ָ���ⲿ��
	*/
	//���ѭ��:����ÿ����
	for(unordered_map<Node*, unordered_set<Node*>*>::iterator it0 = loops->begin();it0!=loops->end();++it0)
	{
		Node* symbol = it0->first;
		unordered_set<Node*>* loop = it0->second;
		//ѭ��:�����ǵ�in��outȫ�������־��
		for (unordered_set<Node*>::iterator it1 = loop->begin(); it1 != loop->end(); ++it1)
		{
			Node* node = (*it1);
			if (node == symbol)
			{
				unordered_set<Node*>* in = node->in;
				unordered_set<Node*>* out = node->out;
				unordered_set<Node*>* nin = new unordered_set<Node*>();
				unordered_set<Node*>* nout = new unordered_set<Node*>();
				for (unordered_set<Node*>::iterator it2 = in->begin(); it2 != in->end(); ++it2)
				{
					if (loop->count(*it2) == 0)
					{
						nin->insert(*it2);
					}
				}
				delete in;
				node->in = nin;

				for (unordered_set<Node*>::iterator it2 = out->begin(); it2 != out->end(); ++it2)
				{
					if (loop->count(*it2) == 0)
					{
						nout->insert(*it2);
					}
				}
				delete out;
				node->out = nout;
				continue;
			}
				

			unordered_set<Node*>* in = node->in;
			unordered_set<Node*>* out = node->out;
			for (unordered_set<Node*>::iterator it2 = in->begin(); it2 != in->end(); ++it2)
			{
				if (loop->count(*it2) == 0)
				{
					//it2:��ָ����node�Ľڵ�
					//ҲҪ������Ĺ�ϵָ��symbol
					(*it2)->out->erase(node);

					(*it2)->out->insert(symbol);
					symbol->in->insert(*it2);
				}
			}
			for (unordered_set<Node*>::iterator it2 = out->begin(); it2 != out->end(); ++it2)
			{
				if (loop->count(*it2) == 0)
				{
					//ҲҪ������Ĺ�ϵָ��symbol
					(*it2)->in->erase(node);

					(*it2)->in->insert(symbol);
					symbol->out->insert(*it2);
				}
			}

			in->clear();
			out->clear();
		}
	}
}

void Graph::asyncLoop()
{
	for (unordered_map<Node*, unordered_set<Node*>*>::iterator it = loops->begin(); it != loops->end(); ++it)
	{
		//��ȡloop
		unordered_set<Node*>* loop = it->second;
		//������:��ȷ�ϵ�string
		unordered_set<string> checkedId;

		//���ѭ��0: ����loop,�����ǵļ�����Ѱ��uncheckedId,Ȼ���ٴα���loop
		for (unordered_set<Node*>::iterator it0 = loop->begin(); it0 != loop->end(); ++it0)
		{
			//�ڲ�ѭ��1
			for (unordered_set<string>::iterator it1 = (*it0)->set->begin(); it1 != (*it0)->set->end(); ++it1)
			{
				//δȷ��
				if (checkedId.count(*it1) == 0)
				{
					checkedId.insert(*it1);

					//�ڲ�ѭ��2:������loop���it1
					for (unordered_set<Node*>::iterator it2 = loop->begin(); it2 != loop->end(); ++it2)
					{
						(*it2)->set->insert(*it1);
					}
				}
			}
		}
	}
}

void Graph::addSet(Node * target, Node * source)
{
	unordered_set<string>* t = target->set;
	unordered_set<string>* s = source->set;

	for (unordered_set<string>::iterator it = s->begin(); it != s->end(); ++it)
	{
		if ((*it) == "~")
			continue;
		t->insert(*it);
	}
}



void Graph::addAll()
{
	//���ǰ����Ӻ�,��Ҫͬ��һ��Loop
	asyncLoop();

	//���ѭ��:Ѱ�����Ϊ0����֯,ֱ��û����֯���Ϊ0Ϊֹ

	//����ɵĽڵ�
	unordered_set<Node*> finished;

	bool found = true;
	while (found)
	{
		found = false;
		for (unordered_map<string, Node*>::iterator it0 = nodes->begin(); it0 != nodes->end(); ++it0)
		{
			Node* node = it0->second;
			if (node->in->size() == 0&&finished.count(node)==0)
			{
				//�ҵ����Ϊ0����
				found = true;

				//�����ĳ����ش����Լ��ļ���
				//Ȼ��,������Ŀ���Ƴ������������
				unordered_set<Node*>* out = node->out;
				for (unordered_set<Node*>::iterator it1 = out->begin(); it1 != out->end(); ++it1)
				{
					addSet(*it1,node);
					(*it1)->in->erase(node);
				}

				finished.insert(node);
			}
		}
	}


	//�����:ͬ����������
	asyncLoop();
}

Graph::Graph()
{
	//���нڵ�
	nodes = new unordered_map<string, Node*>();

	//���ķ���
	loops = new unordered_map<Node*, unordered_set<Node*>*>();
}


Graph::~Graph()
{
	
	for (unordered_map<Node*, unordered_set<Node*>*>::iterator it = loops->begin(); it != loops->end(); ++it)
	{
		delete it->second;
	}
	delete loops;

	for (unordered_map<string, Node*>::iterator it = nodes->begin(); it != nodes->end(); ++it)
	{
		delete it->second;
	}
	delete nodes;
}

void Graph::addNode(string node, unordered_set<string>* set)
{
	if (nodes->count(node) != 0)
		return;
	Node* n = new Node();
	n->in = new unordered_set<Node*>();
	n->out = new unordered_set<Node*>();
	n->value = node;
	n->set = set;
	nodes->insert(pair<string, Node*>(node, n));
}

void Graph::addVector(string target, string source)
{
	if (target == source)
		return;
	

	Node* ntarget = nodes->at(target);
	Node* nsource = nodes->at(source);

	ntarget->in->insert(nsource);
	nsource->out->insert(ntarget);
}

void Graph::exec()
{
	findLoop();
	analyzeLoop();
	addAll();
}
