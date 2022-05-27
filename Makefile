CFLAGS=-g

vpath %.h src
vpath %.c src

main : main.o freadstr.o test.o solve.o
solve.o : solve.h
freadstr.o : freadstr.h
test.o : test.h

.PHONY: clean
clean :
	rm -f main *.o