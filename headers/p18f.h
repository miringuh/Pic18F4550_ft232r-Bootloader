#if !defined(__P18F4550_)
#define __P18F4550_
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#define _XTAL_FREQ 20000000

void led(void)
{
    TRISA = 0;
    PORTA = 1;
    __delay_ms(40);
    PORTA = 2;
    __delay_ms(10);
}

#endif // __P18F4550_
