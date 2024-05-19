CC=gcc
CFLAGS=-I. -lm -lrt -lpthread -lpigpio
DEPS = mcp4725.h
OBJ = mcp4725.o mcp4725exp1.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mcp4725exp1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o mcp4725exp1
