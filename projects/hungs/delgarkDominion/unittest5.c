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

int checkMine(struct gameState *state, int playerNum) {

	//int mineEffect(int choice1, int choice2, int handPos, int currentPlayer, struct gameState *state)

	for (int i = 0; i < state->handCount[playerNum]; i++) {
		state->hand[playerNum][i] = mine;
	}

	int prevHandCount = state->handCount[playerNum];
	int playCount = state->playedCardCount;

	int mine = mineEffect(1, 1, 0, playerNum, state);

	int boolean = 1;

	if (prevHandCount - state->handCount[playerNum] > 1) {
		boolean = 0;
		char message[] = "Putting card into the hand\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 1;
		char message[] = "Putting card into the hand\n";
		asserttrue(boolean, message);
	}

	if (playCount +1 == state->playedCardCount) {
		boolean = 1;
		char message[] = "Test successful trashing of card\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test successful trashing of card\n";
		asserttrue(boolean, message);
	}

	if (mine == -1){
		boolean = 0;
		char message[] = "TEST correct card selction\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 1;
		char message[] = "TEST correct card selction\n";
		asserttrue(boolean, message);
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

	printf("Testing Mine.\n");
	int testMine = checkMine(&testG, thisPlayer);

	

	return 0;
}