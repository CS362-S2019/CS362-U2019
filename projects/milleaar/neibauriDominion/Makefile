CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
#testDrawCard: testDrawCard.c dominion.o rngs.o
#	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

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

unittestresults: dominion.o testBaron testMinion testAmbassador testTribute testMine testInitGame testShuffle testEndTurn testGetWinners testDrawCard
	./testBaron &> unittestresult.out
	./testMinion >> unittestresult.out
	./testAmbassador >> unittestresult.out
	./testTribute >> unittestresult.out
	./testMine >> unittestresult.out
	gcov dominion.c -b -f >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

randomBaron: randomtestcard1.c dominion.o rngs.o
	gcc -o randomBaron -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomMinion: randomtestcard2.c dominion.o rngs.o
	gcc -o randomMinion -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomTribute: randomtestcard3.c dominion.o rngs.o
	gcc -o randomTribute -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)

randomtestresults: randomBaron randomMinion randomTribute
	./randomBaron &> randomtestresults.out
	./randomMinion >> randomtestresults.out
	./randomTribute >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
