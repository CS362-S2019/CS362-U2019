#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>


void asserttrue(int boolean, char message[]) {
	if (boolean) {
		printf("PASSED: %s", message);
	}
	else {
		printf("FAILED: %s", message);
	}
}

int main() {


	int r1, r2;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int nextPlayer = 1;

	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };

	struct gameState G, testG, testH, testI;

	initializeGame(numPlayers, k, seed, &G);

	int c1=0, c2=0, c3=0, bonus = 0, handPos=0;

	memcpy(&testG, &G, sizeof(struct gameState));
	
	int topTwo[2] = { -1,-1 };
	testG.deck[nextPlayer][testG.deckCount[nextPlayer] - 1] = gold;
	testG.deck[nextPlayer][testG.deckCount[nextPlayer] - 2] = copper;

	int prevCoins = testG.coins;

	int boolean = 1;

	printf("Test tribute card\n");
	
	int isPlus4 = handleTribute(nextPlayer, thisPlayer, &testG, topTwo);

	if (testG.coins == prevCoins +4 ) {
		boolean = 1;
		char message[] = "Test get correct gold\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test get correct gold\n";
		asserttrue(boolean, message);
	}
	
	memcpy(&testH, &G, sizeof(struct gameState));

	testH.deck[nextPlayer][testH.deckCount[nextPlayer] - 1] = estate;
	testH.deck[nextPlayer][testH.deckCount[nextPlayer] - 2] = duchy;

	topTwo[0] = -1;
	topTwo[1] = -1;

	int prevCount = testH.handCount[thisPlayer];

	int isPlusTwoCards = handleTribute(nextPlayer, thisPlayer, &testH, topTwo);

	if (testH.handCount[thisPlayer] == prevCount + 2) {
		boolean = 1;
		char message[] = "Test get correct hand count\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test get correct hand count\n";
		asserttrue(boolean, message);
	}

	memcpy(&testI, &G, sizeof(struct gameState));

	testI.deck[nextPlayer][testI.deckCount[nextPlayer] - 1] = baron;
	testI.deck[nextPlayer][testI.deckCount[nextPlayer] - 2] = minion;

	topTwo[0] = -1;
	topTwo[1] = -1;

	prevCount = testI.numActions;

	int isPlus4Actions = handleTribute(nextPlayer, thisPlayer, &testI, topTwo);

	if (testI.numActions == prevCount + 4) {
		boolean = 1;
		char message[] = "Test get correct action count\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test get correct action count\n";
		asserttrue(boolean, message);
	}

	
	return 0;
}