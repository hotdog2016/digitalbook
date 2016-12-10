CROSSCOMPILE:=arm-linux-

CFLAGS:=-I$(PWD)/include
CFLAGS+=-Wall -O2 -c
CFLAGS+=-march=armv4t

LDFLAGS:=-lfreetype -lm -lts -lpthread -ljpeg
CC:=$(CROSSCOMPILE)gcc
LD:=$(CROSSCOMPILE)ld

OBJS:=main.o\
	displaydev/fb.o\
	displaydev/disdev.o\
	drawtext/drawtext.o\
	drawtext/pageopr.o\
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
	network/netinit.o\
	drawpicture/picmanager.o\
	drawpicture/bmpanalyze.o

all:$(OBJS)
	$(CC) $(LDFLAGS) -o main $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJS) main

