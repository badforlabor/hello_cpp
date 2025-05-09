/**
 * Auth :   liubo
 * Date :   2025-05-09 09:47
 * Comment: C++参数包展开，std::index_sequence用法等
 * 
    std::index_sequence：C++14 引入的工具，用于生成编译时整数序列。
    参数包展开：C++11 引入的模板特性，允许将参数包展开为多个独立元素。
 */


#include <iostream>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <type_traits>

using namespace std;
#include "log.h"

#include "AutoRegTestFunc.h"
#include "test_inline.h"

namespace test_parameter_pack
{
    template<class T>
    string to_string(T a)
    {
        return std::to_string(a);
    }
    string to_string(const std::string& a)
    {
        return a;
    }
    // string to_string(size_t a)
    // {
    //     return std::to_string((int)a);
    // }
    template<class T>
    string to_string(const std::vector<T>& a)
    {
        std::string ret;
        ret += "(";
        for(size_t i=0; i<a.size(); i++)
        {
            if(i > 0)
            {
                ret += ",";
            }
            ret += to_string(a[i]);            
        }
        ret += ")";
        return ret;
    }
    void func1(int a, float b, const std::string& c)
    {
        
    }
    void func2(const std::vector<int>& a, const std::vector<float>& b)
    {
        
    }

    // 收集“有多少个参数”
    struct CollectArgsCount
    {
        template<typename ...Args>
        static void SaveArgs(std::string& ArgsDesc)
        {
            SaveArgsImpl<Args...>(ArgsDesc, std::index_sequence_for<Args...>{});
        }
        
        template<typename ...Args, size_t... IDX>
        static void SaveArgsImpl(std::string& ArgsDesc, std::index_sequence<IDX...>)
        {
            // 这里就是“参数包展开”，必须加", ..."（少逗号，就有编译错误）
            (SaveOneArgs<Args>(ArgsDesc, IDX), ...);
        }
        
        template<typename T>
        static void SaveOneArgs(std::string& ArgsDesc, size_t idx)
        {
            if(idx > 0)
            {
                ArgsDesc += ",";
            }
            ArgsDesc += to_string(idx);
        }        
    };

    // 收集参数内容

    struct CollectArgsContent
    {
        template<typename ...Args>
        static void SaveArgs(std::string& ArgsDesc, const std::tuple<Args...>& tupleValue)
        {
            SaveArgsImpl(ArgsDesc, tupleValue, std::index_sequence_for<Args...>{});
        }

        template<typename ...Args, size_t... IDX>
        static void SaveArgsImpl(std::string& ArgsDesc, const std::tuple<Args...>& tupleValue, std::index_sequence<IDX...>)
        {
            // 这个也叫“折叠表达式”
            // 这里就是“参数包展开”，必须加", ..."（少逗号，就有编译错误）
            (SaveOneArgs<Args>(ArgsDesc, std::get<IDX>(tupleValue), IDX), ...);
        }
        
        template<typename T>
        static void SaveOneArgs(std::string& ArgsDesc, const T& t, size_t idx)
        {
            if(idx > 0)
            {
                ArgsDesc += ",";
            }
            ArgsDesc += to_string(t);
        } 
    };
    
    template<auto func>
    struct FunctionBind;

    // 模板的参数，只要传func即可。其他的自动推导。如果传RET, ARG..，反而会出现编译错误！
    template<typename RET, typename... ARG, RET(*func)(ARG...)>
    struct FunctionBind<func>
    {        
        static void Invoke(std::string& ArgsDesc, ARG&&... args)
        {
            // 收集参数到ArgsOutput中
            CollectArgsCount::SaveArgs<ARG...>(ArgsDesc);
            ArgsDesc += ";";
            
            // 把参数收集到tuple中
            auto ArgsInTuple = std::make_tuple(args...);
            CollectArgsContent::SaveArgs<ARG...>(ArgsDesc, ArgsInTuple);            
            
            func(std::forward<ARG>(args)...);
        }
    };

    
    static void Test1()
    {
        {         
            std::string ArgsDesc;
            FunctionBind<&func1>::Invoke(ArgsDesc, 1, 2, "3");
            std::cout << ArgsDesc << std::endl;   
        }
        
        {         
            std::string ArgsDesc;
            std::vector<int> args1;
            args1.push_back(1);args1.push_back(2);args1.push_back(3);
            std::vector<float> args2;
            args2.push_back(3);args2.push_back(4);args2.push_back(5);
            FunctionBind<&func2>::Invoke(ArgsDesc, args1, args2);
            std::cout << ArgsDesc << std::endl;   
        }
    }
    
    static AutoRegTestFunc autoTestParametarPack(Test1);
}




