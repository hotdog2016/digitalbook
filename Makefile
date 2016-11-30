CROSSCOMPILE:=arm-linux-

CFLAGS:=-I$(PWD)/include
CFLAGS+=-Wall -O2 -c
CFLAGS+=-march=armv4t

LDFLAGS:=-lfreetype -lm -lts -lpthread
CC:=$(CROSSCOMPILE)gcc
LD:=$(CROSSCOMPILE)ld

OBJS:=main.o\
	draw/draw.o\
	draw/pageopr.o\
	displaydev/fb.o\
	displaydev/disdev.o\
	font/freetype.o\
	font/fontmanager.o\
	encode/encodemanager.o\
	encode/utf-8.o\
	input/touchscreen_thread.o\
	input/stdinput_thread.o\
	input/inputmanager_thread.o\
	debug/printmanager.o\
	debug/serialprint.o\
	debug/netprint.o\
	network/netmanager.o\
	network/netinit.o

all:$(OBJS)
	$(CC) $(LDFLAGS) -o main $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJS) main

