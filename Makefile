CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2553
C_FILES := $(wildcard *.c)
H_FILES := $(wildcard *.h)
OBJS:=$(patsubst %.c, %.o, $(C_FILES))
OBJS_ELF:=$(patsubst %.c, %.elf, $(C_FILES))

all: $(OBJS) $(OBJS_ELF) $(H_FILES)

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr $(OBJS) $(OBJS_ELF)
