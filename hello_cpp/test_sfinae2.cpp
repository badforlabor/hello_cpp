/**
 * Auth :   liubo
 * Date :   2025-03-14 08:38
 * Comment: sfinae的例子2
 */

// Copyright KuaiShou Games, Inc. All Rights Reserved.
// Author: liubo11@kuaishou.com
/**
 * Auth :   liubo
 * Date :   2024-12-11 09:15 
 * Comment: 理解SFINAE的意义，练习一些高级的关键字：concept, requires
 */ 

#include <iostream>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "log.h"

#include "AutoRegTestFunc.h"
#include "test_inline.h"

namespace test_sfinae2
{
    template<class T>
    class TSharedPtr : std::shared_ptr<T> {};
    
    template<class T>
    class TSharedRef : std::shared_ptr<T> {};
    
    template<class T>
    class TWeakPtr : std::weak_ptr<T> {};
    
    template<class T>
    class TArray : std::vector<T> {};
    
    template<class T>
    class TSet : std::unordered_set<T> {};
    
    template<class K, class V>
    class TMap : std::unordered_map<K, V> {};
}

namespace test_sfinae2
{    
    template <typename T>
    concept is_cpp_hotfix_type = requires {
        typename T::CppHotfixType; // inject through GENERATED_HOTFIX_BODY macro 
    };

    // 萃取 TSharedPtr<T> 中的 T
    template <typename T>
    struct ExtractSmartPtrType;

    template <typename T>
    struct ExtractSmartPtrType<TSharedPtr<T>> {
        using Type = T;
    };

    template <typename T>
    struct ExtractSmartPtrType<TSharedRef<T>> {
        using Type = T;
    };

    template <typename T>
    struct ExtractSmartPtrType<TWeakPtr<T>> {
        using Type = T;
    };

    // 移除 const 和引用
    template <typename T>
    struct RemoveConstAndRef {
        using Type = std::remove_const_t<std::remove_reference_t<T>>;
    };
    
    // 检查类型是否允许，支持 TSharedPtr<T>, TSharedRef<T>, TWeakPtr<T>, const 和 &
    template <typename T>
    struct IsTypeAllowedImpl {
        static constexpr bool value = is_cpp_hotfix_type<T>;
    };

    template <typename T>
    struct IsTypeAllowedImpl<T*> {
        static constexpr bool value = is_cpp_hotfix_type<T>;
    };

    template <typename T>
    struct IsTypeAllowedImpl<TSharedPtr<T>> {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value;
    };

    template <typename T>
    struct IsTypeAllowedImpl<TSharedRef<T>> {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value;
    };

    template <typename T>
    struct IsTypeAllowedImpl<TWeakPtr<T>> {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value;
    };

    template <typename T>
    struct IsTypeAllowedImpl<const T> {
        static constexpr bool value = IsTypeAllowedImpl<T>::value;
    };

    template <typename T>
    struct IsTypeAllowedImpl<T&&> {
        static constexpr bool value = IsTypeAllowedImpl<T>::value;
    };

    template <typename T>
    struct IsTypeAllowedImpl<TSet<T>> {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value;
    };
    

    template <typename T>
    struct IsTypeAllowedImpl<TArray<T>> {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value;
    };

    template <typename K, typename V>
    struct IsTypeAllowedImpl<TMap<K, V>> {
        using CleanType = typename RemoveConstAndRef<K>::Type; // 移除 const 和引用
        using CleanType2 = typename RemoveConstAndRef<V>::Type; // 移除 const 和引用
        static constexpr bool value = IsTypeAllowedImpl<CleanType>::value ||  IsTypeAllowedImpl<CleanType2>::value;
    };
    
    // IsTypeAllowed 的接口
    template <typename T>
    constexpr bool IsTypeAllowed() {
        using CleanType = typename RemoveConstAndRef<T>::Type; // 移除 const 和引用
        return IsTypeAllowedImpl<CleanType>::value;
    }


    template <class T>
    struct RemoveContainer {
        using Type      = T;
        using ConstType = const T;
    };
    template <class T>
    struct RemoveContainer<TArray<T>> {
        using Type      = RemoveContainer<T>::Type;
        using ConstType = RemoveContainer<T>::ConstType;
    };
    template <class T>
    struct RemoveContainer<TSet<T>> {
        using Type      = RemoveContainer<T>::Type;
        using ConstType = RemoveContainer<T>::ConstType;
    };
    
    class FClass0
    {
        
    };
    
    class FClass1
    {
    public:
        using CppHotfixType = void;        
    };
    
    static void Test1()
    {
        int falseList[] = {
            IsTypeAllowed<FClass0>(),
            IsTypeAllowed<int>(),
            IsTypeAllowed<TSharedPtr<int>>(),
            IsTypeAllowed<TArray<TSharedPtr<int>>>(),
            IsTypeAllowed<TArray<int>>(),
            IsTypeAllowed<TMap<int, int>>(),            
        };
        
        int trueList[] = {            
            IsTypeAllowed<FClass1>(),
            IsTypeAllowed<TSharedPtr<FClass1>>(),
            IsTypeAllowed<TArray<TSharedPtr<FClass1>>>(),
            IsTypeAllowed<TSet<TSharedPtr<FClass1>>>(),
            IsTypeAllowed<TSet<TSharedRef<TSet<TSharedRef<FClass1>>>>>(),
            IsTypeAllowed<TSet<TSharedPtr<FClass1>>>(),
            IsTypeAllowed<TSet<TSharedRef<TSet<TWeakPtr<FClass1>>>>>(),
            IsTypeAllowed<TSet<TWeakPtr<TSet<TSharedRef<FClass1>>>>>(),
            
            IsTypeAllowed<TMap<int, FClass1>>(),
            IsTypeAllowed<TMap<int, TSet<TSharedPtr<FClass1>>>>(),
            IsTypeAllowed<TWeakPtr<TMap<int, TSet<TSharedPtr<FClass1>>>>>(),
        };

        for(int i=0; i<std::size(falseList); i++)
        {
            assert(falseList[i] == 0);    
        }  
        for(int i=0; i<std::size(trueList); i++)
        {
            assert(trueList[i] == 1);
        }          
    }
    static void Test2()
    {
        {         
            auto v1 = (std::is_same<RemoveContainer<TSet<FClass1>>::Type, FClass1>::value);
            auto v2 = (std::is_same<RemoveContainer<TSet<TSharedPtr<FClass1>>>::Type, TSharedPtr<FClass1>>::value);
            auto v3 = (std::is_same<RemoveContainer<TSet<TSharedPtr<FClass1>>>::Type, FClass1>::value);
            assert(v1);
            assert(v2);
            assert(!v3);   
        }
        
        {         
            auto v1 = (std::is_same<RemoveContainer<TArray<TSet<FClass1>>>::Type, FClass1>::value);
            auto v2 = (std::is_same<RemoveContainer<TArray<TSet<TSharedPtr<FClass1>>>>::Type, TSharedPtr<FClass1>>::value);
            assert(v1);
            assert(v2);
        }

        std::cout << TypeName<int>::value() << std::endl;
        std::cout << TypeName<float>::value() << std::endl;
        std::cout << TypeName<std::string>::value() << std::endl;
    }
    
    static AutoRegTestFunc autoTest1(Test1);
    static AutoRegTestFunc autoTest2(Test2);
}

