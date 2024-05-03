CC=gcc
CFLAGS=-I. -lpigpio -lrt -lpthread
DEPS = mcp320x.h
OBJ = mcp320x.o mcp320xexp1.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mcp320xexp1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o mcp320xexp1
