#pragma once
#include<string>
#include<unordered_set>
using namespace std;
class Node
{
public:
	unordered_set<Node*>* out;
	unordered_set<Node*>* in;
	string value;

	//ע��,set���ⲿ��,������
	unordered_set<string>* set;

	~Node();

};
