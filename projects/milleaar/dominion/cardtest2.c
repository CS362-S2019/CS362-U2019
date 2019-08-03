/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing shuffle function
*
* testShuffle: cardtest2.c dominion.o rngs.o
* gcc -o testShuffle -g cardtest2.c dominion.o rngs.o $(CFLAGS)
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
    printf("TESTING shuffle:\n");
#endif

    
#if (NOISY_TEST == 1)
            printf("Testing with deck count < 1\n");
#endif

    //make new game
    struct gameState G;
    r = initializeGame(numPlayer, k, seed, &G);
    int currentPlayer = whoseTurn(&G);

    //choosing a card to throw in near end to test shuffle order
    G.deck[currentPlayer][G.deckCount[currentPlayer] - 1] = smithy;
    beforeState = G;
    //manually setting deck count to < 1
    G.deckCount[currentPlayer] = 0;

    int result = shuffle(currentPlayer, &G);

    if(result == -1) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Deck count < 1 returned -1\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Deck count < 1 did not return -1\n");
#endif
    }

#if (NOISY_TEST == 1)
            printf("Testing shuffling capability\n");
#endif
    //resetting deck count to test shuffle of cards
    G.deckCount[currentPlayer] = beforeState.deckCount[currentPlayer];
    

    result = shuffle(currentPlayer, &G);

    //testing if cards are in same order
    int matched = 0;
    for(int i = 0; i < G.deckCount[currentPlayer]; i++)
    {
        if(G.deck[currentPlayer][i] == beforeState.deck[currentPlayer][i])
        {
            matched++;
        }
    }
    if(matched != G.deckCount[currentPlayer])
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Deck order not perfect match\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Deck order was not shuffled\n");
#endif
    }

#if (NOISY_TEST == 1)
            printf("Testing for missing cards\n");
#endif
    //testing if amount of cards are the same, implying no cards lost

    int beforeCopper = 0, beforeEstate = 0, beforeSmithy = 0;
    int afterCopper = 0, afterEstate = 0, afterSmithy = 0;

    for (int p = 0; p < beforeState.deckCount[currentPlayer]; p++)
    {
        if (beforeState.deck[currentPlayer][p] == copper)
        {
            beforeCopper++;
        }
        else if (beforeState.deck[currentPlayer][p] == estate)
        {
            beforeEstate++;
        }
        else if(beforeState.deck[currentPlayer][p] == smithy) 
        {
            beforeSmithy++;
        }
    }

    for (int p = 0; p < G.deckCount[currentPlayer]; p++)
    {
        if (G.deck[currentPlayer][p] == copper)
        {
            afterCopper++;
        }
        else if (G.deck[currentPlayer][p] == estate)
        {
            afterEstate++;
        }
        else if (G.deck[currentPlayer][p] == smithy) 
        {
            afterSmithy++;
        }
    }

    if (beforeCopper == afterCopper)
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Copper cards preserved\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Missing copper cards\n");
#endif
    }


    if (beforeEstate == afterEstate) {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Estate cards preserved\n");
#endif
    }
    else {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Missing estate cards\n");
#endif
    }

    if (beforeSmithy == afterSmithy)
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Smithy cards preserved\n");
#endif
    }
    else
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Missing smithy cards\n");
#endif
    }
            

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