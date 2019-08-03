/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing drawCard function
*
* testDrawCard: cardtest5.c dominion.o rngs.o
* gcc -o testDrawCard -g cardtest5.c dominion.o rngs.o $(CFLAGS)
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

void checkCounts(struct gameState* before, struct gameState* after, float* passed, float* total){
    
    int currentPlayer = whoseTurn(before);

#if (NOISY_TEST == 1)
    printf("\t\tDeck decremented correctly: ");
#endif
    if(before->deckCount[currentPlayer] - 1 == after->deckCount[currentPlayer]) {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED\n");
#endif
    }

#if (NOISY_TEST == 1)
    printf("\t\tHand incremented correctly: ");
#endif
    if(before->handCount[currentPlayer] + 1 == after->handCount[currentPlayer]) {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED\n");
#endif
    }

}

void checkCard(struct gameState* before, struct gameState* after, float* passed, float* total){

    int currentPlayer = whoseTurn(before);
    int deckCounterBefore = before->deckCount[currentPlayer];
    int handCounterAfter = after->handCount[currentPlayer];

#if (NOISY_TEST == 1)
    printf("\t\tCard drawn from deck: ");
#endif
    if(before->deck[currentPlayer][deckCounterBefore - 1] == after->hand[currentPlayer][handCounterAfter - 1])
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED\n");
#endif
    }

}

int main()
{
    int seed = 1000;
    int numPlayer = 2;
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
                 ,
                 remodel, smithy, village, baron, great_hall};
    struct gameState beforeState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf("TESTING drawCard:\n");
#endif
#if (NOISY_TEST == 1)
    printf("\tTesting empty discard and empty deck:\n");
#endif
    // - give empty discard pile and deck count 0 expect -1
    struct gameState G;
    r = initializeGame(numPlayer, k, seed, &G);
    int currentPlayer = whoseTurn(&G);
    
    G.discardCount[currentPlayer] = 0;
    G.deckCount[currentPlayer] = 0;
    int result = drawCard(currentPlayer, &G);
    if(result == -1) {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\t\tPASS. Caught bad input\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\t\tFAILED. Missed bad input\n");
#endif
    }

#if (NOISY_TEST == 1)
    printf("\tTesting full discard pile and empty deck:\n");
#endif
	// - give full discard pile with deck count 0
    r = initializeGame(numPlayer, k, seed, &G);
    for (int i = 0; i < G.deckCount[currentPlayer]; i++)
    {
      G.discard[currentPlayer][i] = G.deck[currentPlayer][i];
      G.deck[currentPlayer][i] = -1;
    }
    G.discardCount[currentPlayer] = G.deckCount[currentPlayer];
    G.deckCount[currentPlayer] = 0;
    beforeState = G;
    result = drawCard(currentPlayer, &G);

    if(G.deckCount[currentPlayer] + 1 == beforeState.discardCount[currentPlayer]) {
        if(G.discardCount[currentPlayer] == beforeState.deckCount[currentPlayer])
        {
            testsPassed++;
            totalTests++;
#if (NOISY_TEST == 1)
            printf("\t\tPASS. Discard moved to deck and drawn from\n");
#endif
        }
        else
        {
            totalTests++;
#if (NOISY_TEST == 1)
            printf("\t\tFAILED.\n");
#endif
        }
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\t\tFAILED.\n");
#endif
    }

#if (NOISY_TEST == 1)
    printf("\tTesting normal deck and discard:\n");
#endif
    //checking normal implementation
    r = initializeGame(numPlayer, k, seed, &G);
    beforeState = G;
    result = drawCard(currentPlayer, &G);
    checkCard(&beforeState, &G, &testsPassed, &totalTests);
    checkCounts(&beforeState, &G, &testsPassed, &totalTests);

    float percentPass = (testsPassed / totalTests) * 100.0;

#if (NOISY_TEST == 1)
    if (testsPassed == totalTests)
    {
        printf("ALL TESTS PASSED\n");
    }
    else
    {
        printf("%3.2f of Tests Passed.\n", percentPass);
    }
#endif

    return 0;
}