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
	int numPlayers = 4;
	int thisPlayer = 0;

	int playerArray[4] = { 100, 2, 3 ,4 };
	int playerArrayTwo[4] = { 10, 10, 2 ,1 };
	int playerArrayThree[4] = { 2, 88, 88 ,1 };

	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };

	struct gameState G, testG, testH, testI;

	initializeGame(numPlayers, k, seed, &G);

	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Testing getWinners.\n");

	int t1 = getWinners(&playerArray, &testG);

	int boolean;


	if (playerArray[0] == 1 && playerArray[1] == 0 && playerArray[2] == 0 && playerArray[1] == 0) {
		boolean = 1;
		char message[] = "Test for 1 winner.\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test for 1 winner.\n";
		asserttrue(boolean, message);
	}

	memcpy(&testH, &G, sizeof(struct gameState));

	int t2 = getWinners(&playerArrayTwo, &testH);

	if (playerArrayTwo[0] == 1 && playerArrayTwo[1] == 1 && playerArrayTwo[2] == 0 && playerArrayTwo[1] == 0) {
		boolean = 1;
		char message[] = "Test for 2 winners.\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test for 2 winners.\n";
		asserttrue(boolean, message);
	}

	memcpy(&testI, &G, sizeof(struct gameState));

	testI.whoseTurn = 1;

	int t3 = getWinners(&playerArrayTwo, &testI);

	if (playerArrayThree[0] == 1 && playerArrayThree[1] == 0 && playerArrayThree[2] == 1 && playerArrayThree[1] == 0) {
		boolean = 1;
		char message[] = "Test for winner based on turn.\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test for winner based on turn.\n";
		asserttrue(boolean, message);
	}
	

	return 0;
}