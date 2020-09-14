# Places a static or shared (dynamic) library into ../../lib

all: static

dynamic:
	gcc -I../../include -fPIC -c density.c
	gcc -shared -o libgosdrilling.so density.o
	mv -f libgosdrilling.so ../../lib
	rm -f density.o

static:
	gcc -I../../include -c density.c
	ar rc libgosdrilling.a density.o
	ranlib libgosdrilling.a
	mv -f libgosdrilling.a ../../lib
	rm -f density.o

clean:
	rm -f ../../lib/libgosdrilling.so ../../lib/libgosdrilling.a