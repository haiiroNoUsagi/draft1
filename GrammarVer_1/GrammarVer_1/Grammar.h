/*
文法类:一个文法的实例
	
	初始化:需要一个文法文件的路径

	对外部提供了方法:
		名称				描述

		isEnabled()			判断文法是否成功生成

		getProductList()	获取该文法所有产生式(已切分,按顺序排列.)

		getFirst(string)	计算某符号或表达式的first集合
		getFollow(string)	同上,计算follow集合

	使用原则:文法会提供上述集合或列表的指针,但是不应该在外部销毁这些指针

*/
//#ifdef GRAMMAR_DEV
//#define DLL_TYPE dllexport
//#else
//#define DLL_TYPE dllimport
//#endif


#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<fstream>

#include"Graph.h"

using namespace std;

class Grammar
{
private:
	/*----- 私有变量 -----*/
	bool enabled;
	//产生式的主要结构
	unordered_map<
		string,				//产生式左部
		vector<				//产生式右部列表
		vector<string>*		//产生式右部
							//注意,右部最后一位是raw
		>*
	>* Product_List;

	unordered_map<int,pair<string,int>>* index;			//外部对象通过唯一的数字获取右部产生式,因此他们是一一对应的

	vector<string>* rawList;							//存储拆分的产生式列表
	
	unordered_set<string>* empty_in_First;				//记录哪些First集合中含有空串

	unordered_map<string, unordered_set<string>*>* temp_sets_first;	//查询过的表达式的集合,暂时存在于此
	//非终结符的两个集合
	unordered_map<string, unordered_set<string>*>* FIRSTS;
	unordered_map<string, unordered_set<string>*>* FOLLOWS;


	/*----- 私有方法 -----*/

	// 以下两个方法在init方法中调用,用于初始化非终结符的两个集合
	void initFirst();
	void initFollow();

	// 如果需要查找表达式的First或Follow集合,则要通过私有方法完成
	unordered_set<string>* getFirstOfExpr(string expr);
public:
	Grammar();
	~Grammar();

	string startID;					//文法的起始符

	//初始化文法
	bool init(string filePath);

	bool isEnabled();

	// 获取所有产生式列表(切分)
	vector<string>* getProductions();

	// 获取产生式对应的左部
	string getProdLeft(int index);
	// 获取产生式元素
	vector<string>* getProdItems(int index);

	// First Follow函数 
	// 在生成文法时,默认为每一个非终结符生成了它们的两个集
	// 若要计算表达式的集,调用私有方法
	unordered_set<string>* getFirst(string input);
	unordered_set<string>* getFollow(string input);
};

