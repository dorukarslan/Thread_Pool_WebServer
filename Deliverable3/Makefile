# An admittedly primitive Makefile
# To compile, type "make" or make "all"
# To remove files, type "make clean"

CC = gcc
CFLAGS = -Wall
OBJS = wserver.o wclient.o request.o io_helper.o 
LIBS= -pthread
.SUFFIXES: .c .o 

all: wserver wclient spin.cgi


wserver: wserver.o request.o io_helper.o 
	$(CC) $(CFLAGS) -o wserver wserver.o request.o io_helper.o  $(LIBS)

wclient: wclient.o io_helper.o
	$(CC) $(CFLAGS) -o wclient wclient.o io_helper.o  $(LIBS)

spin.cgi: spin.c
	$(CC) $(CFLAGS) -o spin.cgi spin.c  $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	-rm -f $(OBJS) wserver wclient spin.cgi
