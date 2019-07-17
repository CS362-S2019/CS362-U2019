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

void duplicateTreasure(struct gameState *before, struct gameState *G, float *passed, float *total){
        #if (NOISY_TEST == 1)
            printf ("Testing duplicate named Treasure cards:\n");
        #endif
            if(before->coins + 2 == G->coins) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Coins increased by 2 for duplicate treasure\n");
                printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif
            }
            else {
                if(before->coins + 4 == G->coins) {
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not account for duplicate name\n");
                    printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif    
                }
                else{
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not add coins correctly\n");
                    printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif                     
                }
            }
}

void duplicateVictory(struct gameState *before, struct gameState *G, float *passed, float *total){
        int currentPlayer = whoseTurn(before);

        #if (NOISY_TEST == 1)
            printf ("Testing duplicate named Victory cards:\n");
        #endif
            if(before->handCount[currentPlayer] + 2 == G->handCount[currentPlayer]) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Cards increased by 2 for duplicate victory\n");
                printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif
            }
            else {
                if(before->handCount[currentPlayer] + 4 == G->handCount[currentPlayer]) {
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not account for duplicate name\n");
                    printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif    
                }
                else{
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not add cards correctly\n");
                    printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif                     
                }
            }
}

void duplicateAction(struct gameState *before, struct gameState *G, float *passed, float *total){

        #if (NOISY_TEST == 1)
            printf ("Testing duplicate named Action cards:\n");
        #endif
            if(before->numActions + 2 == G->numActions) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Actions increased by 2 for duplicate action\n");
                printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif
            }
            else {
                if(before->numActions + 4 == G->numActions) {
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not account for duplicate name\n");
                    printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->numActions, G->numActions);
        #endif    
                }
                else{
                    (*total)++;
        #if (NOISY_TEST == 1)
                    printf("\t\tFAILED. Did not add actions correctly\n");
                    printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif                     
                }
            }
}

void treasureVictory(struct gameState *before, struct gameState *G, float *passed, float *total){
        int currentPlayer = whoseTurn(before);

        #if (NOISY_TEST == 1)
            printf ("Testing Treasure and Victory cards:\n");
        #endif

        #if (NOISY_TEST == 1)
            printf ("\tTreasure:\n");
        #endif
        if(before->coins + 2 == G->coins) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Coins increased by 2\n");
                printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif
        }
        else{
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add coins correctly\n");
                printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif                     
        }

        #if (NOISY_TEST == 1)
            printf ("\tVictory:\n");
        #endif
        if(before->handCount[currentPlayer] + 2 == G->handCount[currentPlayer]) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Cards increased by 2\n");
                printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif
        }
        else {
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add cards correctly\n");
                printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif                     
        }
            
}

void treasureAction(struct gameState *before, struct gameState *G, float *passed, float *total){

        #if (NOISY_TEST == 1)
            printf ("Testing Treasure and Action cards:\n");
        #endif

        #if (NOISY_TEST == 1)
            printf ("\tTreasure:\n");
        #endif
        if(before->coins + 2 == G->coins) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Coins increased by 2\n");
                printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif
        }
        else{
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add coins correctly\n");
                printf("\t\tBefore Coins: %d\tAfter Coins: %d\n",before->coins, G->coins);
        #endif                     
        }

        #if (NOISY_TEST == 1)
            printf ("\tAction:\n");
        #endif
        if(before->numActions + 2 == G->numActions) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Actions increased by 2\n");
                printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif
        }
        else {
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add actions correctly\n");
                printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif                     
                
        }


}

void victoryAction(struct gameState *before, struct gameState *G, float *passed, float *total){
        int currentPlayer = whoseTurn(before);

        #if (NOISY_TEST == 1)
            printf ("Testing Victory and Action cards:\n");
        #endif
        #if (NOISY_TEST == 1)
            printf ("\tVictory:\n");
        #endif
        if(before->handCount[currentPlayer] + 2 == G->handCount[currentPlayer]) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Cards increased by 2\n");
                printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif
        }
        else {
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add cards correctly\n");
                printf("\t\tBefore Card Count: %d\tAfter Card Count: %d\n",before->handCount[currentPlayer], G->handCount[currentPlayer]);
        #endif                     
        }

        #if (NOISY_TEST == 1)
            printf ("\tAction:\n");
        #endif
        if(before->numActions + 2 == G->numActions) {
                (*passed)++;
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Actions increased by 2\n");
                printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif
        }
        else {
                (*total)++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not add actions correctly\n");
                printf("\t\tBefore Action Count: %d\tAfter Action Count: %d\n",before->numActions, G->numActions);
        #endif                     
                
        }
        
}

int main() {

    int seed = 1000;
    int numPlayer = 2;
    int r;
    int handPos = 0;
    int handCount = 5;
    int k[10] = {tribute, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState beforeState;
    int cardTest1, cardTest2;

    float testsPassed = 0;
    float totalTests = 0;

#if (NOISY_TEST == 1)
    printf ("TESTING tributeEffect:\n");
#endif

    //testing possible combinations of different cards
    //i determines which card combnination to use
    for(int i = 0; i < 6; i++){

            //make new game
            struct gameState G;
            r = initializeGame(numPlayer, k, seed, &G);
            int currentPlayer = whoseTurn(&G);
            int nextPlayer = currentPlayer + 1;

            G.hand[currentPlayer][handPos] = tribute;

            //Moved draw cards to here, only drawing at the start of a turn
            for (int it = 0; it < handCount; it++){
                drawCard(nextPlayer, &G);
            }
            //fill deck with duplicate cards
            switch ( i ) {
                case 0:
                    cardTest1 = copper;
                    cardTest2 = copper;
                    break;
                case 1:
                    cardTest1 = estate;
                    cardTest2 = estate;
                    break;
                case 2:
                    cardTest1 = tribute;
                    cardTest2 = tribute;
                    break;
                case 3:
                    cardTest1 = copper;
                    cardTest2 = estate;
                    break;
                case 4:
                    cardTest1 = copper;
                    cardTest2 = tribute;
                    break;
                case 5:
                    cardTest1 = estate;
                    cardTest2 = tribute;
            }

            for(int j = 0; j < G.deckCount[nextPlayer]; j++){         
                if(j % 2 == 0) {
                    G.deck[nextPlayer][j] = cardTest1;
                } 
                else {
                    G.deck[nextPlayer][j] = cardTest2;
                }
            }
            

            beforeState = G;

            int result = tributeEffect(currentPlayer, nextPlayer, &G);

            //switch doing test for each combination
            switch ( i ) {
                case 0:
                    duplicateTreasure(&beforeState, &G, &testsPassed, &totalTests);
                    break;
                case 1:
                    duplicateVictory(&beforeState, &G, &testsPassed, &totalTests);
                    break;
                case 2:
                    duplicateAction(&beforeState, &G, &testsPassed, &totalTests);
                    break;
                case 3:
                    treasureVictory(&beforeState, &G, &testsPassed, &totalTests);
                    break;
                case 4:
                    treasureAction(&beforeState, &G, &testsPassed, &totalTests);
                    break;
                case 5:
                    victoryAction(&beforeState, &G, &testsPassed, &totalTests);
            }

            //testing deck count of next player
        #if (NOISY_TEST == 1)
            printf ("Testing Deck Counts:\n");
        #endif
            if(beforeState.deckCount[nextPlayer] - 2 == G.deckCount[nextPlayer]) {
                testsPassed++;
                totalTests++;
        #if (NOISY_TEST == 1)
                printf("\t\tPASS. Next player discarded two cards\n");
                printf("\t\tBefore Deck Count: %d\tAfter Deck Count: %d\n",beforeState.deckCount[nextPlayer], G.deckCount[nextPlayer]);
        #endif
            }
            else {
                totalTests++;
        #if (NOISY_TEST == 1)
                printf("\t\tFAILED. Did not remove cards correctly\n");
                printf("\t\tBefore Deck Count: %d\tAfter Deck Count: %d\n",beforeState.deckCount[nextPlayer], G.deckCount[nextPlayer]);
        #endif                     
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
