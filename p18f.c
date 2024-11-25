#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include "headers/p18f.h"
// #include "headers/conf.h"
__interrupt(low_priority) void getData(void)
{
}

int main(int argc, char const *argv[])
{
    ADCON1 = 0X00;
    CMCON = 0;
    while (1)
    {
        led();
        led();
        
    }
}
