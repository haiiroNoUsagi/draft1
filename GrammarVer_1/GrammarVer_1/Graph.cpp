#include "Graph.h"

void Graph::check(Node* target,unordered_set<Node*>&checked, stack<Node*>& route)
{
	unordered_set<Node*>& outs = *(target->out);
	//路径进栈
	route.push(target);
	for (unordered_set<Node*>::iterator it = outs.begin(); it != outs.end(); ++it)
	{
		Node* out = *it;

		//检查路径
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
			//发现环,则将之后的点全部加入loop
			//但是并不破坏已有结构
			else
			{
				hasLoop = true;
				//如果两个环相连,则这两个环可看做同一环
				//因此如果已存在的环中存在这些点,加入即可
				unordered_set<Node*>* loop = new unordered_set<Node*>();

				//bool 查看是否有环相连
				bool linked = false;
				while (!temp.empty())
				{
					Node* item = temp.top();
					route.push(temp.top());

					temp.pop();

					if (!linked)
					{
						//检查已存在的环
						for (unordered_map<Node*, unordered_set<Node*>*>::iterator it2 = loops->begin(); it2 != loops->end(); ++it2)
						{
							unordered_set<Node*>* existedLoop = it2->second;
							//已存在
							if (existedLoop->count(item) != 0)
							{
								linked = true;
								//将loop中已存在的全部添加到已存在的环中
								for (unordered_set<Node*>::iterator it3 = loop->begin(); it3 != loop->end(); ++it3)
								{
									existedLoop->insert(*it3);
								}
								delete loop;
								loop = existedLoop;
							}
						}
					}

					//添加到loop(不论是新的还是旧的)
					loop->insert(item);
					
				}

				//如果是新的,就创建一个
				if (!linked)
				{
					loops->insert(pair<Node*, unordered_set<Node*>*>(out,loop));
				}
			}

		}

		// 未发现环,则继续查找
		if (!hasLoop)
		{
			check(out, checked, route);
		}
	}
	//路径出栈
	route.pop();
	//所有路径都检查完毕,才会加入checked集合中
	checked.insert(target);
}

void Graph::findLoop()
{
	//外层循环:遍历每个节点
	//每次循环检查当前节点是否已经检查,如果被检查了,跳过
	unordered_set<Node*> checkedNodes;
	for (unordered_map<string, Node*>::iterator it0 = nodes->begin(); it0 != nodes->end(); ++it0)
	{
		Node* currNode = it0->second;
		// 如果尚未被检查,就从这个节点出发,获取它的每一条路线
		// 使用两个栈,一个代表它的路线,一个代表当前的
		if (checkedNodes.count(currNode) == 0)
		{
			stack<Node*> route;
			//开始遍历每一条路线
			check(currNode,checkedNodes,route);
		}
	}
}

void Graph::analyzeLoop()
{
	//处理目标:
	/*
	除了标志元素,其余元素的in与out的大小都应为0
	标志元素in与out都是从外部来,指向外部的
	*/
	//外层循环:遍历每个组
	for(unordered_map<Node*, unordered_set<Node*>*>::iterator it0 = loops->begin();it0!=loops->end();++it0)
	{
		Node* symbol = it0->first;
		unordered_set<Node*>* loop = it0->second;
		//循环:将他们的in和out全部加入标志中
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
					//it2:是指输入node的节点
					//也要将对面的关系指向symbol
					(*it2)->out->erase(node);

					(*it2)->out->insert(symbol);
					symbol->in->insert(*it2);
				}
			}
			for (unordered_set<Node*>::iterator it2 = out->begin(); it2 != out->end(); ++it2)
			{
				if (loop->count(*it2) == 0)
				{
					//也要将对面的关系指向symbol
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
		//获取loop
		unordered_set<Node*>* loop = it->second;
		//辅助量:已确认的string
		unordered_set<string> checkedId;

		//外层循环0: 遍历loop,从他们的集合中寻找uncheckedId,然后再次遍历loop
		for (unordered_set<Node*>::iterator it0 = loop->begin(); it0 != loop->end(); ++it0)
		{
			//内层循环1
			for (unordered_set<string>::iterator it1 = (*it0)->set->begin(); it1 != (*it0)->set->end(); ++it1)
			{
				//未确认
				if (checkedId.count(*it1) == 0)
				{
					checkedId.insert(*it1);

					//内层循环2:向所有loop添加it1
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
	//添加前和添加后,都要同步一次Loop
	asyncLoop();

	//外层循环:寻找入度为0的组织,直到没有组织入度为0为止

	//已完成的节点
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
				//找到入度为0的了
				found = true;

				//向它的出发地传送自己的集合
				//然后,向他的目标移除对自身的引用
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


	//最后处理:同步环的内容
	asyncLoop();
}

Graph::Graph()
{
	//所有节点
	nodes = new unordered_map<string, Node*>();

	//环的分组
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
