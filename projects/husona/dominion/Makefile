CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion_helpers.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)
	
randomtestresults: dominion.o randomtestcard1.c
	echo "randomtestcard1.c:" &> randomtestresults.out
	gcc -g -o randomtestcard1 rngs.c dominion.c randomtestcard1.c $(CFLAGS)
	randomtestcard1 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	
	echo "randomtestcard2.c:" >> randomtestresults.out
	gcc -g -o randomtestcard2 rngs.c dominion.c randomtestcard2.c $(CFLAGS)
	randomtestcard2 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	
	echo "randomtestcard3.c:" >> randomtestresults.out
	gcc -g -o randomtestcard3 rngs.c dominion.c randomtestcard3.c $(CFLAGS)
	randomtestcard3 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	
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
	gcov -b -f dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe randomtestcard1 randomtestcard1.exe randomtestcard2 randomtestcard2.exe randomtestcard3 randomtestcard3.exe
