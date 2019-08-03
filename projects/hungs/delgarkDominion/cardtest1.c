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

	struct gameState G, testG, testH, testI;

	int m = initializeGame(1, k, seed, &G);

	//memcpy(&testG, &G, sizeof(struct gameState));

	printf("Testing Initialize game.\n");

	int boolean;

	if (m == -1) {
		boolean = 1;
		char message[] = "Test make sure at least 2 players\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test make sure at least 2 players\n";
		asserttrue(boolean, message);
	}

	int n = initializeGame(MAX_PLAYERS+1, k, seed, &testG);

	if (n == -1) {
		boolean = 1;
		char message[] = "Test make sure # players does not exceed MAX_PLAYERS\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test make sure # players does not exceed MAX_PLAYERS\n";
		asserttrue(boolean, message);
	}
	
	int kingdom[10] = { adventurer, adventurer, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };
	
	int o = initializeGame(numPlayers, kingdom, seed, &testH);

	if (o == -1) {
		boolean = 1;
		char message[] = "Test make sure no duplicate cards in the middle\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Test make sure no duplicate cards in the middle\n";
		asserttrue(boolean, message);
	}

	int p = initializeGame(numPlayers, k, seed, &testI);

	if (p == 0) {
		boolean = 1;
		char message[] = "Shuffle doesn't mess up\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "Shuffle doesn't mess up\n";
		asserttrue(boolean, message);
	}

	if (testI.supplyCount[curse] == 10) {
		boolean = 1;
		char message[] = "correct # of curses\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of curses\n";
		asserttrue(boolean, message);
	}

	if (testI.supplyCount[estate] == 8 && testI.supplyCount[duchy] == 8 && testI.supplyCount[province] == 8) {
		boolean = 1;
		char message[] = "correct # of victory cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of victory cards\n";
		asserttrue(boolean, message);
	}

	if(testI.supplyCount[copper] == (60 - (7*numPlayers)) ){
		boolean = 1;
		char message[] = "correct # of copper cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of copper cards\n";
		asserttrue(boolean, message);
	}

	if (testI.supplyCount[silver] == 40) {
		boolean = 1;
		char message[] = "correct # of silver cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of silver cards\n";
		asserttrue(boolean, message);
	}

	if (testI.supplyCount[gold] == 30) {
		boolean = 1;
		char message[] = "correct # of gold cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of gold cards\n";
		asserttrue(boolean, message);
	}

	if (testI.supplyCount[great_hall] == 8 && testI.supplyCount[gardens] == 8) {
		boolean = 1;
		char message[] = "correct # of victory kingdom cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of victory kingdom cards\n";
		asserttrue(boolean, message);
	}

	int hasCorrectCards = 1;

	for (int i = 0; i < 10; i++) {
		if (k[i] != great_hall && k[i] != gardens) {
			if (testI.supplyCount[i] != 10) {
				hasCorrectCards = 0;
			}
		}
	}

	if (hasCorrectCards == 1) {
		boolean = 1;
		char message[] = "correct # of other supply cards\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of other supply cards\n";
		asserttrue(boolean, message);
	}

	int hasCorrectHandCount = 1;

	for (int i = 0; i < numPlayers; i++) {

		if (testI.handCount[i] != 0 || testI.discardCount[i] != 0) {
			hasCorrectHandCount = 0;
		}
	}

	if (hasCorrectHandCount == 1) {
		boolean = 1;
		char message[] = "correct # of cards in hand\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of cards in hand\n";
		asserttrue(boolean, message);
	}

	int hasCorrectTokens = 1;

	for (int i = 0; i <= treasure_map; i++) {
		if (testI.embargoTokens[i] != 0) {
			hasCorrectTokens = 0;
		}
	}

	if (hasCorrectTokens == 1) {
		boolean = 1;
		char message[] = "correct # of tokens\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "correct # of tokens\n";
		asserttrue(boolean, message);
	}

	if (testI.outpostPlayed == 0) {
		boolean = 1;
		char message[] = "set outpost\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set outpost\n";
		asserttrue(boolean, message);
	}

	if (testI.phase == 0) {
		boolean = 1;
		char message[] = "set phase\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set phase\n";
		asserttrue(boolean, message);
	}

	if (testI.numActions == 1) {
		boolean = 1;
		char message[] = "set actions\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set actions\n";
		asserttrue(boolean, message);
	}

	if (testI.numBuys == 1) {
		boolean = 1;
		char message[] = "set buys\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set buys\n";
		asserttrue(boolean, message);
	}

	if (testI.playedCardCount == 0) {
		boolean = 1;
		char message[] = "set played cards\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set played cards\n";
		asserttrue(boolean, message);
	}

	if (testI.whoseTurn == 0) {
		boolean = 1;
		char message[] = "set player one turn\n";
		asserttrue(boolean, message);
	}

	else {
		boolean = 0;
		char message[] = "set player one turn\n";
		asserttrue(boolean, message);
	}

	if (testI.handCount[testI.whoseTurn] == 5) {
		boolean = 1;
		char message[] = "set first player's hand\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "set first player's hand\n";
		asserttrue(boolean, message);
	}

	return 0;
}