all: clean
	gcc -fPIC -c -O0 -g test.c -I../
	gcc -fPIC -O3 test.o ../liblbr.a -o test
clean:
	rm  -f test.o test output.log
