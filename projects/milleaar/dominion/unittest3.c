/* -----------------------------------------------------------------------
 * Aaron Miller
 *
 * Testing ambassadorEffect function
 * 
 * testAmbassador: unittest3.c dominion.o rngs.o
 *      gcc -o testAmbassador -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState beforeState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf ("TESTING baronEffect:\n");
#endif

    //"i" will make the choice1 = 0 or 1
    //testing both possibilities
    for (int i = 0; i < 2; i++) {
#if (NOISY_TEST == 1)
        printf("Testing with choice1 = %d.\n", i);
#endif
        //testing estate in different places in hand
        //final iteration of loop tests with no estate
        for (int j = 0; j < (handCount + 1); j++) {
#if (NOISY_TEST == 1)
            if(j < handCount){
                printf("Testing with estate in hand position = %d.\n", j);
            }
            else {
                printf("Testing estate not placed in hand\n");
            }
#endif

            choice1 = i;

            //make new game
            struct gameState G;
            r = initializeGame(numPlayer, k, seed, &G);
            int currentPlayer = whoseTurn(&G);

            for(int p = 0; p < handCount; p++) {
                if(p == j) {
                    G.hand[currentPlayer][p] = estate;
                }
                else{
                    G.hand[currentPlayer][p] = baron;
                }
            }
            //printing hand contents
            // for(int handPos = 0; handPos < G.handCount[currentPlayer]; handPos++){
            //     printf("Card %d : %d\n", handPos, G.hand[currentPlayer][handPos]);	
            // }


            beforeState = G;
            int result = baronEffect(choice1, &G);

            if(choice1) {
                //chose to discard estate for treasure
                if( j != handCount){
                    if ( (G.coins - beforeState.coins) == 4 ){
                        testsPassed++;
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tPASS. Coins increased\n");
#endif
                    }
                    else {
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tFAILED. Coins not increased\n");
#endif                     
                    }

                    if(beforeState.discardCount[currentPlayer] + 1 == G.discardCount[currentPlayer]) {
                        testsPassed++;
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tPASS. Estate discarded\n");
#endif
                    }
                    else {
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tFAILED. Estate not discarded\n");
#endif     
                    }
                }
                else if ( j == handCount) {
                    //chose to discard estate but did not have an estate card
                    if( G.hand[currentPlayer][G.handCount[currentPlayer]] == estate ) {
                        testsPassed++;
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tPASS. Picked up estate\n");
#endif
                    }
                    else{
                        totalTests++;
#if (NOISY_TEST == 1)
                        printf("\tFAILED. Did not pickup estate\n");
#endif
                    }
                }
                //else test failed for choosing to discard estate
                else {
#if (NOISY_TEST == 1)
                    printf("\tINTERNAL TEST FAILURE WITH ASSIGNING VARIABLES\n");
#endif
                }
            }
            else {
                //chose to gain estate
                if( (beforeState.supplyCount[estate] - 1) == G.supplyCount[estate]) {
                    testsPassed++;
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tPASS. Supply decremented\n");
#endif
                }
                else{
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tFAILED. Supply not decremented\n");
#endif
                }

                if ( G.hand[currentPlayer][G.handCount[currentPlayer]] == estate) {
                    testsPassed++;
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tPASS. Picked up estate\n");
#endif
                }
                else{
                    totalTests++;
#if (NOISY_TEST == 1)
                    printf("\tFAILED. Did not pickup estate\n");
#endif
                }



            }


        }

    }

    float percentPass = (testsPassed / totalTests) * 100.0;

#if (NOISY_TEST == 1)
    if(testsPassed == totalTests){
        printf("ALL TESTS PASSED\n");
    }
    else{
        printf("%3.2f of Tests Passed.\n", percentPass);
    }
#endif

    return 0;
}
