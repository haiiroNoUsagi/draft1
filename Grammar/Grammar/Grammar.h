/*
文法类:生成一个文法.它解析文法描述文件,生成易于处理的文法对象
	注意文法中的空串用~代替
	文法支持'号
*/

#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<fstream>

#include"Graph.h"

#ifdef GRAMMAR_DEV
#define DLL_TYPE dllexport
#else
#define DLL_TYPE dllimport
#endif

using namespace std;
class  Grammar
{
private:
	//辅助量
	unordered_set<string>* x;

	bool enabled;
	
	// 对应关系:1产生式左部->1产生式列表->n产生式右部
	unordered_map<
		string,		//产生式左部
		vector<					//产生式右部列表
			vector<string>*		//产生式右部
								//注意,右部最后一位是raw
		>*
	>* Product_List;


	// 生成FIRST\FOLLOW集合
	unordered_map<string, unordered_set<string>*>* FIRSTS;
	unordered_map<string, unordered_set<string>*>* FOLLOWS;

	void initFirstFollow();
	void initFirst();
	void initFollow();

	string startID;
public:
	Grammar();
	~Grammar();

	// 调用此方法,生成该文法
	bool analyzeGrammarFile(string filePath);
	
	// 返回左部对应的产生式个数
	int getListCount(string left);

	// 返回左部对应产生式的元素个数(-1)
	int getItemsCount(string left, int index);

	// 返回某一左部元素
	string getProductItem(string left, int index1, int index2);

	// 返回规则对应的原式
	string getRawProduct(string left,int index);

	// 返回FIRST集与FOLLOW集
	const unordered_set<string>& FIRST(string left);
	const unordered_set<string>& FOLLOW(string left);
};

