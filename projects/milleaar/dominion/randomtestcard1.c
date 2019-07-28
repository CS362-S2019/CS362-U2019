/* -----------------------------------------------------------------------
 * Aaron Miller
 *
 * Testing baronEffect function
 * 
 * randomBaron: randomtestcard1.c dominion.o rngs.o
 *      gcc -o randomBaron -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "math.h"

#define NOISY_TEST 1

void assert(int cmp);
void randomizeGame(struct gameState *G, int estateBool, int *kingdomCards);
void testBaron(int choice, struct gameState *post);
int countEstates(int player, struct gameState *game);

void assert(int cmp) {
    if(cmp) 
    {
        printf("\t[  PASS  ]\n");
    } 
    else 
    {
        printf("\t[**FAIL**]\n");
    }
}

void randomizeGame(struct gameState *G, int estateBool, int *kingdomCards){

    int seed = floor(Random() * 1000);
    int numPlayers = floor(Random() * MAX_PLAYERS + 1);
    
    initializeGame(numPlayers, kingdomCards, seed, G);

    G->numPlayers = numPlayers;
    G->supplyCount[estate] = floor(Random() * 13); //estates set to range from 0 - 12
    G->whoseTurn = floor(Random() * G->numPlayers);
    int currentPlayer = whoseTurn(G);
    G->numBuys = floor(Random() * 100); //buys set to range from 0 to 99
    G->coins = floor(Random() * 100); //coins set to range from 0 to 99
    G->handCount[currentPlayer] = floor(Random() * MAX_HAND);
    for(int i = 0; i < G->handCount[currentPlayer]; i++) 
    {
        G->hand[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->deckCount[currentPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->deckCount[currentPlayer]; i++) 
    {
        G->deck[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->discardCount[currentPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->discardCount[currentPlayer]; i++) 
    {
        G->discard[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->playedCardCount = floor(Random() * 100); //played cards set to range from 0 to 99
    //editing amount of estates in hand
    //every 50 rounds, no estates in hand to improve coverage
    if (estateBool)
    {
        int handEstates = floor(Random() * G->handCount[currentPlayer]);
        for(int i = 0; i < handEstates; i++) 
        {
            G->hand[currentPlayer][i] = estate;
        }
    }

}

void testBaron(int choice1, struct gameState *post) {
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    int currentPlayer = whoseTurn(post);
    int result = baronEffect(choice1, post);

    int discard = 1, drawnCard = 1, extraCoins = 0;
    if(choice1) 
    {
        if(countEstates(currentPlayer,&pre) > 0) 
        {
            extraCoins = 4;
            drawnCard = 0;
        }
        else 
        {
            discard = 0;
            extraCoins = 0;
            drawnCard = 1;
        }
    }
    else 
    {
        discard = 0;
        extraCoins = 0;
        drawnCard = 1;
    }

    int preEstate = countEstates(currentPlayer, &pre) + drawnCard;
    int postEstate = countEstates(currentPlayer, post);

    printf("Testing with choice1 = %d, estates on hand = %d\n", choice1, preEstate);

    printf("Testing Buys: ");
    assert(post->numBuys == pre.numBuys + 1);
    printf("Expected: %d\t\tActual:%d\n\n", pre.numBuys + 1, post->numBuys);

    printf("Testing Coins: ");
    assert(post->coins == pre.coins + extraCoins);
    printf("Expected: %d\t\tActual:%d\n\n", pre.coins + extraCoins, post->coins);

    printf("Testing Hand Count: ");
    assert(post->handCount[currentPlayer] == pre.handCount[currentPlayer] + drawnCard - discard);
    printf("Expected: %d\t\tActual:%d\n\n", pre.handCount[currentPlayer] + drawnCard - discard, post->handCount[currentPlayer]);

    printf("Testing Discard Count: ");
    assert(post->discardCount[currentPlayer] == pre.discardCount[currentPlayer] + discard);
    printf("Expected: %d\t\tActual:%d\n\n", pre.discardCount[currentPlayer] + discard, post->discardCount[currentPlayer]);

    printf("Testing Played Cards Count: ");
    assert(post->playedCardCount == pre.playedCardCount + 1);
    printf("Expected: %d\t\tActual:%d\n\n", pre.playedCardCount + discard, post->playedCardCount);

    printf("Testing Estate Counts: ");
    assert(preEstate == postEstate);
    printf("Expected: %d\t\tActual:%d\n\n", preEstate, postEstate);

}

int countEstates(int player, struct gameState *G) {
  int numEstates = 0;
  for(int i = 0; i < G->handCount[player]; i++) 
  {
    if(G->hand[player][i] == estate) 
    {
      numEstates++;
    }
  }
  return numEstates;
}


int main () {

    int i, n, choice1;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
            remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("Testing baronEffect:\n");
    printf ("RANDOM TESTS.\n");

    SelectStream(2);
    PutSeed(3);

    for (n = 0; n < 20000; n++) 
    {
        for (i = 0; i < sizeof(struct gameState); i++) 
        {
            ((char*)&G)[i] = floor(Random() * 256);
        }
        //making 0 estates in hand every 50 rounds to improve coverage
        int estateBool = 1;
        if(n % 50) 
        {
            estateBool = 0;
        }
        randomizeGame(&G, estateBool, k);
        //choice1 is limited to 0 or 1 because there are only two choices
        //makes for more even distribution between yes and no values
        choice1 = floor(Random() * 2);
        testBaron(choice1, &G);
    }

    return 0;
}