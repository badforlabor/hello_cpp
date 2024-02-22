/**
 * Auth :   liubo
 * Date :
 * Comment: 调用纯虚函数，会崩溃
 *		不要在构造函数中，调用虚函数，否则很危险！！！
 */

#include <iostream>
#include <sstream>

#include "AutoRegTestFunc.h"

// 测试方法，把这里的宏打开
#define DO_TEST_CALL_VIRTUAL_FUNCTION 0

namespace test_pure_virtual
{
	class CDerived;
	class CBase
	{
	public:
		CBase(CDerived* derived);
		~CBase();
		virtual void function(void) = 0;

		CDerived* m_pDerived;
	};

	class CDerived : public CBase
	{
	public:
		CDerived() : CBase(this) {};   // C4355
		virtual void function(void) {};
	};

	CBase::CBase(CDerived* derived) : m_pDerived(derived) 
	{
		printf("CBase\n");
		m_pDerived->function();
	}
	CBase::~CBase()
	{
		printf("~CBase\n");
		m_pDerived->function();
	}

	void myPurecallHandler(void)
	{
		printf("In _purecall_handler.\n");
		exit(0);
	}

	static void Test1() 
	{
		_set_purecall_handler(myPurecallHandler);
		CDerived myDerived;
	}

#if DO_TEST_CALL_VIRTUAL_FUNCTION
	static AutoRegTestFunc autoTest1(Test1);
#endif
}