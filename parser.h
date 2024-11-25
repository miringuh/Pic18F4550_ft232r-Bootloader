#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <byteswap.h>
#if !defined(__PARSER)
#define __PARSER

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
uint32_t rd_prog_buffer[16];
uint32_t word_buffer[16000];
uint16_t word_buffer_cnt = 0;

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
void nop()
{
    printf("    nop();//4th pgc^p9 delay->pgc low p10\n");
}
void eeprom_prog_wr()
{
    printf("    command(0x00,SET_EEPGD);\n");
    printf("    command(0x00,CLR_CFG);//prog eeprom\n");
}
void bulk_erase()
{
    printf("    address(0X3C0005);\n");
    printf("    command(0x0C,0x3F3F);\n");
    printf("    address(0X3C0004);\n");
    printf("    command(0x0C,0x8F8F);\n");
    printf("    erase_nop();//blk erase\n\n");
    eeprom_prog_wr();
    printf("\n    start();\n");
}
//
void sendWord(char a, char b, char c, char d, uint8_t comm)
{
    word_buffer[word_buffer_cnt] = (char2hex(c) << 4) | char2hex(d);
    word_buffer_cnt++;
    word_buffer[word_buffer_cnt] = (char2hex(a) << 4) | char2hex(b);
    word_buffer_cnt++;
    uint8_t inner_cnt = 16;
    if (state == inner_cnt)
    {
        printf("    beg_prog();\n");
        printf("\n    start();\n");
        state = 0;
    }
    state++;
    addr++;
    comms = 0x0D; // post inc by 2
    printf("    ld_prog_wrd(0x%.6X,0X%.2X,0x%c%c%c%c);\n", (mem_addr + prog_cnt), comm, a, b, c, d);
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
    sendWord(c, d, a, b, comms);
}
void read_prog()
{
    printf("    beg_prog();\n");
    printf("    nop();\n\n");
    printf("\n    start();\n");
    for (uint16_t i = 0; i < counterx; i++)
    {
        printf("    read_prog_mem(0x%.6X,0x%.2X);\n", rd_prog_buffer[i], word_buffer[i]);
    }
    printf("\n");
}
// CONF //
void eeprom_conf_wr()
{
    read_prog();
    printf("    command(0x00,SET_EEPGD);\n");
    printf("    command(0x00,SET_CFG);//conf_eeprom\n");
}
void configurations()
{
    eeprom_conf_wr();
    printf("\n    start();\n");
    printf("    ld_wrd_conf(0x300000,0X0F,0x001C);\n");
    rd_prog_buffer[0] = 0x1C;
    printf("    ld_wrd_conf(0x300001,0X0F,0xCC00);\n");
    rd_prog_buffer[1] = 0xCC;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x300002,0X0F,0x0010);\n");
    rd_prog_buffer[2] = 0x10;
    printf("    ld_wrd_conf(0x300003,0X0F,0x0000);\n");
    rd_prog_buffer[3] = 0x00;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x300004,0X0F,0x0000);\n");/////
    rd_prog_buffer[4] = 0x00;
    printf("    ld_wrd_conf(0x300005,0X0F,0x8000);\n");
    rd_prog_buffer[5] = 0x80;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x300006,0X0F,0x00E1);\n");
    rd_prog_buffer[6] = 0xE1;
    printf("    ld_wrd_conf(0x300007,0X0F,0x0000);\n");////
    rd_prog_buffer[7] = 0x00;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x300008,0X0F,0x0003);\n");
    rd_prog_buffer[8] = 0x03;
    printf("    ld_wrd_conf(0x300009,0X0F,0xC000);\n");
    rd_prog_buffer[9] = 0xC0;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x30000A,0X0F,0x000C);\n");
    rd_prog_buffer[10] = 0x0C;
    printf("    ld_wrd_conf(0x30000B,0X0F,0xC000);\n");
    rd_prog_buffer[11] = 0xC0;
    printf("    start();\n");
    printf("    ld_wrd_conf(0x30000C,0X0F,0x000F);\n");
    rd_prog_buffer[12] = 0x0F;
    printf("    ld_wrd_conf(0x30000D,0X0F,0x4000);\n");
    rd_prog_buffer[13] = 0x40;
    printf("    start();\n");
}
void conf_address(char a, char b, char c, char d)
{
    conf_addr = 0x300000; //(char2hex(a) << 12) + (char2hex(b) << 8) + (char2hex(c) << 4) + char2hex(d);
    // printf("conf addr %X\n\n", conf_addr);
}
void wordf(char a, char b, char c, char d)
{

    uint16_t word_cnf_even = (char2hex(c) << 4) | (char2hex(d));
    uint16_t word_cnf_odd = (char2hex(a) << 12) | (char2hex(b) >> 8);

    switch (conf_addr + conf_cnt)
    {
    case 0x300004:
    case 0x300007:
        if (test_cnt == 2)
        {
            printf("    start();\n");
            test_cnt = 0;
        }
        test_cnt++;

        printf("    ld_wrd_conf(0x%.6x,0X0F,0x0000);\n", conf_addr + conf_cnt);
        // printf("    command(0x00,0x0000);\n");
        rd_prog_buffer[conf_cnt] = 0x00;
        conf_cnt++;
        break;
    default:
        if ((conf_cnt % 2) == 0)
        {
            if (test_cnt == 2)
            {
                printf("    start();\n");
                test_cnt = 0;
            }
            test_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);\n", (conf_addr + conf_cnt), word_cnf_even);
            // printf("    command(0x00,0x0000);\n");
            rd_prog_buffer[conf_cnt] = word_cnf_even;
            conf_cnt++;
            if (test_cnt == 2)
            {
                printf("    start();\n");
                test_cnt = 0;
            }
            test_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);\n", (conf_addr + conf_cnt), word_cnf_odd);
            // printf("    command(0x00,0x0000);\n");
            rd_prog_buffer[conf_cnt] = ((word_cnf_odd) >> 8);
            conf_cnt++;
        }
        if ((conf_cnt % 2) != 0)
        {
            if (test_cnt == 2)
            {
                printf("    start();\n");
                test_cnt = 0;
            }
            test_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);\n", (conf_addr + conf_cnt), word_cnf_odd);
            printf("    command(0x00,0x0000);\n");
            rd_prog_buffer[conf_cnt] = ((word_cnf_odd) >> 8);
            conf_cnt++;
            if (test_cnt == 2)
            {
                printf("    start();\n\n");
                test_cnt = 0;
            }
            test_cnt++;
            printf("    ld_wrd_conf(0x%.6X,0X0F,0x%.4X);\n", (conf_addr + conf_cnt), word_cnf_even);
            printf("    command(0x00,0x0000);\n");
            rd_prog_buffer[conf_cnt] = word_cnf_even;
            conf_cnt++;
        }
        
        break;
    }
}
//
void readFile()
{
    FILE *fd;
    char buff[16000];
    uint16_t cnt = 0;
    printf("#include </usr/lib/avr/include/avr/io.h>\n");
    printf("#include </usr/lib/avr/include/avr/builtins.h>\n");
    printf("#include \"headers/p18f4550.h\"\n");
    printf("\n\n");
    printf("int main(int argc, char const *argv[])\n");
    printf("{\n");
    printf("    _delay_ms(400);\n");
    printf("    DDRC=0X03;\n");
    printf("    DDRB=0X3F;\n");
    printf("    eusart_init(b19200h,1);\n");
    printf("    start();\n\n");
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
            //     printf("\n\n");
            //     eeprom_conf_wr();
            //     conf_addr = 0x300000;
            //     address(buff[i + 3], buff[i + 4], buff[i + 5], buff[i + 6]);
            //     // wordf(buff[i + 9], buff[i + 10], buff[i + 11], buff[i + 12]);
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
                configurations();
                printf("\n");
                for (uint8_t i = 0; i < 14; i++)
                {
                    printf("    read_conf_mem(0x%X,0x%.2X);\n", (0x300000 + i), rd_prog_buffer[i]);
                }
            }
        }
    }

    printf("    stop();\n");
    printf("    while(1);\n");
    printf("}\n");

    fclose(fd);
}

#endif // __PARSER