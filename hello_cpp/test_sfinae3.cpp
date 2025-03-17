/**
 * Auth :   liubo
 * Date :   2025-03-17 09:47
 * Comment: SFINAE,测试const T, T, T&&等命中情况（左值、右值、左值引用、右值引用、const、指针）
 *      模板函数的重载选择规则是怎样的？（原则是？）
 *          C++的重载规则，根据C++的重载规则，非模板函数优先于模板函数
 *          C++的规则，当有多个可行的函数模板时，编译器会选择最特化的版本
 *          当参数是左值时，可能更倾向于选择T&或者T&&的版本
 *
 *     std提供的类型萃取函数：     
            is_const_v
            is_pointer_v
            is_reference_v
            is_rvalue_reference_v

            remove_reference_t
            remove_pointer_t
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

namespace test_sfinae3
{
#if true
    /* 主模板定义 */
    template <typename T> struct GetTType
    {
        static constexpr int value = 0;
    };

    /* 各类型特化版本 */
    template <typename T> struct GetTType<const T>
    {
        static constexpr int value = 1;
    };  // const T
    template <typename T> struct GetTType<T&>
    {
        static constexpr int value = 2;
    };  // T&
    template <typename T> struct GetTType<const T&>
    {
        static constexpr int value = 3;
    };  // const T&
    template <typename T> struct GetTType<T&&>
    {
        static constexpr int value = 4;
    };  // T&&
    template <typename T> struct GetTType<T*>
    {
        static constexpr int value = 5;
    };  // T*
    template <typename T> struct GetTType<const T*>
    {
        static constexpr int value = 6;
    };  // const T*


    
    template <typename T>
    void check_type2(T* param) {
        cout << "TypeCode1:" << GetTType<T*>::value << std::endl;
    }
    template <typename T>
    void check_type2(const T* param) {
        cout << "TypeCode2:" << GetTType<const T*>::value << std::endl;
    }

    // 提醒！！！用这种形式，参数“T& param”无法区分出是“int&”, int这两种情况，因为都是左值
    template <typename T>
    void check_type2(T& param) {
        cout << "TypeCode3:" << GetTType<T&>::value << std::endl;
    }
    template <typename T>
    void check_type2(const T& param) {
        cout << "TypeCode4:" << GetTType<const T&>::value << std::endl;
    }
    
    /* 验证函数模板 */
    template <typename T>
    void check_type2(T&& param) {
        // std::cout << "Type code: " << GetTType<
        //     std::remove_reference_t<          // 移除引用
        //     std::remove_const_t<              // 移除const
        //     std::remove_pointer_t<            // 移除指针
        //     std::decay_t<T>                   // 退化类型
        // >>>::value << std::endl;
        // cout << "TypeCode:" << GetTType<std::remove_reference_t<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>>::value << std::endl;
        // cout << "TypeCode:" << GetTType<T>::value << std::endl;
        // cout << "TypeCode:" << GetTType<std::remove_pointer_t<T>>::value << std::endl;        
        cout << "TypeCode:" << GetTType<T&&>::value << std::endl;   
    }
    
    static void Test1()
    { 
        int a = 10;
        const int b = 20;
        int& c = a;
        const int& d = b;
        int* e = &a;
        const int* f = &b;

        int*& g = e;
        const int*& h = f;

        // 验证基础类型
        check_type2(a);                // 0 (T)，失败了，返回了2
        check_type2(b);                // 1 (const T)，失败了，返回了3
    
        // 验证引用类型
        check_type2(c);                // 2 (T&)
        check_type2(d);                // 3 (const T&)
    
        // 验证右值引用
        check_type2(std::move(a));     // 4 (T&&)
        check_type2(30);               // 4 (T&&)
    
        // 验证指针类型
        check_type2(e);                // 5 (T*)
        check_type2(f);                // 6 (const T*)
        
        check_type2(g);                // 5 (T*)
        check_type2(h);                // 6 (const T*)

    }
    
    static AutoRegTestFunc autoTestForward3(Test1);
#endif

    
    // 用于将编译器内部类型名称转换为可读格式
    string demangle(const char* name) {
        return name;
    }

    // 主模板函数，利用万能引用和类型分发
    template<typename T>
    void check_type(T&& param) {
        using RawT = remove_reference_t<T>; // 移除引用后的类型
        const bool is_ptr = is_pointer_v<RawT>;
        const bool is_const_ptr = is_ptr && is_const_v<remove_pointer_t<RawT>>;
        const bool is_ref = is_reference_v<T>;
        const bool is_const_ref = is_ref && is_const_v<remove_reference_t<T>>;

        cout << "参数类型: ";
        if (is_ptr) {
            if (is_const_ptr) cout << "const T*";
            else cout << "T*";
        } else if (is_ref) {
            if (is_const_ref) cout << "const T&";
            else if (is_rvalue_reference_v<T&&>) cout << "T&&";
            else cout << "T&";
        } else {
            if (is_const_v<RawT>) cout << "const T";
            else cout << "T";
        }
        cout << " (原始类型: " << demangle(typeid(T).name()) << ")" << endl;
    }
    
    template <typename T>
    void check_type3(T&& param) {
        using RawT = std::remove_reference_t<T>;  // 移除引用，保留原始类型（含指针和const）
        if constexpr (std::is_pointer_v<RawT>) {
            // 处理指针类型
            using PointeeT = std::remove_pointer_t<RawT>;
            if constexpr (std::is_const_v<PointeeT>) {
                std::cout << "TypeCode:6" << std::endl;  // const T*
            } else {
                std::cout << "TypeCode:5" << std::endl;  // T*
            }
        } else if constexpr (std::is_const_v<RawT>) {
            // 处理 const T（非指针）
            std::cout << "TypeCode:1" << std::endl;
        } else if constexpr (std::is_lvalue_reference_v<T&&>) {
            // 处理左值引用（T& 或 const T&）
            if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
                std::cout << "TypeCode:3" << std::endl;  // const T&
            } else {
                std::cout << "TypeCode:2" << std::endl;  // T&
            }
        } else if constexpr (std::is_rvalue_reference_v<T&&>) {
            // 处理右值引用（T&&）
            std::cout << "TypeCode:4" << std::endl;
        } else {
            // 基础类型 T
            std::cout << "TypeCode:0" << std::endl;
        }
    }
    
    static void Test2()
    {
        int a = 10;
        const int b = 20;
        int& c = a;
        const int& d = b;
        int* e = &a;
        const int* f = &b;

        check_type(a);       // T&
        check_type(b);       // const T&
        check_type(c);       // T&
        check_type(d);       // const T&
        check_type(30);      // T&&
        check_type(e);       // T*
        check_type(f);       // const T*
        check_type(move(a)); // T&&
    }
    
    static AutoRegTestFunc autoTestSFinae2(Test2);
    
}




