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

	struct gameState G, testG;

	initializeGame(numPlayers, k, seed, &G);

	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Testing Baron.\n");
	//int testBaron = checkBarron(&testG, thisPlayer);

	int prevSupplyCount = supplyCount(estate, &testG);
	int prevEstateCount = testG.supplyCount[estate];
	int prevcoinCount = testG.coins;

	//test when there are no estates, draw estate card
	int r = handleBaron(thisPlayer, 0, &testG);
	
	int boolean = 1;
	
	if (supplyCount(estate, &testG) == (prevSupplyCount - 1) && testG.supplyCount[estate] == (prevEstateCount + 1) && r == 0) {
		boolean = 1;
		char message[] = "Test draw estate\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test draw estate\n";
		asserttrue(boolean, message);
	}

	//test when there are 1 or more estates, discard estate, will fail because of bug introduced so gcov will not be produced
	int l = handleBaron(thisPlayer, 1, &testG);

	if (testG.coins == (prevcoinCount + 4) && l == 0) {
		boolean = 1;
		char message[] = "Test discard estate\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test discard estate\n";
		asserttrue(boolean, message);
	}

	//test when there is only 1 estate left in supply, game is over
	testG.supplyCount[estate] = 1;
	for (int i = 0; i < testG.handCount[thisPlayer]; i++) {
		testG.hand[thisPlayer][i] = copper;
	}

	int q = handleBaron(thisPlayer, 1, &testG);
	if (q == 1) {
		boolean = 1;
		char message[] = "Test estate count goes to 0\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test estate count goes to 0\n";
		asserttrue(boolean, message);
	}	

	return 0;
}