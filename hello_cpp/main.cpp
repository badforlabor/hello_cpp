#include <iostream>
#include <string.h>

#include "AutoRegTestFunc.h"
#include "log.h"

int main(int argc, char* argv[])
{
    LogHelper::Info("start test...");
    AutoRegTestFunc::RunAll();

    // 其他代码

    
    AutoRegTestFunc::Destroy();
    LogHelper::Info("end test.");
    system("pause");
    return 0;
}
