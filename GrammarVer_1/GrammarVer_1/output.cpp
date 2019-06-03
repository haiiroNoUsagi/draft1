/* 
文法类 version1
	目标:
	 1. 脱离头文件的引入,只导出几个标准库的类型的 "引用"
	 2. 实现First follow函数的调用
	 3. 自动清理

	 结论:想要避免引入头文件,不能够返回类内非静态成员的函数指针
	 因为它们都有一个隐含的变量:this
	 函数指针只能使用类外的.


导出函数模板:
extern "C" _declspec(DLL_TYPE) [返回类型] [函数名称] (void* instance,[其它参数])
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try
	{
		if (grammar.isEnabled())
		{
			return grammar.[函数]
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
v1.0缺陷: 
	所有产生式左部非终结符只能出现一次,否则无法正常生成文法
	必须使用竖线隔开

	 导出函数列表:
extern "C" _declspec(dllimport) void* _Grammar_getInstance(string filePath);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProductions(void* instance);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdItems(void* instance, int index);
extern "C" _declspec(dllimport) vector<string>* _Grammar_getProdLeft(void* instance, int index);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFirst(void* instance,string expr);
extern "C" _declspec(dllimport) unordered_set<string>* _Grammar_getFollow(void* instance,string expr);
*/

#ifdef GRAMMAR_DEV
#define DLL_TYPE dllexport
#else
#define DLL_TYPE dllimport
#endif


#include"Grammar.h"

// 获取文法的实例.
// 参数:文件路径
// 返回值: 成功返回Grammar实例; 失败返回NULL;
extern "C" _declspec(DLL_TYPE) void* _Grammar_getInstance(string filePath)
{
	Grammar* res = new Grammar();
	res->init(filePath);
	if (res->isEnabled())
	{
		return res;
	}
	else
	{
		delete res;
		return NULL;
	}
}

// 获取文法产生式列表
// 参数:实例
// 返回值:vector<string>* 无需删除,原则:谁创建谁删除
extern "C" _declspec(DLL_TYPE)vector<string>* _Grammar_getProductions(void* instance)
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try
	{
		if (grammar.isEnabled())
		{
			return grammar.getProductions();
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}

// 获取索引条目的产生式右部
// 参数:实例,索引
// 返回值:vector<string>*
extern "C" _declspec(DLL_TYPE)vector<string>* _Grammar_getProdItems(void* instance, int index)
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try
	{
		if (grammar.isEnabled())
		{
			if (index < 0 || index >= grammar.getProductions()->size())
			{
				return NULL;
			}
			return grammar.getProdItems(index);
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}

// 获取索引条目的产生式左部
// 参数:实例,索引
// 返回值:string
extern "C" _declspec(DLL_TYPE)string _Grammar_getProdLeft(void* instance, int index)
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try
	{
		if (grammar.isEnabled())
		{
			if (index < 0 || index >= grammar.getProductions()->size())
			{
				return NULL;
			}
			return grammar.getProdLeft(index);
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}

// 获取该文法的某First集
// 参数:文法实例(任意指针),First函数的参数
// 返回值:First集的指针
// 如果参数是非法指针,返回NULL

extern "C" _declspec(DLL_TYPE)unordered_set<string>* _Grammar_getFirst(void* instance,string expr)
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try 
	{
		if (grammar.isEnabled())
		{
			return grammar.getFirst(expr);
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}

// 获取该文法的某Follow集
// 参数:文法实例(任意指针),Follow函数的参数
// 返回值:Follow集的指针
// 如果参数是非法指针,返回NULL

extern "C" _declspec(DLL_TYPE) unordered_set<string>* _Grammar_getFollow(void* instance, string expr)
{
	Grammar& grammar = *(Grammar*)instance;
	// 异常处理以确定指针是否合法
	try
	{
		if (grammar.isEnabled())
		{
			return grammar.getFollow(expr);
		}
		else
		{
			return NULL;
		}
	}
	catch (exception& e)
	{
		return NULL;
	}
}



//
//int main()
//{
//	return 0;
//}