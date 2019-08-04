/* -----------------------------------------------------------------------
 * Aaron Miller
 *
 * Testing tributeEffect function
 * 
 * randomTribute: randomtestcard3.c dominion.o rngs.o
 *      gcc -o randomTribute -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)
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
void randomizeGame(struct gameState *G, int *kingdomCards, int coinCards);
void testTribute(struct gameState *G);

void assert(int cmp) {
    if(cmp) 
    {
        printf("\t\t[  PASS  ]\n");
    } 
    else 
    {
        printf("\t\t[**FAIL**]\n");
    }
}

void randomizeGame(struct gameState *G, int *kingdomCards, int coinCards){

    int seed = floor(Random() * 1000);
    int numPlayers = floor(Random() * MAX_PLAYERS + 1);
    
    initializeGame(numPlayers, kingdomCards, seed, G);

    G->numPlayers = numPlayers;
    G->supplyCount[estate] = floor(Random() * 13); //estates set to range from 0 - 12
    G->whoseTurn = floor(Random() * G->numPlayers);

    int currentPlayer = whoseTurn(G);
    int nextPlayer = currentPlayer + 1;
    if(nextPlayer == G->numPlayers) 
    {
        nextPlayer = 0;
    }

    G->numActions = floor(Random() * 100); //buys set to range from 0 to 99
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
    int handEstates = floor(Random() * G->handCount[currentPlayer]);
    for(int i = 0; i < handEstates; i++) 
    {
      G->hand[currentPlayer][i] = estate;
    }
    int handPos = floor(Random() * G->handCount[currentPlayer] + 1);
    G->hand[currentPlayer][handPos] = tribute;

    //editing next player's hand
    G->handCount[nextPlayer] = floor(Random() * MAX_HAND);
    for(int i = 0; i < G->handCount[nextPlayer]; i++) 
    {
        G->hand[nextPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->deckCount[nextPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->deckCount[nextPlayer]; i++) 
    {
        G->deck[nextPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->discardCount[nextPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->discardCount[nextPlayer]; i++) 
    {
        G->discard[nextPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    //editing amount of coins in hand
    for(int i = 0; i < coinCards; i++) 
    {
      G->hand[nextPlayer][i] = copper;
    }
}

void testTribute(struct gameState *post) {
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    int currentPlayer = whoseTurn(post);
    int nextPlayer = currentPlayer + 1;
    if(nextPlayer == pre.numPlayers) 
    {
        nextPlayer = 0;
    }

    int tributeRevealedCards[2] = {pre.hand[nextPlayer][0], pre.hand[nextPlayer][1]};
    int result = callTribute(post, currentPlayer, nextPlayer, tributeRevealedCards);

    int actions = 0, extraCoins = 0, drawnCards = 0, nextDiscard = 2;
    

    if (tributeRevealedCards[0] == tributeRevealedCards[1])
    {
      tributeRevealedCards[1] = -1;
    }

    for (int i = 0; i < 2; i++)
    {
      if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold)
      {
          //Treasure cards
          extraCoins += 2;
      }
            
      else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province
      || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall)
      {
          //Victory Card Found
          drawnCards += 2;
      }
      else if (tributeRevealedCards[i] > 1) 
      {
          //Action Card
          actions += 2;
      }
    }

    
    printf("Testing with revealed cards: %d and %d\n", tributeRevealedCards[0], tributeRevealedCards[1]);
    printf("Testing Actions: ");
    assert(post->numActions == pre.numActions + actions);
    printf("Expected: %d\t\tActual:%d\n\n", pre.numActions + actions, post->numActions);

    printf("Testing Coins: ");
    assert(post->coins == pre.coins + extraCoins);
    printf("Expected: %d\t\tActual:%d\n\n", pre.coins + extraCoins, post->coins);

    printf("Testing Hand Count: ");
    assert(post->handCount[currentPlayer] == pre.handCount[currentPlayer] + drawnCards);
    printf("Expected: %d\t\tActual:%d\n\n", pre.handCount[currentPlayer] + drawnCards, post->handCount[currentPlayer]);

    printf("Testing Next Player Discard Count: ");
    assert(post->discardCount[nextPlayer] == pre.discardCount[nextPlayer] + nextDiscard);
    printf("Expected: %d\t\tActual:%d\n\n", pre.discardCount[nextPlayer] + nextDiscard, post->discardCount[nextPlayer]);

}

int main () {

    int i, n;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
            remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("Testing minionEffect:\n");
    printf ("RANDOM TESTS.\n");

    SelectStream(2);
    PutSeed(3);

    for (n = 0; n < 20000; n++) 
    {
        for (i = 0; i < sizeof(struct gameState); i++) 
        {
            ((char*)&G)[i] = floor(Random() * 256);
        }
        //every 33 games, force a coin card to appear in hand
        //every 100 games, force two coin cards to appear
        int coinCards = 0;
        if(n % 33 == 0) 
        {
            coinCards = 1;
        }
        if(n % 100 == 0) 
        {
            coinCards = 2;
        }
        randomizeGame(&G, k, coinCards);
        testTribute(&G);
    }


    return 0;
}