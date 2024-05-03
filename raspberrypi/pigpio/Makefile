CC=gcc
CFLAGS=-I. -lm -lrt -lpthread -lpigpio
DEPS = mcp320x.h
OBJ = mcp320x.o statistic.o mcp320xexp2.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mcp320xexp2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o mcp320xexp2
