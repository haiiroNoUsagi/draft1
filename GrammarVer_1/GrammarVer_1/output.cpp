/* 
�ķ��� version1
	Ŀ��:
	 1. ����ͷ�ļ�������,ֻ����������׼������͵� "����"
	 2. ʵ��First follow�����ĵ���
	 3. �Զ�����

	 ����:��Ҫ��������ͷ�ļ�,���ܹ��������ڷǾ�̬��Ա�ĺ���ָ��
	 ��Ϊ���Ƕ���һ�������ı���:this
	 ����ָ��ֻ��ʹ�������.


��������ģ��:
extern "C" _declspec(DLL_TYPE) [��������] [��������] (void* instance,[��������])
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
	try
	{
		if (grammar.isEnabled())
		{
			return grammar.[����]
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
v1.0ȱ��: 
	���в���ʽ�󲿷��ս��ֻ�ܳ���һ��,�����޷����������ķ�
	����ʹ�����߸���

	 ���������б�:
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

// ��ȡ�ķ���ʵ��.
// ����:�ļ�·��
// ����ֵ: �ɹ�����Grammarʵ��; ʧ�ܷ���NULL;
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

// ��ȡ�ķ�����ʽ�б�
// ����:ʵ��
// ����ֵ:vector<string>* ����ɾ��,ԭ��:˭����˭ɾ��
extern "C" _declspec(DLL_TYPE)vector<string>* _Grammar_getProductions(void* instance)
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
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

// ��ȡ������Ŀ�Ĳ���ʽ�Ҳ�
// ����:ʵ��,����
// ����ֵ:vector<string>*
extern "C" _declspec(DLL_TYPE)vector<string>* _Grammar_getProdItems(void* instance, int index)
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
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

// ��ȡ������Ŀ�Ĳ���ʽ��
// ����:ʵ��,����
// ����ֵ:string
extern "C" _declspec(DLL_TYPE)string _Grammar_getProdLeft(void* instance, int index)
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
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

// ��ȡ���ķ���ĳFirst��
// ����:�ķ�ʵ��(����ָ��),First�����Ĳ���
// ����ֵ:First����ָ��
// ��������ǷǷ�ָ��,����NULL

extern "C" _declspec(DLL_TYPE)unordered_set<string>* _Grammar_getFirst(void* instance,string expr)
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
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

// ��ȡ���ķ���ĳFollow��
// ����:�ķ�ʵ��(����ָ��),Follow�����Ĳ���
// ����ֵ:Follow����ָ��
// ��������ǷǷ�ָ��,����NULL

extern "C" _declspec(DLL_TYPE) unordered_set<string>* _Grammar_getFollow(void* instance, string expr)
{
	Grammar& grammar = *(Grammar*)instance;
	// �쳣������ȷ��ָ���Ƿ�Ϸ�
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