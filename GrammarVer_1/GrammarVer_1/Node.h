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

	//注意,set是外部的,不析构
	unordered_set<string>* set;

	~Node();

};
