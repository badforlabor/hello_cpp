﻿#include "AutoRegTestFunc.h"

#include <vector>


static std::vector<AutoRegTestFunc::Func> *callbackList;

AutoRegTestFunc::AutoRegTestFunc(Func callback)
{
    if(callbackList == nullptr)
    {
        callbackList = new std::vector<AutoRegTestFunc::Func>;        
    }
    callbackList->push_back(callback);
}

void AutoRegTestFunc::RunAll()
{
    if(callbackList == nullptr)
    {
        return;
    }
    
    for(auto it : *callbackList)
    {
        if(it)
        {
            it();
        }
    }
}

void AutoRegTestFunc::Destroy()
{
    if(callbackList == nullptr)
    {
        return;
    }
    
    delete callbackList;
    callbackList = nullptr;
}
AutoRegTestFunc::Func AutoRegTestFunc::Get(int idx)
{
    if (callbackList == nullptr)
    {
        return nullptr;
    }

    if (idx >= 0 && callbackList->size())
    {
        return (*callbackList)[idx];
    }
    return nullptr;
}
