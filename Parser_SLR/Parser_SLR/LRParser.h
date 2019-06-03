/*
LRParser类: LR分析器类
通过这个类构造一个SLR分析器
*/
#pragma once

#include<tuple>
#include<vector>
#include<string>
#include<iostream>
#include<unordered_set>
#include<unordered_map>
using namespace std;

//typedef string(*pf_string__pvoid)(void*);
typedef vector<string>*(*pf_pvectorSTR__pvoid)(void*);
typedef vector<string>*(*pf_pvectorSTR__pvoid_int)(void*,int);
typedef vector<string>*(*pf_pvectorSTR__pvoid_string)(void*,string);
typedef unordered_set<string>*(*pf_pusetSTR__pvoid_string)(void*, string);

// 定义一个结构体.
// 意义:项目
struct Item_1
{
	string raw;								//原产生式,不带点的
	vector<string>* prod_NOT_DELETE;		//从文法中获取产生式右部的引用,不能删除!
	int point;								//点的位置,处于prod->at(point)的左边
};

// 定义一个结构体,记录每个符号的详细信息
struct Token_Property
{
	string raw_token;			// 该符号的原式
	string comment;				// 描述符号的类别
};

class LRParser
{
private:
	/*--状态标志--*/
	bool enabled;

	/*--外部引用变量--*/
	void* grammar;
	vector<string>* _grammar_prodList;					//产生式列表
	pf_pvectorSTR__pvoid_int _grammar_getProd;			//输入索引获取产生式右部
	pf_pusetSTR__pvoid_string _grammar_getFirst;		//First函数
	pf_pusetSTR__pvoid_string _grammar_getFollow;		//Follow函数

	/*--私有变量--*/
	string extendProd;									//拓展文法的起始符
	vector<string>* extendProd_Vector;					//拓展文法的向量

	unordered_map<string, Item_1*>* I_map;				//原项目到项目结构体之间的映射

	unordered_map<int,unordered_set<string>*>* closures;	//项目集In对应的闭包
	unordered_map<string, unordered_set<string>*>* gotos;	//goto函数运行过的列表,注意,这是将数字拼在了串之前.

	vector<unordered_set<string>*>* itemSet_Family;			// 该文法的LR(0)项目集规范族,vector的元素,即为In
	//unordered_map<string, I_expr*>* LRI_map;			// 上述集合是原式,此处是映射
	//unordered_map<string, int>* LRI_set_Index;			// 为规范族添加索引

	

	/*--私有方法--*/
		
	/*重用代码*/
	void new_I(string prod, vector<string>& right, unordered_set<string>& target_set);			//新的项目
	void next_I(string lastProd, Item_1& lastI, unordered_set<string>& target_set);				//已有项目,点往后跳
	// 输入:一个项目族的指针
	// 输出:若不存在相同项目族,返回NULL,否则返回该项目族的指针
	unordered_set<string>* checkExist(unordered_set<string>* NewItem);											//检查生成的项目族是否已存在


	//闭包函数,求某一项目集的闭包
	//参数:I(n)? 输入n.
	//返回值:项目闭包的集合(原式)
	//在私有变量定义了原式到项目结构体的映射
	//特别注意,构造转换关系的时候,首先要保证In存在
	//所以,要先创建In,再将规则1所需的产生式加入In,最后运行closure
	unordered_set<string>* closure(int In);							//重载:计算项目集In的闭包
	//unordered_set<string>* closure(int unordered_set<string>& I);	//重载:任意项目集的闭包

	// 转移函数,原理同书上
	// 参数:I(n)?输入n. ,下一个符号
	unordered_set<string>* _goto(int In, string input);

	// LR0_build函数(递归)
	// 无返回值和参数
	// 使用已写好的方法,生成项目族.注意,首次到达In时,创建新的I
	// (可能会出现环?)

	void LR0_build(unordered_set<string>&LeftList, int In);

	// 分析记号流,并生成相应的Token数组
	void GetTokens(vector<pair<string,string>>& input);

public:
	LRParser();
	~LRParser();

	bool isEnabled();
	
	// init函数:初始化LR分析器
	// 参数:文法的实例, 以及这个实例的以下函数指针:
	//		获取产生式列表
	//		获取指定索引位置的产生式
	//		该文法的First函数
	//		该文法的Follow函数
	bool init(void* grammar,
		pf_pvectorSTR__pvoid function_getAllProd,
		pf_pvectorSTR__pvoid_int _grammar_getProd,
		pf_pusetSTR__pvoid_string _grammar_getFirst,
		pf_pusetSTR__pvoid_string _grammar_getFollow);

	// parse函数:开始分析
	// 参数:输出流,输入记号流的引用(vector<string>)
	// 输入:记号流
	// 输出:向输出流输出语法分析所用的产生式,以及最后的分析结果或错误信息
	void parse(vector<string>& tokens,ostream& output);
};

