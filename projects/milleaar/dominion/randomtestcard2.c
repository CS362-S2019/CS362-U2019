/* -----------------------------------------------------------------------
 * Aaron Miller
 *
 * Testing minionEffect function
 * 
 * randomMinion: randomtestcard2.c dominion.o rngs.o
 *      gcc -o randomMinion -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
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
void randomizeGame(struct gameState *G, int *kingdomCards, int *handPos);
void testMinion(int choice1, int choice2, struct gameState *post, int handPos);
int testPlayer(int player, struct gameState *pre, struct gameState *post);

void assert(int cmp) {
  if(cmp) 
  {
    printf("\t\t[  PASS  ]\n");
  } else 
  {
    printf("\t\t[**FAIL**]\n");
  }
}

void randomizeGame(struct gameState *G, int *kingdomCards, int* handPos){

    int seed = floor(Random() * 1000);
    int numPlayers = floor(Random() * MAX_PLAYERS + 1);
    
    initializeGame(numPlayers, kingdomCards, seed, &G);

    G->numPlayers = numPlayers;
    G->supplyCount[estate] = floor(Random() * 13); //estates set to range from 0 - 12
    G->whoseTurn = floor(Random() * G->numPlayers);
    int currentPlayer = whoseTurn(&G);
    G->numActions = floor(Random() * 100); //buys set to range from 0 to 99
    G->handCount[currentPlayer] = floor(Random() * MAX_HAND);
    for(int i = 0; i < G->handCount[currentPlayer]; i++) {
        G->hand[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->deckCount[currentPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->deckCount[currentPlayer]; i++) {
        G->deck[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->discardCount[currentPlayer] = floor(Random() * MAX_DECK);
    for(int i = 0; i < G->discardCount[currentPlayer]; i++) {
        G->discard[currentPlayer][i] = kingdomCards[ (int) floor(Random() * 10) ];
    }
    G->playedCardCount = floor(Random() * 100); //played cards set to range from 0 to 99
    //editing amount of estates in hand
    int handEstates = floor(Random() * G->handCount[currentPlayer]);
    for(int i = 0; i < handEstates; i++) {
      G->hand[currentPlayer][i] = estate;
    }

    (*handPos) = floor(Random() * G->handCount[currentPlayer] + 1);
    G->hand[currentPlayer][(*handPos)] = minion;
    
}

void testMinion(int choice1, int choice2, struct gameState *post, int handPos) {
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    int currentPlayer = whoseTurn(post);
    int result = minionEffect(choice1, choice2, post, handPos);

    int discard = 1, extraCoins = 0, hand;
    if(choice1) 
    {
        extraCoins = 2;
        hand = pre.handCount[currentPlayer];
    }
    else if (choice2)
    {
        discard = pre.handCount[currentPlayer];
        hand = 4;
    }

    printf("Testing Actions: ");
    assert(post->numActions == pre.numActions + 1);
    printf("Expected: %d\t\tActual:%d\n\n", pre.numActions + 1, post->numActions);

    printf("Testing Coins: ");
    assert(post->coins == pre.coins + extraCoins);
    printf("Expected: %d\t\tActual:%d\n\n", pre.coins + extraCoins, post->coins);

    printf("Testing Hand Count: ");
    assert(post->handCount[currentPlayer] == hand);
    printf("Expected: %d\t\tActual:%d\n\n", hand, post->handCount[currentPlayer]);

    printf("Testing Discard Count: ");
    assert(post->discardCount[currentPlayer] == pre.discardCount[currentPlayer] + discard);
    printf("Expected: %d\t\tActual:%d\n\n", pre.discardCount[currentPlayer] + discard, post->discardCount[currentPlayer]);

    printf("Testing Other Players:\n ");
    for (int i = 0; i < pre.numPlayers; i++) 
    {
        int changeExpected = 0;
        if(i == currentPlayer) 
        {
            continue;
        }
        if(pre.handCount[i] >= 5)
        {
            if(!choice1 && choice2) 
            {
                changeExpected = 1;
            }
        }
        printf("Testing Player %d: ", i + 1);
        int actual = testPlayer(i, &pre, post);
        assert(changeExpected == actual);
        printf("Expected: %d\t\tActual:%d\n\n", changeExpected, actual);
    }
}

int testPlayer(int player, struct gameState *pre, struct gameState *post) {
  int results = 0;
  
  //check hand
  if(pre->handCount[player] != post->handCount[player]) 
  {
    results = 1;
  }
  
  for(int i = 0; i < pre->handCount[player]; i++) 
  {
    if(pre->hand[player][i] != post->hand[player][i]) 
    {
      results = 1;
    }
  }
  //check deck
  if(pre->deckCount[player] != post->deckCount[player]) 
  {
    results = 1;
  }

  for(int i = 0; i < pre->deckCount[player]; i++) 
  {
    if(pre->deck[player][i] != post->deck[player][i]) 
    {
      results = 1;
    }
  }
  //check discard
  if(pre->discardCount[player] != post->discardCount[player]) 
  {
    results = 1;
  }

  for(int i = 0; i < pre->discardCount[player]; i++) 
  {
    if(pre->discard[player][i] != post->discard[player][i]) 
    {
      results = 1;
    }
  }

  return results;
}


int main () {

  int i, n, choice1, choice2, handPos;
  

  int k[10] = {adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, great_hall};

  struct gameState G;

  printf ("Testing minionEffect:\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
    randomizeGame(&G, k, &handPos);
    //choice is limited to 0 or 1 because they are boolean values
    //makes for more even distribution between yes and no values
    choice1 = floor(Random() * 2);
    choice2 = floor(Random() * 2);
    testMinion(choice1, choice2, &G, handPos);
  }


  return 0;
}