/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing mineEffect function
*
* testMine: unittest5.c dominion.o rngs.o
*      gcc -o testMine -g  unittest5.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {

	int seed = 1000;
	int numPlayer = 2;
	int choice1, r;
	int handCount = 5;
	int k[10] = { adventurer, council_room, feast, gardens, mine
		, remodel, smithy, village, baron, great_hall };
	struct gameState beforeState;

	float testsPassed = 0;
	float totalTests = 0;

#if (NOISY_TEST == 1)
	printf("TESTING mineEffect:\n");
#endif


	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {

			switch (i) {
			case 0:
				choice1 = estate;
				break;
			case 1:
				choice1 = copper;
				break;
			case 2:
				choice1 = gold;
			}

			switch (j) {
			case 0:
				choice2 = -1;
				break;
			case 1:
				choice2 = copper;
				break;
			case 2:
				choice2 = silver;
				break;
			case 3:
				choice2 = gold;
			}

#if (NOISY_TEST == 1)
			printf("Testing with choice1 = %d, choice2 = %d", choice1, choice2);
#endif

			//make new game
			struct gameState G;
			r = initializeGame(numPlayer, k, seed, &G);
			int currentPlayer = whoseTurn(&G);

			
			G.hand[currentPlayer][0] = mine;
			G.hand[currentPlayer][1] = choice1;
			
			//printing hand contents
			// for(int handPos = 0; handPos < G.handCount[currentPlayer]; handPos++){
			//     printf("Card %d : %d\n", handPos, G.hand[currentPlayer][handPos]);	
			// }


			beforeState = G;
			int result = mineEffect(choice1, choice2, currentPlayer, &G, 0);


			//check treasure card amounts
			int beforeCopper, beforeSilver, beforeGold = 0;
			int afterCopper, afterSilver, afterGold = 0;
			for (int p = 0; p < beforeState.handCount[currentPlayer]; p++) {
				if (beforeState.hand[currentPlayer][i] == copper) {
					beforeCopper++;
				}
				else if (beforeState.hand[currentPlayer][i] == silver) {
					beforeSilver++;
				}
				else if (beforeState.hand[currentPlayer][i] == gold) {
					beforeGold++;
				}
			}
			for (int p = 0; p < G.handCount[currentPlayer]; p++) {
				if (G.hand[currentPlayer][i] == copper) {
					afterCopper++;
				}
				else if (G.hand[currentPlayer][i] == silver) {
					afterSilver++;
				}
				else if (G.hand[currentPlayer][i] == gold) {
					afterGold++;
				}
			}

			//giving bad choices
			if (i == 0 || j == 0) {

				if (result == -1) {
					testsPassed++;
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tPASS. Caught bad input\n");
#endif
				}
				else {
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tFAILED. Missed bad input\n");
#endif                     
				}
			}
			//if choice1 copper and choice2 is copper or silver
			else if (i == 1 && (j == 1 || j == 2)) {

				if (result == -1) {
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tFAILED. Was valid combination of treasures\n");
#endif              
				}

			}
			//if choice1 is copper and choice2 is gold
			else if (i == 1 && j == 3) {
				if (result == -1) {
					testsPassed++;
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tPASS. Caught bad combination\n");
#endif
				}
			}
			//if choice1 is gold and choice 2 is copper or silver
			else if (i == 2 && (j == 1 || j == 2)) {
				if (result == -1) {
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tFAILED. Was valid combination of treasures\n");
#endif              
				}
			}
			//if choice1 is gold and choice2 is gold
			else if (i == 2 && j == 3) {
				if (result == -1) {
					totalTests++;
#if (NOISY_TEST == 1)
					printf("\tFAILED. Was valid combination of treasures\n");
#endif              
				}
			}

			

		}
			


	}


	float percentPass = (testsPassed / totalTests) * 100.0;

#if (NOISY_TEST == 1)
	if (testsPassed == totalTests) {
		printf("ALL TESTS PASSED\n");
	}
	else {
		printf("%3.2f of Tests Passed.\n", percentPass);
	}
#endif

	return 0;
}
