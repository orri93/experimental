all: static

dynamic:
	cd c; make dynamic
	cd go; make all

static:
	cd c; make static
	cd go; make all

clean:
	cd c; make clean
	cd go; make clean
