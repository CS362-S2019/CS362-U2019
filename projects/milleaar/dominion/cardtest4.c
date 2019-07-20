/* -----------------------------------------------------------------------
* Aaron Miller
*
* Testing getWinners function
* 
* testGetWinners: cardtest4.c dominion.o rngs.o
* gcc -o testGetWinners -g cardtest4.c dominion.o rngs.o $(CFLAGS)
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
    int card = duchy;
    int players[MAX_PLAYERS] = {0, 0, 0, 0};
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
                 ,
                 remodel, smithy, village, baron, great_hall};
    struct gameState beforeState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf("TESTING getWinners:\n");
#endif

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int totalPlayers = numPlayer + i;
            int expectedWinner = j;
            if(j >= totalPlayers) {
                continue;
            }
            
#if (NOISY_TEST == 1)
            printf("\tTesting with %d players:\n", totalPlayers);
            printf("\tTesting with player%d with card:%d:\n", expectedWinner+1, card);
#endif

            //make new game
            struct gameState G;
            r = initializeGame(totalPlayers, k, seed, &G);

            
            G.deck[expectedWinner][0] = card;

            beforeState = G;
            for(int p = 0; p < MAX_PLAYERS; p++){
                players[p] = 0;
            }

            int result = getWinners(players, &G);

            for(int p = 0; p < MAX_PLAYERS; p++){
#if (NOISY_TEST == 1)
                printf("\t\tPlayer%d: %d\n", p + 1, players[p]);
#endif
            }

            if(players[expectedWinner] == 1) 
            {
                testsPassed++;
                totalTests++;
#if (NOISY_TEST == 1)
                printf("\tPASS. Correct winner chosen\n");
#endif
            }
            else 
            {
                int winners = 0;
                for(int p = 0; p < totalPlayers; p++) {
                    if(players[p] == 1)
                    {
                        winners++;
                    }
                }
                if(winners)
                {
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tFAILED. Did not select the correct winner\n");
#endif
                }
                else 
                {
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tFAILED. Did not select ANY winner\n");
#endif
                }

            }
        }
    }

//testing again but with multiple winners
    card = curse;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int totalPlayers = numPlayer + i;
            int expectedLoser = j;
            if(j >= totalPlayers) {
                continue;
            }
            
            
#if (NOISY_TEST == 1)
            printf("\tTesting with %d players:\n", totalPlayers);
            printf("\tTesting with player%d with card:%d:\n", expectedLoser+1, card);
#endif

            //make new game
            struct gameState G;
            r = initializeGame(totalPlayers, k, seed, &G);

            //if more than 2 players, then possibly more than one winner
            int expectedWinners = 0;
            if(totalPlayers > 2) 
            {
                for(int p = 0; p < totalPlayers; p++){
                    if(p != whoseTurn(&G)){
                        if(p != expectedLoser) {
                            expectedWinners++;
                        }
                    }
                }
            }
            else
            {
                expectedWinners = 1;
            }
#if (NOISY_TEST == 1)            
            printf("\tExpected Winners:%d\n",expectedWinners);
#endif           
            G.deck[expectedLoser][0] = card;

            beforeState = G;
            for(int p = 0; p < MAX_PLAYERS; p++){
                players[p] = 0;
            }

            int result = getWinners(players, &G);

            for(int p = 0; p < MAX_PLAYERS; p++){
#if (NOISY_TEST == 1)
                printf("\t\tPlayer%d: %d\n", p + 1, players[p]);
#endif
            }

            if(players[expectedLoser] == 1) 
            {
                totalTests++;
#if (NOISY_TEST == 1)
                printf("\tFAILED. Wrong winner chosen\n");
#endif
            }
            else 
            {
                int winners = 0;
                for(int p = 0; p < totalPlayers; p++) {
                    if(players[p] == 1)
                    {
                        winners++;
                    }
                }
                if(winners == expectedWinners)
                {
                    testsPassed++;
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tPASS. Chose winners\n");
#endif
                }
                else 
                {
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tFAILED. Did not select correct winners\n");
#endif
                }

            }
        }
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