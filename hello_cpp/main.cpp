#include <iostream>
#include <string.h>

#include "AutoRegTestFunc.h"

int main(int argc, char* argv[])
{
    AutoRegTestFunc::RunAll();

    // 其他代码

    
    AutoRegTestFunc::Destroy();
    system("pause");
    return 0;
}
