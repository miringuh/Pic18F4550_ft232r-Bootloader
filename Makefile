FILE=p18f
PFILE=/usr/bin/clang
PATH=/opt/microchip/xc8/v2.46/pic/bin/xc8
PATH2=/opt/microchip/xc8/v2.46/bin/xc8-cc

all:
	${PATH2} -mcpu=18f4550 --fill=0XFFFF@0:0X009F -mdownload-hex  -mno-keep-startup -ginhx32 -oheaders/$(FILE) ${FILE}.c
#	${PATH} --chip=18F4550 --fill=0X00FF@0x0800:0X8BF -Q -Oheaders/$(FILE) ${FILE}.c
#	--fill=0X3FFF@0:0X1F
#	-mdownload-hex
#	make parser
parser:
	/usr/bin/clang -oparser2.o parser.c
	./parser2.o>headers/test.txt
#	/usr/bin/clang -oparser.o parser.c
#	./parser.o>../AVRs/isp.c
#	./parser.o>headers/test.txt

arch:
	/opt/microchip/xc8/v2.46/bin/xc8-ar dv --target=so headers/f628.h

clean:
	/bin/rm -f headers/*.sym headers/*.hex headers/*.sdb headers/*.lst headers/*.o headers/*.rlf headers/*.pre headers/*.mum
	/bin/rm -f headers/*.s headers/*.d headers/*.i headers/*.p1 headers/*.cmf headers/*.elf headers/*.hxl headers/*.as 
	/bin/rm -f headers/*.obj
	/bin/clear