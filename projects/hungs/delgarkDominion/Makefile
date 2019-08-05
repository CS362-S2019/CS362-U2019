CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults: 
	echo "Results of Unit Test 1:" >> unittestresults.out
	gcc -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)
	unittest1 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Unit Test 2:" >> unittestresults.out
	gcc -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)
	unittest2 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Unit Test 3:" >> unittestresults.out
	gcc -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)
	unittest3 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Unit Test 4:" >> unittestresults.out
	gcc -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)
	unittest4 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Unit Test 5:" >> unittestresults.out
	gcc -o unittest5 dominion.c rngs.c unittest5.c $(CFLAGS)
	unittest5 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Card Test 1:" >> unittestresults.out
	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)
	cardtest1 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Card Test 2:" >> unittestresults.out
	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)
	cardtest2 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Card Test 3:" >> unittestresults.out
	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)
	cardtest3 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Card Test 4:" >> unittestresults.out
	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)
	cardtest4 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out
	echo "Results of Card Test 5:" >> unittestresults.out
	gcc -o cardtest5 dominion.c rngs.c cardtest5.c $(CFLAGS)
	cardtest5 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	gcov dominion.c -f >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

unittest1: unittest1.c dominion.o 
	gcc -g -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)

unittest2: unittest2.c dominion.o 
	gcc -g -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)

unittest3: unittest3.c dominion.o 
	gcc -g -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)

unittest4: unittest4.c dominion.o 
	gcc -g -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)

unittest5: unittest5.c dominion.o 
	gcc -g -o unittest5 dominion.c rngs.c unittest5.c $(CFLAGS)

cardtest1:cardtest1.c dominion.o 
	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)

cardtest2:cardtest2.c dominion.o 
	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)

cardtest3:cardtest3.c dominion.o 
	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)

cardtest4:cardtest4.c dominion.o 
	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)

cardtest5:cardtest5.c dominion.o 
	gcc -o cardtest5 dominion.c rngs.c cardtest5.c $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o
	gcc -o randomtestcard1 dominion.c rngs.c randomtestcard1.c $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o
	gcc -o randomtestcard2 dominion.c rngs.c randomtestcard2.c $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o
	gcc -o randomtestcard3 dominion.c rngs.c randomtestcard3.c $(CFLAGS)

randomtestresults: 
	make randomtestcard1
	echo "Running Random 1" >> randomtestresults.out
	./randomtestcard1
	gcov -f -b -c dominion.c >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out
	make randomtestcard2
	echo "Running Random 2" >> randomtestresults.out
	./randomtestcard2
	gcov -f -b -c dominion.c >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out
	make randomtestcard3
	echo "Running Random 3" >> randomtestresults.out
	./randomtestcard3
	gcov -f -b -c dominion.c >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest2 unittest3 unittest4 unittest5 cardtest1 cardtest2 cardtest3 cardtest4 cardtest5 randomtestcard1 randomtestcard2 randomtestcard3

