//
// Created by Yuning on 2019/07/28.
//
#include "dominion.h"
#include "dominion_helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
//#include "rngs.h"

// Global Variables used by asserttrue function
int testsPassed = 0;
int testsFailed = 0;

/***************************************************
 *                   asserttrue
 * Parameters: a true (non-0) or false (0) integer input
 * Outputs: PASSED if true, FAILED if false
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
	int i, test;
	int numPlayer, player;
	int player1 = 0;
	int k[10] = { minion, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };
	int handCount;
	int minionPos; // Baron position in hand
	int choice1; // 1: gain 2 coins
	int choice2; // 1: discard hand
	int numDiscard, numDraw;
	int numTest = 100; // How many random tests to run
	struct gameState G, preG;

	// Run random tests
	for (test = 0; test < numTest; test++) {
		/***************************************************
				* Set up random hand and action choice
		***************************************************/
			// number of players are randomly selected between 2 to 4
		numPlayer = (rand() % 3) + 2;

		// Randomly choose action
		choice1 = rand() % 2;
		choice2 = rand() % 2;
		if (choice1) { printf("Choosing gain 2 coins\n"); }
		else if (choice2) { printf("Choosing discard hand\n"); }
		else { printf("No valid choice\n"); }

		// Start game
		memset(&G, 0, sizeof(struct gameState)); // Clear game state
		initializeGame(numPlayer, k, seed, &G); // Initialize new game

		for (player = 0; player < numPlayer; player++) {
			// Randomly select hand size 
			handCount = (rand() % 5) + 1;
			G.handCount[player] = handCount;
			minionPos = rand() % handCount;

			// Check the card name
			if (player == player1) { minionPos = rand() % handCount; }
			printf("Setting Player %d Hand, %d card(s):", player, handCount);
			for (i = 0; i < handCount; i++) {
				if (player == player1 && i == minionPos) {
					G.hand[player][i] = minion;
				}
				else {
					// Randomly select cards in hand (there are 27 different cards)
					G.hand[player][i] = rand() % 27;
				}
				printf(" %d", G.hand[player][i]);
			}
			printf("\n");
		}


		/***************************************************
		* Run test
		***************************************************/
			// Save the previous game state before testing function
		memcpy(&preG, &G, sizeof(struct gameState));

		// Call the function being tested
		playCard(minionPos, choice1, choice2, 0, &G); // 1 for discard

		printf("Checking that 1 action was gained:\n");
		// One action was lost playing card, so no net change
		asserttrue(G.numActions == preG.numActions);

		printf("Checking that %d coins were gained:\n", choice1 * 2);
		asserttrue(G.coins == preG.coins + (choice1 * 2));

		// Loop through each player to check discarded cards
		for (i = 0; i < numPlayer; i++) {
			if (!choice1 && choice2) { // Chose discard
				if (i == player1) { // First player
					numDiscard = G.handCount[player1];
					numDraw = 4;
				} // Other players
				else if (preG.handCount[i] == 5) {
					numDiscard = 5;
					numDraw = 4;
				}
				else {
					numDiscard = 0;
					numDraw = 0;
				}
			}
			else { // Did not choose discard
				numDraw = 0;
				if (i == player1) { // First player
					numDiscard = 1;
				}
				else { // Other players
					numDiscard = 0;
				}
			}
			printf("Checking that player %d discarded %d cards:\n", i, numDiscard);
			asserttrue(G.discardCount[i] == numDiscard);

			printf("Checking that player %d drew %d cards:\n", i, numDraw);
			asserttrue(G.handCount[i] == preG.handCount[i] - numDiscard + numDraw);
		}
	}

	if (testsFailed) { printf("\nFAILED %d tests\n", testsFailed); }
	else { printf("\nPASSED all %d tests\n", testsPassed); }

	return 0;
}