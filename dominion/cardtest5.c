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

	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };

	struct gameState G, testG, testH;

	initializeGame(numPlayers, k, seed, &G);

	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Testing Draw card.\n");

	testG.deckCount[thisPlayer] = 10;

	int testOne = drawCard(thisPlayer, &testG);

	int boolean;

	if (testOne == -1) {
		boolean = 0;
		char message[] = "Test # of cards in deck cannot be 0 after shuffling\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 1;
		char message[] = "Test # of cards in deck cannot be 0 after shuffling\n";
		asserttrue(boolean, message);
	}

	return 0;
}