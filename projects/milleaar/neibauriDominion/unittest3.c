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
    int choice1, choice2, copies, r;
    int handPos = 0;
    int handCount = 5;
    int k[10] = {ambassador, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState beforeState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf ("TESTING ambassadorEffect:\n");
#endif

        //testing choice2 of amount of cards to discard
        for (int j = 0; j < 4; j++) {
            //testing amount of copies available of card chosen to discard
            for (int p = 0; p < 3; p++) {
    #if (NOISY_TEST == 1)
                printf("Testing with choice1 = %d, choice2 = %d, copies = %d.\n", 1, j, p);
    #endif
                choice1 = 1;
                choice2 = j;
                copies = p;

                //make new game
                struct gameState G;
                r = initializeGame(numPlayer, k, seed, &G);
                int currentPlayer = whoseTurn(&G);
                int nextPlayer = currentPlayer + 1;

                G.hand[currentPlayer][handPos] = ambassador;
                G.hand[currentPlayer][2] = copper;

                for(int z = 1; z <= copies; z++) {
                    G.hand[currentPlayer][z] = feast;
                    G.supplyCount[feast]--;
                }
                for(int z = 4; z > copies; z--){
                    G.hand[currentPlayer][z] = k[z + 4];
                }

                //printing hand contents
                // for(int handPos = 0; handPos < G.handCount[currentPlayer]; handPos++){
                //     printf("Card %d : %d\n", handPos, G.hand[currentPlayer][handPos]);	
                // }

                beforeState = G;
                int result = callAmbassador(choice1, choice2, &G, handPos, currentPlayer);

                if (result == -1) {
                    if(choice2 > 2 || choice2 < 0 || choice1 == handPos || copies <= choice2){
                        testsPassed++;
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tPASS. Bad parameter combination caught\n");
    #endif
                    }
                    else {
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tFAILED. Returned -1 but all parameters good\n");
    #endif                     
                    }
                    
                }
                else{
                    //making sure supply is affected as intended
                    //if pass, then other players received copy of card
                    if(G.supplyCount[choice1] == (beforeState.supplyCount[choice1] + choice2 - numPlayer)){
                        testsPassed++;
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tPASS. Supply decreased correctly\n");
    #endif
                    }
                    else {
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tFAILED. Supply not decreased correctly\n");
    #endif                     
                    }

                    //making sure played cards increased
                    if(G.playedCardCount > beforeState.playedCardCount){
                        testsPassed++;
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tPASS. Played cards increased\n");
    #endif
                    }
                    else{
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tFAILED. Played cards not increased\n");
    #endif              
                    }

                    //making sure cards are removed from current player
                    int beforeCopies = 0;
                    for(int z = 0; z < beforeState.handCount[currentPlayer]; z++) {
                        if(beforeState.hand[currentPlayer][choice1] == beforeState.hand[currentPlayer][z]){
                            beforeCopies++;
                        }
                    }
                    int afterCopies = 0;
                    for(int z = 0; z < G.handCount[currentPlayer]; z++) {
                        if(beforeState.hand[currentPlayer][choice1] == G.hand[currentPlayer][z]){
                            afterCopies++;
                        }
                    }
                    if(beforeCopies - afterCopies == choice2) {
                        testsPassed++;
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tPASS. Cards were discarded\n");
    #endif
                    }
                    else{
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tFAILED. Wrong amount of cards discarded\n");
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
