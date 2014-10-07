CC      = gcc
LD      = ld
AR      = ar
OBJCOPY = objcopy
OBJDUMP = objdump

CFLAGS 		:= -Wall -Os 

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

all:
	${CC}  serv_jefby.c -o server 
	${CC}  client_jefby.c -o client 

.PHONY : 

clean:
	rm -f server client *.o *~ *.bak
	
