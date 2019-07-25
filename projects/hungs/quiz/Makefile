CFLAGS= -Wall -fpic -coverage -lm -std=c99


testme: testme.c
	gcc -o testme testme.c $(CFLAGS)

result: testme
	./testme &> result.out
	gcov -b testme.c >> result.out
	cat testme.c.gcov >> result.out

clean:
	rm -f *.o testme  *.gcov *.gcda *.gcno *.so *.out 