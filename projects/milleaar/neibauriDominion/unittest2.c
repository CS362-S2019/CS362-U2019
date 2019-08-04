/* -----------------------------------------------------------------------
 * Aaron Miller
 *
 * Testing minionEffect function
 * 
 * testMinion: unittest2.c dominion.o rngs.o
 *      gcc -o testMinion -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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
    int choice1, choice2, r, handCount;
    int handPos = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, minion, great_hall};
    struct gameState beforeState;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf ("TESTING minionEffect:\n");
#endif

    //"i" will make the choice1 = 0 or 1
    //testing both possibilities
    for (int i = 0; i < 2; i++) {

        //"j" will make the choice2 = 0 or 1
        //testing both possibilities
        for (int j = 0; j < 2; j++) {

            //testing both possibilities of <5 cards and 5 cards
            //for other players
            for(int p = 0; p < 2; p++){

                if(p == 0) {
                    handCount = 4;
                }
                else {
                    handCount = 5;
                }
                choice1 = i;
                choice2 = j;
#if (NOISY_TEST == 1)
        printf("Testing with choice1 = %d, choice2 = %d, and handCount = %d.\n", i, j, handCount);
#endif
                //make new game
                struct gameState G;
                r = initializeGame(numPlayer, k, seed, &G);
                int currentPlayer = whoseTurn(&G);
                int nextPlayer = currentPlayer + 1;

                G.hand[currentPlayer][handPos] = minion;

                //Moved draw cards to here, only drawing at the start of a turn
                for (int it = 0; it < handCount; it++){
                    drawCard(nextPlayer, &G);
                }

                //printing hand contents
                for(int handPos = 0; handPos < G.handCount[currentPlayer]; handPos++){
                    printf("CP Card %d : %d\n", handPos, G.hand[currentPlayer][handPos]);	
                }

                //printing hand contents of next player
                for(int handPos = 0; handPos < G.handCount[nextPlayer]; handPos++){
                    printf("NP Card %d : %d\n", handPos, G.hand[nextPlayer][handPos]);	
                }

                beforeState = G;
                int result = callMinion(choice1, choice2, &G, handPos, currentPlayer);

                if(choice1) {
                    //chose to discard for treasure
                    if ( (G.coins - beforeState.coins) == 2 ){
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
                }

                else if(choice2) {
                    //check discard count of current player
                    //subtracting 1 since discarding minion card at start
                    if(beforeState.handCount[currentPlayer] == (G.handCount[currentPlayer]) ) {
                        int new = 0;
                        for(int z = 0; z < beforeState.handCount[currentPlayer]; z++){
                            if( beforeState.hand[currentPlayer][z] != G.hand[currentPlayer][z]){
                                new = 1;
                            }
                        }
                        if( new ) {
                            testsPassed++;
                            totalTests++;
        #if (NOISY_TEST == 1)
                            printf("\tPASS. Discarded current player's cards\n");
        #endif
                        }
                        else {
                            totalTests++;
        #if (NOISY_TEST == 1)
                            printf("\tFAILED. Current player's cards not discarded\n");
        #endif                     
                        }
                    }
                    else {
                        //if the hand counts don't match, then cards discarded
                        testsPassed++;
                        totalTests++;
        #if (NOISY_TEST == 1)
                        printf("\tPASS. Discarded current player's cards\n");
        #endif
                    }
                    

                    //check card count of current player
                    if(G.handCount[currentPlayer] == 4 ) {
                        testsPassed++;
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tPASS. Current player drew 4 cards\n");
    #endif
                    }
                    else {
                        totalTests++;
    #if (NOISY_TEST == 1)
                        printf("\tFAILED. Current player did not draw 4\n");
    #endif                     
                    }

                    //check if next player discarded
                    if(handCount == 4) {
                        if(beforeState.handCount[nextPlayer] == (G.handCount[nextPlayer]) ) {
                            int new = 0;
                            for(int z = 0; z < beforeState.handCount[nextPlayer]; z++){
                                if( beforeState.hand[nextPlayer][z] != G.hand[nextPlayer][z]){
                                    new = 1;
                                }
                            }
                            if( !new ) {
                                testsPassed++;
                                totalTests++;
            #if (NOISY_TEST == 1)
                                printf("\tPASS. Did not discard next player's cards\n");
            #endif
                            }
                            else {
                                totalTests++;
            #if (NOISY_TEST == 1)
                                printf("\tFAILED. Next player's cards discarded\n");
            #endif                     
                            }
                        }
                        else {
                            //if the hand counts don't match, then cards discarded
                            totalTests++;
            #if (NOISY_TEST == 1)
                            printf("\tFAILED. Discarded next player's cards\n");
            #endif
                        }

                    }
                    //else handCount is 5
                    else {
                        if(beforeState.handCount[nextPlayer] == (G.handCount[nextPlayer]) ) {
                            int new = 0;
                            for(int z = 0; z < beforeState.handCount[nextPlayer]; z++){
                                if( beforeState.hand[nextPlayer][z] != G.hand[nextPlayer][z]){
                                    new = 1;
                                }
                            }
                            if( new ) {
                                testsPassed++;
                                totalTests++;
            #if (NOISY_TEST == 1)
                                printf("\tPASS. Discarded next player's cards\n");
            #endif
                            }
                            else {
                                totalTests++;
            #if (NOISY_TEST == 1)
                                printf("\tFAILED. Next player's cards not discarded\n");
            #endif                     
                            }
                        }
                        else {
                            //if the hand counts don't match, then cards discarded
                            testsPassed++;
                            totalTests++;
            #if (NOISY_TEST == 1)
                            printf("\tPASS. Discarded next player's cards\n");
            #endif
                        }

                    }

                    
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