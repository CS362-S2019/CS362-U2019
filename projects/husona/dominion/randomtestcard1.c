#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

#define DEBUG 0

int testPlayBaron(struct gameState* post, int handPos, int choice)
{
	static int n = 1;
	int i, j, r, w, x = -1, code = 0;
	struct gameState pre;
	
	memcpy (&pre, post, sizeof(struct gameState));
	if(findCard(&pre, estate) != -1)
	{
		x = 1;
	}
	memcpy (&pre, post, sizeof(struct gameState));	// recopy gamestate to ensure above line did not affect state
	w = pre.whoseTurn;
	
	r = playBaron(post, handPos, choice);

	if(r != 0)
	{
		fprintf(stdout, "Test run %d for playBaron caused return value %d, inputs: choice %d, hadEstate %d, handPos %d, players %d, whoseTurn %d\n", n, r, choice, x, handPos, pre.numPlayers, w);
		code = -1;
	}
	// If return value is not 0 then gamestate should be completely unchanged
	else
	{
		pre.numBuys++;	// increase number of buys by 1
		pre.discardCount[pre.whoseTurn]++;	// baron card is discarded
		pre.handCount[pre.whoseTurn]--;		// baron is removed from hand when discarded
		
		if ((choice == 1)  && (findCard(&pre, estate) != -1)) // if chose to discard and has an estate
		{
			pre.coins += 4;	// coin increased by 4
			pre.discardCount[pre.whoseTurn]++;	// estate card is discarded
			pre.handCount[pre.whoseTurn]--;		// estate card is removed from hand when discarded
		}
		else	// player choice to gain estate (or attempted to discard with no estate)
		{
			if(pre.supplyCount[estate] > 0)	// If there was supply
			{
				pre.supplyCount[estate]--;		// supply decreases
				pre.discardCount[pre.whoseTurn]--;	// estate card was added to the players discard pile
			}
		}
		memcpy(&(pre.discard[pre.whoseTurn]), &(post->discard[pre.whoseTurn]), sizeof(int) * MAX_DECK);	// Discard should have changed for this player however exact change unknown
		memcpy(&(pre.hand[pre.whoseTurn]), &(post->hand[pre.whoseTurn]), sizeof(int) * MAX_HAND);	// Hand should have changed for this player however exact change unknown
		memcpy(&(pre.playedCards), &(post->playedCards), sizeof(int) * MAX_DECK);	// (possibly?) should have changed however exact change unknown
		pre.playedCardCount = post->playedCardCount;	// (possibly?) should have changed however exact change unknown
	}
	
	// If the pre and post do not match after pre is modified to expected output, display error with all unexpected changes shown
	if(memcmp(&pre, post, sizeof(struct gameState)) != 0)
	{
		fprintf(stdout, "Test run %d for playBaron caused unexpected result. inputs: choice %d, hadEstate %d, handPos %d, players %d, whoseTurn %d\n", n, choice, x, handPos, pre.numPlayers, w);
		fprintf(stdout, "The following variables changed unexpectedly:\n");
		if(pre.numPlayers != post->numPlayers)
		{
			fprintf(stdout,"\tnumPlayers: expected %d, result %d\n", pre.numPlayers, post->numPlayers);
		}
		for(i = 0; i < treasure_map + 1; i ++)
		{
			if(pre.supplyCount[i] != post->supplyCount[i])
			{
				fprintf(stdout,"\tsupplyCount of card [%d]: expected %d, result %d\n", i, pre.supplyCount[i], post->supplyCount[i]);
			}
			if(pre.embargoTokens[i] != post->embargoTokens[i])
			{
				fprintf(stdout,"\tembargoTokens for card [%d]: expected %d, result %d\n", i, pre.embargoTokens[i], post->embargoTokens[i]);
			}
		}
		if(pre.outpostPlayed != post->outpostPlayed)
		{
			fprintf(stdout,"\toutpostPlayed: expected %d, result %d\n", pre.outpostPlayed, post->outpostPlayed);
		}
		if(pre.outpostTurn != post->outpostTurn)
		{
			fprintf(stdout,"\toutpostTurn: expected %d, result %d\n", pre.outpostTurn, post->outpostTurn);
		}
		if(pre.whoseTurn != post->whoseTurn)
		{
			fprintf(stdout,"\twhoseTurn: expected %d, result %d\n", pre.whoseTurn, post->whoseTurn);
		}
		if(pre.phase != post->phase)
		{
			fprintf(stdout,"\tphase: expected %d, result %d\n", pre.phase, post->phase);
		}
		if(pre.numActions != post->numActions)
		{
			fprintf(stdout,"\tnumActions: expected %d, result %d\n", pre.numActions, post->numActions);
		}
		if(pre.coins != post->coins)
		{
			fprintf(stdout,"\tcoins: expected %d, result %d\n", pre.coins, post->coins);
		}
		if(pre.numBuys != post->numBuys)
		{
			fprintf(stdout,"\tnumBuys: expected %d, result %d\n", pre.numBuys, post->numBuys);
		}
		if(pre.playedCardCount != post->playedCardCount)
		{
			fprintf(stdout,"\tplayedCardCount: expected %d, result %d\n", pre.playedCardCount, post->playedCardCount);
		}
		for(i = 0; i < MAX_PLAYERS; i++)
		{
			if(pre.handCount[i] != post->handCount[i])
			{
				fprintf(stdout,"\thandCount[%d]: expected %d, result %d\n", i, pre.handCount[i], post->handCount[i]);
			}
			if(pre.deckCount[i] != post->deckCount[i])
			{
				fprintf(stdout,"\tdeckCount[%d]: expected %d, result %d\n", i, pre.deckCount[i], post->deckCount[i]);
			}
			if(pre.discardCount[i] != post->discardCount[i])
			{
				fprintf(stdout,"\tdiscardCount[%d]: expected %d, result %d\n", i, pre.discardCount[i], post->discardCount[i]);
			}
			for(j = 0; j < MAX_HAND; j++)
			{
				if(pre.hand[i][j] != post->hand[i][j])
				{
					fprintf(stdout,"\thand[%d][%d]: expected %d, result %d\n", i, j, pre.hand[i][j], post->hand[i][j]);
				}
			}
			for(j = 0; j < MAX_DECK; j++)
			{
				if(pre.deck[i][j] != post->deck[i][j])
				{
					fprintf(stdout,"\tdeck[%d][%d]: expected %d, result %d\n", i, j, pre.deck[i][j], post->deck[i][j]);
				}
				if(pre.discard[i][j] != post->discard[i][j])
				{
					fprintf(stdout,"\tdiscard[%d][%d]: expected %d, result %d\n", i, j, pre.discard[i][j], post->discard[i][j]);
				}
			}
			
		}
		for(i = 0; i < MAX_DECK; i++)
		{
			if(pre.playedCards[i] != post->playedCards[i])
			{
				fprintf(stdout,"\tplayedCards[%d]: expected %d, result %d\n", i, pre.playedCards[i], post->playedCards[i]);
			}
			
		}
		fprintf(stdout, "End of report for test %d.\n", n);
		code = -1;
	}
	  
	  n++;	// increase the testcount
	  return code;
}

int main () 
{
	srand(time(0)); 
	int i, j, n, handPos, choice, failures = 0;
	struct gameState game;


	fprintf (stdout, "Random Testing for playBaron.\n");

	for (n = 0; n < 2000; n++)
	{
		for (i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&game)[i] = (char)(rand() % 256);	// initialize all bytes of game to random number
		}
		game.numPlayers = ((rand() % 3) + 2);	// make sure number of players is less than or equal to 4 but at least 2
		game.whoseTurn = (rand() % game.numPlayers);	// make sure whoseTurn is a valid playerNum
		for(i = 0; i < game.numPlayers; i++)		// make sure for all players deck + hand + discard < MAX_DECK
		{
			game.deckCount[i] = (rand() % (MAX_DECK/3));	// make sure all players have valid number of cards in deck
			
			for(j = 0; j < game.deckCount[i]; j++)
			{
				game.deck[i][j] = (rand() % (treasure_map + 1));	// make sure cards in deck are valid cards
			}
			
			game.discardCount[i] = (rand() % (MAX_DECK/3));		// make sure all players have valid number of cards in discard
			for(j = 0; j < game.discardCount[i]; j++)
			{
				game.discard[i][j] = (rand() % (treasure_map + 1));	// make sure cards in discard pile are valid cards
			}
			
			game.handCount[i] = (rand() % (MAX_HAND/3));	// make sure all players have valid number of cards in hand
			for(j = 0; j < game.handCount[i]; j++)
			{
				game.hand[i][j] = (rand() % (treasure_map + 1));	// make sure cards in hand are valid cards
			}
		}
		game.playedCardCount = (rand() % (MAX_HAND/3));	// make sure playedCardCount is valid number of cards
		
		game.handCount[game.whoseTurn] = ((rand() % (MAX_HAND - 1)) + 1);	// ensure the current player has at least one card in hand (card to play)
		handPos = (rand() % game.handCount[game.whoseTurn]);	// make sure handPos within the players hand
		choice = (rand() % 2);	// make choice randomly 0 or 1, any choice other than 0 should have same effect
		game.hand[game.whoseTurn][handPos] = baron;	// the card played must be a baron card.

		failures += testPlayBaron(&game, handPos, choice);
	}
	fprintf (stdout, "Testing Complete\n\t%d out of %d tests successful.\n", n+failures, n);

	return 0;
}
