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
#include <assert.h>

#include "AutoRegTestFunc.h"


namespace test_other
{
	struct CTestProp
	{
		int A;
		float B;
		std::string C;
		char D : 1;
		char E : 1;
		char F : 1;
		char G;

	};

	static void Test1()
	{
		int OffsetA = offsetof(CTestProp, A);
		int OffsetB = offsetof(CTestProp, B);
		int OffsetC = offsetof(CTestProp, C);
		//int OffsetD = offsetof(CTestProp, D);
		//int OffsetE = offsetof(CTestProp, E);
		//int OffsetF = offsetof(CTestProp, F);
		int OffsetG = offsetof(CTestProp, G);

		std::cout << OffsetA << std::endl;
		std::cout << OffsetB << std::endl;
		std::cout << OffsetC << std::endl;
		//std::cout << OffsetD << std::endl;
		//std::cout << OffsetE << std::endl;
		//std::cout << OffsetF << std::endl;
		std::cout << OffsetG << std::endl;
	}

	static void Test2()
	{
		// 成员变量的地址，就是offset。（地址，得取相对地址，所以得用0，作为基准！）
		int OffsetA = (int) & (((CTestProp*)0)->A);
		int OffsetB = (int)&(((CTestProp*)0)->B);
		int OffsetC = (int)&(((CTestProp*)0)->C);
		//int OffsetD = offsetof(CTestProp, D);
		//int OffsetE = offsetof(CTestProp, E);
		//int OffsetF = offsetof(CTestProp, F);
		int OffsetG = (int)&(((CTestProp*)0)->G);

		assert(OffsetB == offsetof(CTestProp, B));
		assert(OffsetC == offsetof(CTestProp, C));
		assert(OffsetG == offsetof(CTestProp, G));

		std::cout << OffsetA << std::endl;
		std::cout << OffsetB << std::endl;
		std::cout << OffsetC << std::endl;
		//std::cout << OffsetD << std::endl;
		//std::cout << OffsetE << std::endl;
		//std::cout << OffsetF << std::endl;
		std::cout << OffsetG << std::endl;
	}


	struct FLLMActiveTagsScope
	{
		FLLMActiveTagsScope(int a, float b, std::string c)			
		{
			A = a;
			B = b;
			C = c;
		}
		int A; float B; std::string C;
	};
	
	static void Test3()
	{
		FLLMActiveTagsScope scope1 = {1, 2.0, "3"};
		// FLLMActiveTagsScope scope2 = {};
	}
	
	static AutoRegTestFunc autoTest2(Test2);
	static AutoRegTestFunc autoTest3(Test3);
}
