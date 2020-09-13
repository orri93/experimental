# Places a static or shared (dynamic) library into ../../lib

all: static

dynamic:
	gcc -I../../include -fPIC -c geometry.c
	gcc -I../../include -fPIC -c interpolate.c
	gcc -I../../include -fPIC -c noise.c
	gcc -I../../include -fPIC -c signal.c
	gcc -shared -o libgos.so geometry.o interpolate.o noise.o signal.o
	mv -f libgos.so ../../lib
	rm -f geometry.o interpolate.o noise.o signal.o

static:
	gcc -I../../include -c geometry.c
	gcc -I../../include -c interpolate.c
	gcc -I../../include -c noise.c
	gcc -I../../include -c signal.c
	ar rc libgos.a geometry.o interpolate.o noise.o signal.o
	ranlib libgos.a
	mv -f libgos.a ../../lib
	rm -f geometry.o interpolate.o noise.o signal.o
