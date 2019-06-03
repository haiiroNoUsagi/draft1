// ͼ��
// ���ڷ���first follow���໥��ӹ�ϵ
// ʹ�ö�ά�����ʾͼ,����ͼ�ķ�����:�ⲿ->�ڲ�
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
	//���нڵ�
	unordered_map<string,Node*>* nodes;

	//���ķ���
	unordered_map<Node*,unordered_set<Node*>*>* loops;

	void check(Node * target, unordered_set<Node*>& checked, stack<Node*>& route);

	// findloop,Ѱ�һ�,��ѡ��һ������
	void findLoop();

	// analyzeLoop,����,���ڲ�Ԫ�ص�in��out�����е�����.
	void analyzeLoop();

	// asyncLoop,ͬ����������Ԫ��
	void asyncLoop();

	//���Ӽ���.ע��,follow����first������Ӳ�������û�пմ���,���Ҫ���Կմ�!
	void addSet(Node* target,Node* source);

	// ��ɰ�˳����ӵĶ���
	void addAll();
public:
	Graph();
	~Graph();
	
	//��ӽڵ�,�Լ�Ŀ�꼯�ϵ�����
	void addNode(string node, unordered_set<string>* set);
	//�������,Ҳ���ǹ�ϵ
	void addVector(string target,string source);

	//������Ӳ���
	void exec();
};

