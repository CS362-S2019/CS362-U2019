/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing initializeGame function
*
* testInitGame: cardtest1.c dominion.o rngs.o
* gcc -o testInitGame -g cardtest1.c dominion.o rngs.o $(CFLAGS)
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

void checkSupply(struct gameState* state, int k[10], int players, float* testsPassed, float* totalTests){
    int pass = 1;
    int curseCount = 0, victoryCount = 0, hallGarden = 0;
    int kingdom = 10;
    int treasure = 60 - (7 * players);
    switch(players) {
        case 2:
            curseCount = 10;
            victoryCount = 8;
            hallGarden = 8;
            break;
        case 3:
            curseCount = 20;
            victoryCount = 12;
            hallGarden = 12;
            break;
        case 4:
            curseCount = 30;
            victoryCount = 12;
            hallGarden = 12;
            break;
    }

    //testing supply of curse
    if (state->supplyCount[curse] != curseCount){
        pass = 0;
#if (NOISY_TEST == 1)
        printf("\tCurse amount is incorrect\n");
#endif 
    }
    //testing victory counts
    if (state->supplyCount[estate] != victoryCount
        || state->supplyCount[duchy] != victoryCount
        || state->supplyCount[province] != victoryCount){
            pass = 0;
#if (NOISY_TEST == 1)
             printf("\tVictory amount is incorrect\n");
#endif 
    }
    //testing treasure amount
    if (state->supplyCount[copper] != treasure) {
        pass = 0;
#if (NOISY_TEST == 1)
        printf("\tTreasure amount is incorrect\n");
#endif 
    }
    //testing the kingdom cards were initialized    

    for (int j = 0; j < 10; j++)
    {
        if(k[j] != great_hall && k[j] != gardens)
        {
            if(state->supplyCount[ k[j] ] != kingdom)
            {
                pass = 0;
#if (NOISY_TEST == 1)
                printf("\tKingdom amount is incorrect\n");
#endif 
            }
        }
    }
    //testing great halls and gardens if they are present in the deck
    //also testing their supply counts
    if (state->supplyCount[great_hall] != hallGarden) {
        pass = 0;
#if (NOISY_TEST == 1)
        printf("\tGreat Hall amount is incorrect\n");
#endif 
    }
    if (state->supplyCount[gardens] != hallGarden) {
        pass = 0;
#if (NOISY_TEST == 1)
        printf("\tGardens amount is incorrect\n");
#endif 
    }

    if (pass) 
    {
        (*testsPassed)++;
        (*totalTests)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Supply counts were all correct\n");
#endif
    }
    else 
    {
        (*totalTests)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Supply counts were incorrect\n");
#endif
    }

}

void checkDecks(struct gameState* state, float* testsPassed, float* totalTests){
    //must check that the player decks are set to correct amounts
    int players = state->numPlayers;
    int currentPlayer = whoseTurn(state);
    int pass = 1;
    for(int i = 0; i < players; i++) {
        if(i == currentPlayer) {
            if(state->deckCount[i] != 5 || state->handCount[i] != 5) {
                pass = 0;
#if (NOISY_TEST == 1)
                printf("\tCurrent player card counts are incorrect\n");
#endif
            }
        }
        else if(state->deckCount[i] != 10 || state->handCount[i] != 0){
                pass = 0;
#if (NOISY_TEST == 1)
                printf("\tOther player card counts are incorrect\n");
#endif   
        }
    }

    if (pass) 
    {
        (*testsPassed)++;
        (*totalTests)++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Deck counts were all correct\n");
#endif
    }
    else 
    {
        (*totalTests)++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Deck counts were incorrect\n");
#endif
    }

}

int main()
{
    int seed = 1000;
    int numPlayer = 2;
    int r;
    int k[10] = {adventurer, council_room, feast, baron, mine
                 ,
                 remodel, smithy, village, gardens, great_hall};
    int badCards[10] = {adventurer, adventurer, feast, gardens, mine
                 ,
                 remodel, smithy, village, baron, great_hall};

    float testsPassed = 0;
    float totalTests = 0;


#if (NOISY_TEST == 1)
    printf("TESTING initializeGame:\n");
#endif

	// - players MAX + 1 and 1, expect -1
#if (NOISY_TEST == 1)
        printf("\tTesting Max + 1 player count\n");
#endif
    struct gameState G;
    r = initializeGame(MAX_PLAYERS + 1, k, seed, &G);
    if (r == -1) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Caught invalid player count parameter\n");
#endif
    }
    else 
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Did not catch invalid player count parameter\n");
#endif
    }
#if (NOISY_TEST == 1)
        printf("\tTesting 1 player count\n");
#endif
    r = initializeGame(1, k, seed, &G);
    if (r == -1) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Caught invalid player count parameter\n");
#endif
    }
    else 
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Did not catch invalid player count parameter\n");
#endif
    }

	// - set duplicate kingdom cards, expect -1
#if (NOISY_TEST == 1)
        printf("\tTesting Duplicate player cards\n");
#endif
    r = initializeGame(numPlayer, badCards, seed, &G);
    if (r == -1) 
    {
        testsPassed++;
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tPASS. Caught duplicate player cards\n");
#endif
    }
    else 
    {
        totalTests++;
#if (NOISY_TEST == 1)
        printf("\tFAILED. Did not catch duplicate player cards\n");
#endif
    }

	// - test curse card amounts 2,3,4 and victory card amounts 2,3
	// - check supply of great hall or gardens after setup of deck
    for (int i = 0; i < 3; i++)
    {
        int totalPlayers = numPlayer + i;
#if (NOISY_TEST == 1)
        printf("\tTesting supplies with %d players\n", totalPlayers);
#endif
        r = initializeGame(totalPlayers, k, seed, &G);
        checkSupply(&G, k, totalPlayers, &testsPassed, &totalTests);
        checkDecks(&G, &testsPassed, &totalTests);

        // for (i = adventurer; i <= treasure_map; i++) //loop all cards
        // {
        //     printf("Card%d: %d\n", i, G.supplyCount[i]);
        // }
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