#include <xc.h>
#if !defined(__P18_CONFIG)
#define __P18_CONFIG


#pragma config CONFIG1L = 0X1C
#pragma config CONFIG1H = 0XCC
#pragma config CONFIG2L = 0X10 
#pragma config CONFIG2H = 0X00
// #pragma config CONFIG3L = 0X00
#pragma config CONFIG3H = 0X80
#pragma config CONFIG4L = 0X31
// #pragma config CONFIG4H = 0X00
#pragma config CONFIG5L = 0X03
#pragma config CONFIG5H = 0XC0
#pragma config CONFIG6L = 0X0C
#pragma config CONFIG6H = 0XC0
#pragma config CONFIG7L = 0X0F
#pragma config CONFIG7H = 0X40


// //0 1L
// #pragma config PLLDIV=5
// #pragma config CPUDIV=OSC4_PLL6
// #pragma config USBDIV = 1
// //1 1H
// #pragma config FOSC = HS
// #pragma config FCMEN = ON
// #pragma config IESO = ON
// //2 2L
// #pragma config PWRT = ON
// #pragma config BOR = OFF
// #pragma config BORV = 2
// #pragma config VREGEN = OFF
// //3 2H
// #pragma config WDT = OFF
// #pragma config WDTPS = 1
// //4 ************3L
// //5 3H
// #pragma config CCP2MX = OFF
// #pragma config PBADEN = OFF
// #pragma config LPT1OSC = OFF
// #pragma config MCLRE = ON
// //6 4L
// #pragma config STVREN = ON
// #pragma config LVP = OFF
// #pragma config ICPRT = ON
// #pragma config XINST = OFF
// #pragma config DEBUG=OFF
// //7 ***********4H
// //8 5L
// #pragma config CP0=OFF
// #pragma config CP1=OFF
// // #pragma config CP2=OFF
// // #pragma config CP3=OFF
// //9 5H
// #pragma config CPB=OFF
// #pragma config CPD=OFF
// //A 6L
// #pragma config WRT0=OFF
// #pragma config WRT1=OFF
// // #pragma config WRT2=OFF
// // #pragma config WRT3=OFF
// //B 6H
// #pragma config WRTC=OFF
// #pragma config WRTB=OFF
// #pragma config WRTD=OFF
// //C 7L
// #pragma config EBTR0=OFF
// #pragma config EBTR1=OFF
// // #pragma config EBTR2=OFF
// // #pragma config EBTR3=OFF
// //D 7H
// #pragma config EBTRB=OFF

// #pragma config IDLOC0 = 0;
// #pragma config IDLOC1 = 0X000;
// #pragma config IDLOC2 = 0X000;
// #pragma config IDLOC3 = 0X000;
// #pragma config IDLOC4 = 0X000;
// #pragma config IDLOC5 = 0X000;
// #pragma config IDLOC6 = 0X000;
// #pragma config IDLOC7 = 0X000;

// DEVID0 0x3FFFFE
// DEVID1 0x3FFFFF

#endif // __P18_CONFIG
