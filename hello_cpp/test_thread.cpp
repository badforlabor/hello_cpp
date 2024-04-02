/**
 * Auth :   liubo
 * Date :   2024-04-02 10:31
 * Comment: 多线程相关
 */


#include <assert.h>
#include <iostream>
#include <sstream>
#include <thread>

#include "AutoRegTestFunc.h"


namespace test_thread
{

    static void Func1(std::atomic<int>* Counter)
    {
        for(int i=0; i<10; i++)
        {
            std::cout << "func1:" << i << std::endl;
        }
        Counter->fetch_add(1);  // 原子+1
    }
    static void Func2(std::atomic<int>* Counter)
    {
        for(int i=0; i<10; i++)
        {
            std::cout << "func2:" << i << std::endl;
        }
        Counter->fetch_add(1);
    }
    
    static void Test1()
    {
        std::atomic<int> WorkDone = 0;
        std::thread t1(Func1, &WorkDone);
        t1.detach();    // 后台运行
        std::thread t2(Func2, &WorkDone);
        t2.detach();    // 后台运行

        // 等待2个线程都结束
        while (WorkDone.load() != 2)
        {
            std::chrono::milliseconds t(2);
            std::this_thread::sleep_for(t);
        }
        assert(WorkDone.load() == 2);
    }
    
    static AutoRegTestFunc autoTest1(Test1);
}
