#pragma once
/*
自上而下的预测分析
(注意修改项目运行库为/MD)
(文法要消除左递归)
	工作原理:
		1. 初始化
			初始化阶段,读取文法文件(注意,空串使用~代替)
			构造预测分析表.
		2. 接收输入串
			将会调用词法分析类,因此初始化的参数需要获得词法分析器的入口
			输入流进入词法分析器后,程序会阻塞,直到分析完成,返回记号流;
			此时,记号流将会通过回调函数回到分析器中
		3. 处理记号流
			根据要求,设定各类记号及其属性.
		4. 边分析边计算
*/

// 以下是终结符与非终结符的定义
// 为了便于计算,目前采用最简单的判断方法

#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include<unordered_set>
#include<unordered_map>
using namespace std;
// 集成了语法生成器与词法分析器
/*外部方法区*/
#pragma comment(lib,"Scanner.lib")
extern "C" __declspec(dllimport)int _lex_enable(FILE* input, FILE* output, void(*callback_func)(const char*, int));
extern "C" __declspec(dllimport)FILE* _lex_getInput();
#pragma comment(lib,"Grammar.lib")
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance, string expr);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance, string expr);

static vector<string>* tokens;

class Parser_ver1
{
private:
	vector<string>* products;										// 产生式
	vector<vector<string>*>* product_items;							// 产生式对应的右部
	unordered_map<string, unordered_map<string, int>*>* table;	// 分析表

	//unordered_set<string>* terminators;				// 终结符列表(用于判断)
	//unordered_set<string>* nonterminals;			// 非终结符列表

public:
	/*初始化部分*/
	Parser_ver1();
	Parser_ver1(const char* program_file);			// 使用文法文件初始化
	void initGrammar(const char* program_file);		// 手动初始化
	~Parser_ver1();

	/*获取状态*/
	int getStat();

	/*设定分析规则*/
	void addAnalyzeRule();

	/*程序运行*/
	void parse(const char* fileName, bool analyze, ostream& output);

};

