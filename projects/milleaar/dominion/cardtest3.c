/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing endTurn function
*
* testEndTurn: cardtest3.c dominion.o rngs.o
* gcc -o testEndTurn -g cardtest3.c dominion.o rngs.o $(CFLAGS)
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

void checkVars(struct gameState *before, struct gameState *after, float* passed, float* total){

    if(after->phase == 0) 
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Phase set to 0\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Phase is not 0\n");
#endif
    }

    if(after->numActions == 1) 
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. numActions set to 1\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. numActions not 1\n");
#endif
    }

    if(after->numBuys == 1) 
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. numBuys set to 1\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. numBuys is not 1\n");
#endif
    }

    if(after->playedCardCount == 0) 
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. playedCardCount set to 0\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. playedCardCount is not 0\n");
#endif
    }

    //test that the new player drew 5 cards
    if(after->handCount[whoseTurn(after)] == 5) 
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Player drew 5 cards\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Player did not draw 5 cards\n");
#endif
    }

    //check that previous player discarded cards and hand is empty
    if(after->discardCount[whoseTurn(before)] > before->discardCount[whoseTurn(before)])
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Previous player discarded\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Player did not discard\n");
#endif
    }

    if(after->handCount[whoseTurn(before)] == 0)
    {
        (*passed)++;
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Previous player emptied hand\n");
#endif
    }
    else
    {
        (*total)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Player did not empty hand\n");
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
    struct gameState firstState, secondState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf("TESTING endTurn:\n");
#endif

    
#if (NOISY_TEST == 1)
            printf("Testing that player changes\n");
#endif

    //make new game
    struct gameState G;
    r = initializeGame(numPlayer, k, seed, &G);
    int firstPlayer = whoseTurn(&G);

    
    firstState = G;
    G.phase = 1;
    G.numActions = 0;
    G.numBuys = 0;
    G.playedCardCount = 1;
    int result = endTurn(&G);
    int nextPlayer = whoseTurn(&G);

    if(firstPlayer != nextPlayer) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Current Player changed\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Current player not changed\n");
#endif
    }

    checkVars(&firstState, &G, &testsPassed, &totalTests);

#if (NOISY_TEST == 1)
            printf("Testing that player changes when player is \"end\" of player array\n");
#endif

    secondState = G;
    G.phase = 1;
    G.numActions = 0;
    G.numBuys = 0;
    G.playedCardCount = 1;
    result = endTurn(&G);
    nextPlayer = whoseTurn(&G);

    if(firstPlayer == nextPlayer) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Current Player changed back to first player\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Current player not changed\n");
#endif
    }

    checkVars(&secondState, &G, &testsPassed, &totalTests);
            
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