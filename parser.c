#include <stdio.h>
// #include "parser.h"
#include "parser2.h"
int main(int argc, char const *argv[])
{
    // readFile();
    readFile2();
    return 0;
}

/*
S start
T stop
K command --0x00
k comm--0x0F
Q 0x09
M 0x0D
m 0x0C
N nop
Z erase_nop >nop/delay
R read_mem

*/
/*
uint8_t byte_swap(uint8_t val)
{
    uint8_t value1 = ((val & 0x0F) << 4);
    uint8_t value2 = ((val & 0xF0) >> 4);
    return (value1 | value2);
}
void swap_address(uint32_t addr)
{
    uint8_t upper = (uint8_t)((addr & 0xFF0000) >> 16);
    uint8_t high = (uint8_t)((addr & 0x00FF00) >> 8);
    uint8_t low = (uint8_t)(addr & 0x0000FF);

    printf("K%.4X", ((byte_swap(upper)) << 8) | 0xE0);
    printf("K8FE6\n");
    printf("K%.4X", ((byte_swap(high)) << 8) | 0xE0);
    printf("K7FE6\n");
    printf("K%.4X", ((byte_swap(low)) << 8) | 0xE0);
    printf("K6FE6\n");
}
uint32_t swap_num(uint32_t addr)
{
    uint8_t upper = (uint8_t)((addr & 0xFF0000) >> 16);
    uint8_t higher = (uint8_t)((addr & 0x00FF00) >> 8);
    uint8_t lower = (uint8_t)((addr & 0x0000FF));
    uint8_t up = byte_swap(upper);
    uint8_t high = byte_swap(higher);
    uint8_t low = byte_swap(lower);
    uint32_t val = up | ((high) << 8) | ((low) << 16);

    return val;
}
uint16_t swap_eeprom(uint16_t addr)
{
    uint8_t higher = (uint8_t)((addr & 0xFF00) >> 8);
    uint8_t lower = (uint8_t)((addr & 0x00FF));
    uint8_t high = byte_swap(higher);
    uint8_t low = byte_swap(lower);
    uint32_t val = (high) | ((low) << 8);
    return val;
}


S start
T stop
W word
K command
k comm
N nop
Z erase_nop >nop/delay
X address
P ld_prog_wrd
p ld_wrd_conf
V verify
R read_mem
*/