/*************************************************************
 * Created by Yuning Gao 2019/7/28
 *************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "rngs.h"

 // Global Variables
int testsPassed = 0;
int testsFailed = 0;

/***************************************************
 *                   asserttrue
 * Parameter: take a true (non-0) or false (0) integer as input
 * Output: PASSED if true, FAILED if false
 * Returns: 1 for true and 0 for false
 ***************************************************/
int asserttrue(int testResult) {
	if (testResult) {
		printf("PASSED\n");
		testsPassed++;
		return 1;
	}
	else {
		printf("FAILED\n");
		testsFailed++;
		return 0;
	}
}


int main() {
	int seed = rand();
	int i, j, test;
	int numPlayer = 2;
	int player1 = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };
	int handCount;
	int handEstate;
	int supplyEstate;
	int baronPos; // Baron 
	int choice1; // 0 = do not discard, 1 = discard estate
	int discardBaron, discardEstate;
	int numDiscard, numCoin, numSupply;
	int numTest = 5000; // number of random tests
	struct gameState G, preG;

	// Run random tests
	for (test = 0; test < numTest; test++) {
		/******************************************
		*Set up random hand and action choice
		******************************************/
			// hand size is radomly selected
		handCount = (rand() % 5) + 1;
		int hand[handCount];
		handEstate = 0;
		// check the card
		baronPos = rand() % handCount;
		printf("Setting Hand, %d card(s):", handCount);
		for (i = 0; i < handCount; i++) {
			if (i == baronPos) {
				hand[baronPos] = baron;
			}
			else {
				// Randomly select cards in hand (there are 27 different cards)
				hand[i] = rand() % 27;
				if (hand[i] == 1) { handEstate++; }
			}
			printf(" %d", hand[i]);
		}
		printf("\n");

		// Randomly choose estate supply size (0-8)
		supplyEstate = rand() % 9;

		// Randomly choose action, 0: gain estate, 1: discard estate 
		choice1 = rand() % 2;
		if (!choice1) { printf("Choosing gain estate\n"); }
		else { printf("Choosing discard estate with %d estate(s) in hand\n", handEstate); }

		/******************************************
		*Run test
		******************************************/

		memset(&G, 0, sizeof(struct gameState)); // Clear game state
		initializeGame(numPlayer, k, seed, &G); // Initialize new game
		G.supplyCount[estate] = supplyEstate;
		G.handCount[player1] = handCount; // Set number of cards in hand
		// Copy hand with 1 Estate card
		memcpy(G.hand[player1], hand, sizeof(int) * handCount);
		// Save the previous game state before testing function
		memcpy(&preG, &G, sizeof(struct gameState));

		// Call the function being tested
		playCard(baronPos, choice1, 0, 0, &G); // 1 for discard

		// Check buys
		printf("Checking that 1 buy was gained:\n");
		asserttrue(G.numBuys == (preG.numBuys + 1));

		// Check discard
		printf("Checking that discard gained two cards:\n");
		asserttrue(G.discardCount[player1] == preG.discardCount[player1] + 2);
		discardBaron = 0;
		discardEstate = 0;
		for (j = 0; j < G.discardCount[player1]; j++) {
			// Is there an estate in the discard?
			if (G.discard[player1][j] == estate) { discardEstate++; }
			else if (G.discard[player1][j] == baron) { discardBaron++; }
		}
		printf("Checking that discard gained a baron:\n");
		asserttrue(discardBaron == 1);
		printf("Checking that discard gained an estate:\n");
		asserttrue(discardEstate == 1);

		// Check hand
		if (!choice1 || !handEstate) { numDiscard = 1; } // Baron discarded
		else { numDiscard = 2; } // Estate and Baron discarded
		printf("Check that hand lost %d card(s):\n", numDiscard);
		asserttrue(G.handCount[player1] == preG.handCount[player1] - numDiscard);
		if (!choice1 || !handEstate) { numCoin = 0; }
		else { numCoin = 4; } // Estate discarded for coins 
		printf("Testing that player received %d coins:\n", numCoin);
		asserttrue(G.coins == preG.coins + numCoin);

		// Check Supply
		if ((!choice1 || !handEstate) && supplyEstate) { numSupply = 1; }
		else { numSupply = 0; }
		printf("Testing that estate supply lost %d estate(s):\n", numSupply);
		asserttrue(G.supplyCount[estate] == preG.supplyCount[estate] - numSupply);
		printf("\n");
	}

	if (testsFailed) { printf("\nFAILED %d tests\n", testsFailed); }
	else { printf("\nPASSED all %d tests\n", testsPassed); }

	return 0;
}