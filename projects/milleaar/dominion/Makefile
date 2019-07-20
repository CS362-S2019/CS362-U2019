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

testBaron: unittest1.c dominion.o rngs.o
	gcc -o testBaron -g  unittest1.c dominion.o rngs.o $(CFLAGS)

testMinion: unittest2.c dominion.o rngs.o
	gcc -o testMinion -g  unittest2.c dominion.o rngs.o $(CFLAGS)

testAmbassador: unittest3.c dominion.o rngs.o
	gcc -o testAmbassador -g  unittest3.c dominion.o rngs.o $(CFLAGS)

testTribute: unittest4.c dominion.o rngs.o
	gcc -o testTribute -g  unittest4.c dominion.o rngs.o $(CFLAGS)

testMine: unittest5.c dominion.o rngs.o
	gcc -o testMine -g  unittest5.c dominion.o rngs.o $(CFLAGS)

testShuffle: cardtest2.c dominion.o rngs.o
	gcc -o testShuffle -g cardtest2.c dominion.o rngs.o $(CFLAGS)

testEndTurn: cardtest3.c dominion.o rngs.o
	gcc -o testEndTurn -g cardtest3.c dominion.o rngs.o $(CFLAGS)

testDrawCard: cardtest5.c dominion.o rngs.o
	gcc -o testDrawCard -g cardtest5.c dominion.o rngs.o $(CFLAGS)

testGetWinners: cardtest4.c dominion.o rngs.o
	gcc -o testGetWinners -g cardtest4.c dominion.o rngs.o $(CFLAGS)

unittestresults: testBaron testMinion testAmbassador testTribute testMine
	./testBaron &> unittestresult.out
	./testMinion &> unittestresult.out
	./testAmbassador &> unittestresult.out
	./testTribute &> unittestresult.out
	./testMine &> unittestresult.out
	./testShuffle &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
