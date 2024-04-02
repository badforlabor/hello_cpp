/**
 * Auth :   liubo
 * Date :
 * Comment: 调用纯虚函数，会崩溃
 *		不要在构造函数中，调用虚函数，否则很危险！！！
 */

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

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
		virtual ~CBase();
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


namespace test_pure_virtual2
{
	class CBasePure
	{
	public:
		virtual ~CBasePure()
		{
		
		}
		virtual void CallPureFunc() = 0;
		virtual void CallPureFunc2() = 0;
		void CallInside()
		{
			CallPureFunc2();
		}
	};
	class ClassA : public CBasePure
	{
	public:
		virtual ~ClassA() {}
		virtual void CallPureFunc() override
		{
			// delete this;
			CallInside();
			// std::chrono::milliseconds t(0);
			// std::this_thread::sleep_for(t);
			// std::cout << "ClassA" << std::endl;
		}
		virtual void CallPureFunc2() override
		{
			std::chrono::milliseconds t(0);
			std::this_thread::sleep_for(t);
			std::cout << "ClassA" << std::endl;
		}
	};
	
	static void Write(std::vector<CBasePure*>* Db, std::atomic<int>* Counter)
	{
		for(int i=0; i<Db->size(); i++)
		{			
			std::chrono::milliseconds t(5);
			std::this_thread::sleep_for(t);
			(*Db)[i] = new ClassA();
		}
        Counter->fetch_add(1);
	}
	static void Read(std::vector<CBasePure*>* Db, std::atomic<int>* Counter)
	{
		for(int i=0; i<Db->size(); i++)
		{			
			std::chrono::milliseconds t(5);
			std::this_thread::sleep_for(t);
			
			CBasePure* Ptr = (*Db)[i];
			
			if(Ptr != nullptr)
			{
				// 模拟：另一个线程delete的指针，再调用虚函数，就会触发：纯虚函数被调用的bug了
				// 注意，在release上，就能重现。在debug模式，现象是崩溃
				// delete Ptr;
				Ptr->CallPureFunc();
			}
		}		
        Counter->fetch_add(1);
	}
	static void Delete(std::vector<CBasePure*>* Db, std::atomic<int>* Counter)
	{
		for(int i=0; i<Db->size(); i++)
		{			
			std::chrono::milliseconds t(5);
			std::this_thread::sleep_for(t);
			CBasePure* Ptr = (*Db)[i];
			if(Ptr != nullptr)
			{
				delete Ptr;
				// memset((void*)(Ptr+4), 0, sizeof(ClassA)-4);				
			}
		}	
        Counter->fetch_add(1);	
	}
	
	static void Test2() 
	{
		_set_purecall_handler(test_pure_virtual::myPurecallHandler);
		
        std::atomic<int> WorkDone = 0;
		std::vector<CBasePure*> DataBase;
		for(int i=0; i<100; i++)
		{
			DataBase.push_back(nullptr);
		}
		// 开启多线程
		std::thread WriteThread(Write, &DataBase, &WorkDone);
		std::thread ReadThread(Read, &DataBase, &WorkDone);
		std::thread DeleteThread(Delete, &DataBase, &WorkDone);
		WriteThread.detach();
		ReadThread.detach();
		DeleteThread.detach();
		
		while (WorkDone.load() != 3)
		{
			std::chrono::milliseconds t(2);
			std::this_thread::sleep_for(t);
		}  
	}
#if DO_TEST_CALL_VIRTUAL_FUNCTION
	static AutoRegTestFunc autoTest1(Test2);
#endif
}