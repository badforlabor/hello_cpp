#pragma once


class AutoRegTestFunc
{
public:
    typedef void (*Func)();
    AutoRegTestFunc(Func callback);

    static void RunAll();
    static void Destroy();
    static AutoRegTestFunc::Func Get(int idx);
};

#ifndef AUTO_REG_FUNC
#define C7_CONCAT_INTERNAL(a, b) a ## b
#define C7_CONCAT(a, b) C7_CONCAT_INTERNAL(a, b)
#define AUTO_REG_FUNC(x) static AutoRegTestFunc C7_CONCAT(AutoRegTestFuncTempVariable, __LINE__)(x);
#endif
