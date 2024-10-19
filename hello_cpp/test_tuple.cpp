/**
 * Auth :   liubo
 * Date :   2024-10-17 02:41 
 * Comment: 练习Tuple的一些写法
 */


#include <assert.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "AutoRegTestFunc.h"


namespace test_tuple
{
    class FNestedString
    {
    public:
        void Add()
        {
            
        }
        
        template<typename Type, typename ...Types>
        void Add(const Type& Str, const Types& ...Other)
        {
            NestedString.push_back(Str);
            Add(Other...);
        }
        
        template<typename Type, typename ...Types>
        void Add2(int n, const Type& Str, const Types& ...Other)
        {
            NestedString.push_back(Str);
            Add(Other...);
            Add(std::to_string(n));
        }
        template<typename Type, typename ...Types>
        void Add2(const Type& Str, const Types& ...Other, const std::string& n)
        {
            NestedString.push_back(Str);
            Add(Other...);
            Add(n);
        }
        
        std::string ToString()
        {
            std::stringstream ss;
            for (auto Str : NestedString)
            {
                if(!ss.str().empty())
                {
                    ss << ",";                    
                }
                ss << Str;
            }
            return ss.str();
        }
        
        static void Test1()
        {
            FNestedString a;
            a.Add("1", "2", "3");
            a.Add2(5, "4", "4-1");
            a.Add2("6", "7");

#define KG_TT(...) a.Add(__VA_ARGS__)
#define KG_TT2(n, ...) a.Add2(n, __VA_ARGS__)
            KG_TT("a", "b", "c");
            KG_TT2(5, "d", "e");
            
            std::cout << a.ToString() << std::endl;
        }
        
    private:
        std::vector<std::string> NestedString;
    };

    

    static AutoRegTestFunc autoTest1(FNestedString::Test1);
}
