src = $(wildcard *.c)
#src += $(wildcard *.S)
objs = $(patsubst %.c, %.o, $(src))

CFLAGS = #-Werror

CC = arm-linux-gcc

uart.bin : uart.elf
	arm-linux-objcopy -O binary -S uart.elf uart.bin
	arm-linux-objdump -D uart.elf > uart.dis

uart.elf : start.o $(objs)
	arm-linux-ld -Ttext 0 $^ -o $@

show :
	@echo src = $(src)

start.o : start.S
	$(CC) -c -o start.o start.S

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#all:
#	arm-linux-gcc -c -o led.o led.c
#	arm-linux-gcc -c -o uart.o uart.c
#	arm-linux-gcc -c -o main.o main.c
#	arm-linux-gcc -c -o start.o start.S
#	arm-linux-ld -Ttext 0 start.o led.o uart.o main.o -o uart.elf
#	arm-linux-objcopy -O binary -S uart.elf uart.bin
#	arm-linux-objdump -D uart.elf > uart.dis
clean:
	rm *.bin *.o *.elf *.dis
	
.PHONY : show clean

