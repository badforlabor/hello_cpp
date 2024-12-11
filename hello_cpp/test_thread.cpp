/**
 * Auth :   liubo
 * Date :   2024-04-02 10:31
 * Comment: 多线程相关
 *  https://zhuanlan.zhihu.com/p/613630658
 *  
 */


#include <assert.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <windows.h>
#include <ppl.h>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <shared_mutex>
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
    static double fibonacci(double number)
    {
        if (number <= 1)
        {
            return number;
        }
        return fibonacci(number - 1) + fibonacci(number - 2);
    }
    static void Test2()
    {
        std::vector<double> number;
        std::set<DWORD> threadList;
        std::vector<double> arr;
        for(int i=0; i<1000; i++)
        {
            arr.push_back(15);
        }
        std::shared_mutex mutex_;
        
        concurrency::parallel_for(0, (int)arr.size()-1, [&](int n) {
            fibonacci(arr[n]);
            std::unique_lock<std::shared_mutex> lock(mutex_);
            threadList.insert(GetCurrentThreadId());
        }, concurrency::auto_partitioner());
        std::cout << threadList.size() << std::endl;
    }
    
    static AutoRegTestFunc autoTest1(Test1);
    static AutoRegTestFunc autoTest2(Test2);
}
