// 图类
// 用于分析first follow的相互添加关系
// 使用二维数组表示图,有向图的方向是:外部->内部
#pragma once
#include<stack>
#include<vector>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include"Node.h"
using namespace std;



class Graph
{
	//所有节点
	unordered_map<string,Node*>* nodes;

	//环的分组
	unordered_map<Node*,unordered_set<Node*>*>* loops;

	void check(Node * target, unordered_set<Node*>& checked, stack<Node*>& route);

	// findloop,寻找环,并选出一个代表
	void findLoop();

	// analyzeLoop,处理环,将内部元素的in和out都集中到代表.
	void analyzeLoop();

	// asyncLoop,同步环内所有元素
	void asyncLoop();

	//增加集合.注意,follow集和first集的相加操作都是没有空串的,因此要忽略空串!
	void addSet(Node* target,Node* source);

	// 完成按顺序相加的动作
	void addAll();
public:
	Graph();
	~Graph();
	
	//添加节点,以及目标集合的引用
	void addNode(string node, unordered_set<string>* set);
	//添加向量,也就是关系
	void addVector(string target,string source);

	//运行相加操作
	void exec();
};

