



#include "../SystemWrap.h"

#include <Windows.h>


int main()
{
    using namespace SimpleFileLogger;

    LogInit("test.log");

    Trace("%s %d %d %d \n", "abc", 1, 2, 3);

    Sleep(100);

    Trace("def \n");

    LogRelease();

    system("PAUSE");
    return 0;
}