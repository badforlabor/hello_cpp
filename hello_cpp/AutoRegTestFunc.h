#pragma once


class AutoRegTestFunc
{
public:
    typedef void (*Func)();
    AutoRegTestFunc(Func callback);

    static void RunAll();
    static void Destroy();
};
