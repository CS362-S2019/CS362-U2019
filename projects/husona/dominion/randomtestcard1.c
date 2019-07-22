#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int testPlayBaron(struct gameState *post)
{
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));

	int r;
	//  printf ("drawCard PRE: p %d HC %d DeC %d DiC %d\n",
	//	  p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
		
	r = drawCard (p, post);

	//printf ("drawCard POST: p %d HC %d DeC %d DiC %d\n",
	//      p, post->handCount[p], post->deckCount[p], post->discardCount[p]);

	if (pre.deckCount[p] > 0)
	{
		pre.handCount[p]++;
		pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
		pre.deckCount[p]--;
	}
	else if (pre.discardCount[p] > 0)
	{
		memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
		memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
		pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
		pre.handCount[p]++;
		pre.deckCount[p] = pre.discardCount[p]-1;
		pre.discardCount[p] = 0;
	}

	  assert (r == 0);

	  assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
}

int main () 
{
	int i, j, n, handPos, choice;
	struct gameState game, control;
	SelectStream(0);
	PutSeed(1);

	printf ("Random Testing for playBaron.\n");

	for (n = 0; n < 2000; n++)
	{
		for (i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&game)[i] = floor(Random() * 256);	// initialize all bytes of game to random number
		}
		game.numPlayers = floor(Random() * 5);	// make sure number of players is less than or equal to 4
		game.whoseTurn = floor(Random() * (game.numPlayers + 1))	// make sure whoseTurn is a valid playerNum
		for(i = 0; i < game.numPlayers; i ++)	// make sure all players have valid number of cards in deck/discard/hand
		{
			game.deckCount[i] = floor(Random() * MAX_DECK);
			for(j = 0; j < game.deckCount[i]; i++)
			{
				game.deck[i][j] = floor(Random() * (treasure_map + 1));	// make sure cards in deck are valid cards
			}
			game.discardCount[i] = floor(Random() * MAX_DECK);
			for(j = 0; j < game.discardCount[i]; i++)
			{
				game.discard[i][j] = floor(Random() * (treasure_map + 1));	// make sure cards in discard pile are valid cards
			}
			game.handCount[i] = floor(Random() * MAX_HAND);
			for(j = 0; j < game.handCount[i]; i++)
			{
				game.hand[i][j] = floor(Random() * (treasure_map + 1));	// make sure cards in hand are valid cards
			}
		}
		
		handPos = floor(Random() * (game.handCount[game.whoseTurn] + 1));	// make sure handPos within the players hand
		choice = floor(Random() * 2);	// make choice randomly 0 or 1, any choice other than 0 should have same 
		
		testPlayBaron(&game);
	}
	
	printf ("ALL TESTS OK\n");


	return 0;
}
