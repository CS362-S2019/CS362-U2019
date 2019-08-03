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

	printf("Testing Ambassador.\n");

	int testOne = ambassadorEffect(1, 1, 1, thisPlayer, &testG);

	int boolean = 1;

	if (testOne == -1) {
		boolean = 1;
		char message[] = "Test choice1 (hand #) cannot be the same as handPos\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test choice1 (hand #) cannot be the same as handPos\n";
		asserttrue(boolean, message);
	}

	memcpy(&testH, &G, sizeof(struct gameState));

	int testTwo = ambassadorEffect(0, 2, 1, thisPlayer, &testH);

	if (testTwo == -1) {
		boolean = 1;
		char message[] = "Test choice2 (# return to supply) must be 0 to 2 inclusive\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test choice2 (# return to supply) must be 0 to 2 inclusive\n";
		asserttrue(boolean, message);
	}

	

	return 0;
}