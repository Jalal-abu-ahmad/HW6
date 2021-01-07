CC=gcc
CCLINK=$(CC) 
CFLAGS=-g -Wall -std=c99
OBJS= ransom.c
EXEC=prog.exe
RM=rm -rf

$(EXEC): $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

clean:
	$(RM) *.o *.exe