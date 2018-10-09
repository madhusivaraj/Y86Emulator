CC = gcc
CFLAGS = -lm -Wall
COMPILE = $(CC) $(CFLAGS)

all: y86emul y86dis

y86emul:  y86emul.c y86emul.h
		$(COMPILE) -o y86emul y86emul.c

y86dis:  y86dis.c y86emul.h y86dis.h
		$(COMPILE) -o y86dis y86dis.c

clean:
	rm -f y86emul
	rm -f y86dis
	rm -f *.o
