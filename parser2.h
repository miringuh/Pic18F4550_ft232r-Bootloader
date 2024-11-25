#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <byteswap.h>
#if !defined(__PARSER)
#define __PARSER
#define CLR_EEPGD 0X9EA6
#define SET_EEPGD 0X8EA6
#define SET_CFG 0X8CA6
#define CLR_CFG 0X9CA6
////
uint32_t mem_addr = 0;
uint32_t conf_addr = 0x300000;
uint16_t addr = 0;
uint16_t state = 0;
uint8_t comms = 0X0D;
uint16_t prog_cnt = 0;
uint16_t inner_cnt = 0;
uint16_t test_cnt = 0;
uint32_t conf_cnt = 0;
uint16_t counterx = 0;
uint16_t countery = 0;
uint32_t rd_prog_buffer[16];
uint32_t word_buffer[16000];
uint16_t word_buffer_cnt = 0;
uint32_t word_conf_buffer[14];
uint16_t word_conf_buffer_cnt = 0;
uint32_t rd_conf_buffer[14];
//
uint8_t char2hex(char val)
{
    switch (val)
    {
    case '0':
        return 0X00;
        break;
    case '1':
        return 0X01;
        break;
    case '2':
        return 0X02;
        break;
    case '3':
        return 0X03;
        break;
    case '4':
        return 0X04;
        break;
    case '5':
        return 0X05;
        break;
    case '6':
        return 0X06;
        break;
    case '7':
        return 0X07;
        break;
    case '8':
        return 0X08;
        break;
    case '9':
        return 0X09;
        break;
    case 'A':
        return 0X0A;
        break;
    case 'B':
        return 0X0B;
        break;
    case 'C':
        return 0X0C;
        break;
    case 'D':
        return 0X0D;
        break;
    case 'E':
        return 0X0E;
        break;
    case 'F':
        return 0X0F;
        break;
    default:
        exit(0);
        break;
    }
}
uint8_t byte_swap(uint8_t val)
{
    uint8_t value1 = ((val & 0x0F) << 4);
    uint8_t value2 = ((val & 0xF0) >> 4);
    return (value1 | value2);
}
uint16_t swap_wrd(uint16_t addr)
{
    uint8_t high = (uint8_t)((addr & 0xFF00) >> 8);
    uint8_t low = (uint8_t)(addr & 0x00FF);

    uint8_t higher = byte_swap(high);
    uint8_t lower = byte_swap(low);
    uint16_t val = ((lower << 8) | higher);

    return val;
}
uint16_t swap_eeprom(uint16_t addr)
{
    uint8_t high = (uint8_t)((addr & 0xFF00) >> 8);
    uint8_t low = (uint8_t)(addr & 0x00FF);

    uint8_t higher = byte_swap(high);
    uint8_t lower = byte_swap(low);
    uint16_t val = ((lower << 8) | higher);

    return val;
}
void swap_address(uint32_t addr)
{ // oe3c
    uint8_t upper = (uint8_t)((addr & 0x3F0000) >> 16);
    uint8_t high = (uint8_t)((addr & 0x00FF00) >> 8);
    uint8_t low = (uint8_t)(addr & 0x0000FF);
    printf("K%.4X", byte_swap(upper) << 8 | 0xE0);
    printf("K%.4X",swap_wrd(0x6EF8));
    printf("K%.4X", byte_swap(high) << 8 | 0xE0);
    printf("K%.4X", swap_wrd(0x6EF7));
    printf("K%.4X", byte_swap(low) << 8 | 0xE0);
    printf("K%.4X", swap_wrd(0x6EF6));
}
//
void nop()
{
    printf("N");
}
void eeprom_prog_wr()
{
    printf("K%.4X", swap_eeprom(SET_EEPGD));
    printf("K%.4XS", swap_eeprom(CLR_CFG));
}
void bulk_erase()
{
    // printf("\n");
    swap_address(0X3C0005);
    printf("m%.4X", swap_wrd(0X3F3F));
    swap_address(0X3C0004);
    printf("m%.4X", swap_wrd(0X8F8F));
    printf("ZK0000");
    printf("K0000");
    eeprom_prog_wr();
}
// word
void sendWord(char a, char b, char c, char d, uint8_t comm)
{
    unsigned char val;
    word_buffer[word_buffer_cnt] = (char2hex(c) << 4) | char2hex(d);
    word_buffer_cnt++;
    word_buffer[word_buffer_cnt] = (char2hex(a) << 4) | char2hex(b);
    word_buffer_cnt++;
    uint8_t inner_cnt = 16;
    if (state == inner_cnt)
    {
        printf("NK0000");
        printf("S");
        state = 0;
        // beg_prog;
    }
    state++;
    addr++;
    comms = 0x0D; // post inc by 2
    val = 'M';
    if (comm == 0x0D)
    {
        val = 'M';
    }
    else
    {
        val = 'k';
    }
    swap_address((mem_addr + prog_cnt));
    printf("%c", val); // comm
    printf("%X%X%X%X", char2hex(a), char2hex(b), char2hex(c), char2hex(d));
    printf("K0000");
    // M-0x0d k-0x0f
    rd_prog_buffer[counterx] = (mem_addr + prog_cnt);
    prog_cnt++;
    counterx++;
    rd_prog_buffer[counterx] = (mem_addr + prog_cnt);
    prog_cnt++;
    counterx++;
    if (addr == 8)
    {
        prog_cnt = mem_addr;
        addr = 0;
        prog_cnt = 0;
    }
    if (state == (inner_cnt - 1))
    {
        comms = 0x0F; // start programming
    }
}
void address(char a, char b, char c, char d)
{
    mem_addr = (char2hex(a) << 12) + (char2hex(b) << 8) + (char2hex(c) << 4) + char2hex(d);
}
void word(char a, char b, char c, char d)
{
    // printf("\n");
    sendWord(b, a, d, c, comms);
    // printf("\n");
}
void read_prog()
{
    printf("NK0000N");
    printf("S");
    for (uint16_t i = 0; i < counterx; i++)
    {
        swap_address(rd_prog_buffer[i]);
        printf("QR");
    }
}
void read_conf()
{
    printf("S");
    for (uint8_t i = 0; i < 14; i++)
    {
        swap_address(0x300000 + i);
        printf("QR");
    }
}
// CONF //
void eeprom_conf_wr()
{
    printf("K%.4X", swap_eeprom(SET_EEPGD));
    printf("K%.4X", swap_eeprom(SET_CFG));
}
void configurations()
{
    eeprom_conf_wr();
    printf("S");
    rd_conf_buffer[0] = 0x1C;
    swap_address(0x300000);
    printf("k%.4X", swap_wrd(rd_conf_buffer[0]));
    printf("K0000");
    rd_conf_buffer[1] = 0xCC;
    swap_address(0x300001);
    printf("k%.4X", swap_wrd(rd_conf_buffer[1]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[2] = 0x10;
    swap_address(0x300002);
    printf("k%.4X", swap_wrd(rd_conf_buffer[2]));
    printf("K0000");
    rd_conf_buffer[3] = 0x00;
    swap_address(0x300003);
    printf("k%.4X", swap_wrd(rd_conf_buffer[3]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[4] = 0x00;
    swap_address(0x300004);
    printf("k%.4X", swap_wrd(rd_conf_buffer[4]));
    printf("K0000");
    rd_conf_buffer[5] = 0x80;
    swap_address(0x300005);
    printf("k%.4X", swap_wrd(rd_conf_buffer[5]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[6] = 0xE1;
    swap_address(0x300006);
    printf("k%.4X", swap_wrd(rd_conf_buffer[6]));
    printf("K0000");
    rd_conf_buffer[7] = 0x00;
    swap_address(0x300007);
    printf("k%.4X", swap_wrd(rd_conf_buffer[7]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[8] = 0x03;
    swap_address(0x300008);
    printf("k%.4X", swap_wrd(rd_conf_buffer[8]));
    printf("K0000");
    rd_conf_buffer[9] = 0xC0;
    swap_address(0x300009);
    printf("k%.4X", swap_wrd(rd_conf_buffer[9]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[10] = 0x0C;
    swap_address(0x30000A);
    printf("k%.4X", swap_wrd(rd_conf_buffer[10]));
    printf("K0000");
    rd_conf_buffer[11] = 0xC0;
    swap_address(0x30000B);
    printf("k%.4X", swap_wrd(rd_conf_buffer[11]) >> 8);
    printf("K0000");
    printf("S");
    rd_conf_buffer[12] = 0x0F;
    swap_address(0x30000C);
    printf("k%.4X", swap_wrd(rd_conf_buffer[12]));
    printf("K0000");
    rd_conf_buffer[13] = 0x40;
    swap_address(0x30000D);
    printf("k%.4X", swap_wrd(rd_conf_buffer[13]) >> 8);
    printf("K0000");
    printf("S");
}
void conf_address(char a, char b, char c, char d)
{
    conf_addr = 0x300000; //(char2hex(a) << 12) + (char2hex(b) << 8) + (char2hex(c) << 4) + char2hex(d);
    // printf("conf addr %X", conf_addr);
}
void wordf(char a, char b, char c, char d)
{

    uint16_t word_cnf_even = (char2hex(c) << 12) | (char2hex(d) >> 8);
    uint16_t word_cnf_odd = (char2hex(a) << 4) | (char2hex(b));

    switch (conf_addr + conf_cnt)
    {
    case 0x300004:
    case 0x300007:

        if (test_cnt == 2)
        {
            printf("S");
            test_cnt = 0;
        }
        rd_prog_buffer[conf_cnt] = 0x00;
        printf("    ld_wrd_conf(%.6x,0X0F,0x%.4X);", conf_addr + conf_cnt, 0x00);
        printf("K%.4X", swap_eeprom(0x0000));

        conf_cnt++;
        test_cnt++;
        break;

    default:
        if (test_cnt == 2)
        {
            printf("    S");
            test_cnt = 0;
        }
        if ((conf_cnt % 2) == 0)
        {
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);", (conf_addr + conf_cnt), word_cnf_odd);
            printf("K%.4X", swap_eeprom(0x0000));

            rd_prog_buffer[conf_cnt] = word_cnf_odd;
            conf_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);", (conf_addr + conf_cnt), word_cnf_even);
            printf("K%.4X", swap_eeprom(0x0000));

            rd_prog_buffer[conf_cnt] = ((word_cnf_even) >> 8);
            conf_cnt++;
            test_cnt++;
        }
        if ((conf_cnt % 2) != 0)
        {
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);", (conf_addr + conf_cnt), word_cnf_even);
            printf("K%.4X", swap_eeprom(0x0000));

            rd_prog_buffer[conf_cnt] = ((word_cnf_even) >> 8);
            conf_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);", (conf_addr + conf_cnt), word_cnf_odd);
            printf("K%.4X", swap_eeprom(0x0000));

            rd_prog_buffer[conf_cnt] = word_cnf_odd;
            conf_cnt++;

            test_cnt++;
        }
        break;
    }
    test_cnt++;
}

void readFile2()
{
    FILE *fd;
    char buff[16000];
    uint16_t cnt = 0;
    printf("S");
    bulk_erase();
    uint8_t byteCnt = 0;
    uint8_t status = 0;
    uint8_t ccnt = 1;
    fd = fopen("headers/p18f.hex", "r");
    fread(buff, sizeof(buff), 1, fd);
    for (uint16_t i = 0; i < strlen(buff); i++)
    {
        if (buff[i] == ':')
        {
            byteCnt = (char2hex(buff[i + 1]) << 4) + char2hex(buff[i + 2]);
            // if (byteCnt == 0x02)
            // {
            //     printf("");
            //     eeprom_conf_wr();
            //     conf_addr = 0x300000;
            //     address(buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6]);
            //     wordf(buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12]);
            // }
            // if (byteCnt == 0x0E)
            // {
            //     address(buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6]);
            //     wordf(buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12]);
            //     wordf(buff[i + 13], buff[i + 14], buff[i + 15], buff[i + 16]);
            //     wordf(buff[i + 17], buff[i + 18], buff[i + 19], buff[i + 20]);
            //     wordf(buff[i + 21], buff[i + 22], buff[i + 23], buff[i + 24]);
            //     wordf(buff[i + 25], buff[i + 26], buff[i + 27], buff[i + 28]);
            //     wordf(buff[i + 29], buff[i + 30], buff[i + 31], buff[i + 32]);
            //     wordf(buff[i + 33], buff[i + 34], buff[i + 35], buff[i + 36]);
            // }
            if (byteCnt == 0x04)
            {
                address(buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6]);
                word(buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12]);
                word(buff[i + 13], buff[i + 14], buff[i + 15], buff[i + 16]);
            }
            if (byteCnt == 0x10)
            {
                address(buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6]);
                word(buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12]);
                word(buff[i + 13], buff[i + 14], buff[i + 15], buff[i + 16]);
                word(buff[i + 17], buff[i + 18], buff[i + 19], buff[i + 20]);
                word(buff[i + 21], buff[i + 22], buff[i + 23], buff[i + 24]);
                word(buff[i + 25], buff[i + 26], buff[i + 27], buff[i + 28]);
                word(buff[i + 29], buff[i + 30], buff[i + 31], buff[i + 32]);
                word(buff[i + 33], buff[i + 34], buff[i + 35], buff[i + 36]);
                word(buff[i + 37], buff[i + 38], buff[i + 39], buff[i + 40]);
            }
            if (byteCnt == 0x00)
            {
                read_prog();
                configurations();
                read_conf();
            }
        }
    }

    printf("T");
    fclose(fd);
}
/*
S start
T stop
K comm--0x00
k comm--0x0F
Q COMM--0x09
M COMM--0x0D
m COMM--0x0C
N nop
Z erase_nop >nop/delay
R read_mem
*/
#endif // __PARSER