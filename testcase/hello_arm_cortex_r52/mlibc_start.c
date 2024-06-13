#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "uart_pl011.h"

int main(void)
{   
    char buf[] = "Hello cortex-r52\n";
    char *c = buf;

    UartStdOutInit();
    
    while(*c)
    {
        UartPutc(*c);
        ++c;
    }
    
    // 用户代码
    while (1)
    {
        // 主循环
    }
    return 0;
}
