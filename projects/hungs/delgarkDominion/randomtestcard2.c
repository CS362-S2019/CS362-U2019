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

int checkMinionOne(struct gameState *state, int playerNum) {

	//int minionEffect(int choice1, int choice2, int handPos, int currentPlayer, struct gameState *state)

	//check choice 1 
	int prevCoins = state->coins;
	for (int i = 0; i < state->handCount[playerNum]; i++) {
		state->hand[playerNum][i] = minion;
	}
	int playedCount = state->playedCardCount;
	int m = minionEffect(1, 0, 0, playerNum, state);

	int boolean = 1;

	if (prevCoins + 2 == state->coins) {
		boolean = 1;
		char message[] = "Test +2 coins\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test +2 coins\n";
		asserttrue(boolean, message);
	}

	if (playedCount + 1 == state->playedCardCount) {
		boolean = 1;
		char message[] = "Test trashed card\n";
		asserttrue(boolean, message);
	}

	else  {
		boolean = 0;
		char message[] = "Test trashed card\n";
		asserttrue(boolean, message);
	}

	
}

int checkMinionTwo(struct gameState *state, int playerNum) {
	
	for (int i = 0; i < state->handCount[playerNum]; i++) {
		state->hand[playerNum][i] = minion;
	}
	int playedCount = state->playedCardCount;
	int m = minionEffect(0, 1, 0, playerNum, state);
	
	int boolean = 1;

	if (playedCount == state->playedCardCount) {
		boolean = 0;
		char message[] = "Test trash card\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 1;
		char message[] = "Test trash card\n";
		asserttrue(boolean, message);
	}

	int checkHandSize = 1;

	for (int j = 0; j < state->numPlayers; j++) {
		if (state->handCount[j] != 4) {
			checkHandSize = 0;
		}
	}

	if (checkHandSize) {
		boolean = 1;
		char message[] = "Test correct hand size after minion effect\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "Test correct hand size after minion effect\n";
		asserttrue(boolean, message);
	}
	
}


int main() {


	int r1, r2;
	int seed = 1000;
	
	int thisPlayer = 0;

	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };

	struct gameState G, testG, testH;

	printf("BEGIN RANDOM TESTING MINION");

	for (int i = 0; i < 100000; i++) {

		int numPlayers = rand() % 4 + 1;

		initializeGame(numPlayers, k, seed, &G);

		memcpy(&testG, &G, sizeof(struct gameState));

		int testMinion = checkMinionOne(&testG, thisPlayer);

		memcpy(&testH, &G, sizeof(struct gameState));

		int testMinionTwo = checkMinionTwo(&testH, thisPlayer);
	}
	
	printf("END RANDOM TESTING MINION");

	return 0;
}