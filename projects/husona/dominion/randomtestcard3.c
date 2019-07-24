#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

#define DEBUG 0

int testPlayTribute(struct gameState* post, int handPos)
{
	static int n = 1;
	int i, j, c, m, r, w, x, t, code = 0;
	struct gameState pre;
	int revealed[2] = {-1, -1};
	
	memcpy (&pre, post, sizeof(struct gameState));
	w = pre.whoseTurn;
	x = (pre.whoseTurn + 1) % pre.numPlayers; // get the number of the next player
	t = pre.deckCount[x] + pre.discardCount[x];	// total number of cards in next players deck and discard piles
	m = pre.deckCount[pre.whoseTurn] + pre.discardCount[pre.whoseTurn]; // total number of cards available for player to draw
	r = playTribute(post, handPos);

	if(r != 0)
	{
		fprintf(stdout, "Test run %d for playTribute caused return value %d, inputs: handPos %d, players %d, whoseTurn %d\n", n, r, handPos, pre.numPlayers, w);
		code = -1;
	}
	// If return value is not 0 then gamestate should be completely unchanged
	else
	{
		memcpy(&(pre.discard[x]), &(post->discard[x]), sizeof(int) * MAX_DECK);	// Discard should have changed for next player however exact change unknown
		pre.discardCount[x] = post->discardCount[x];	// Discard shound should be affected, due to possible shuffle exact change is unknown
		for(i = 0; i < 2; i++)
		{
			if(i <= pre.discardCount[x])
			{
				revealed[i] = pre.discard[x][pre.discardCount[x] - (1 + i)];
			}
		}
		
		pre.discardCount[pre.whoseTurn]++;	// tribute card is discarded
		pre.handCount[pre.whoseTurn]--;		// tribute is removed from hand when discarded
		
		// revealed cards should be (up to) the last two cards in the nextPlayers discard Pile
		if(t > 1)
		{
			// if the tribute was more than one card, make sure they were not the same named card
			if(pre.discard[x][pre.discardCount[x]-2] == pre.discard[x][pre.discardCount[x]-1])
			{
				t = 1;	// if so we will only check one
			}
		}
		for(i = 0; i < t && i < 2; i++)	// check up to two cards or max of t
		{
			c = pre.discard[x][pre.discardCount[x]-(i + 1)];
			if(c >= copper && c <= gold)	// treasure card
			{
				pre.coins += 2;
			}
			else if((c >= estate && c <= province) || c == gardens || c == great_hall) //victory card
			{
				for(j = 0; j < 2; j++)	// attempt to move two cards into the players hand from either deck or discard
				{
					if(m > 1) // player had enough cards to draw one
					{
						m--;
						pre.handCount[pre.whoseTurn]++;
					}
				}
			}
			else if(c > 0) // not treasure, victory, or curse card therefore action card
			{
				pre.numActions += 2;
			}
		}
		if(t > 0)	// if the next player revealed at least one card
		{
			memcpy(&(pre.deck[x]), &(post->deck[x]), sizeof(int) * MAX_DECK);	// Deck may have changed for this player however exact change unknown
			pre.deckCount[x] = post->deckCount[x];	// Deck may be affected, due to possible shuffle exact change is unknown
			memcpy(&(pre.hand[x]), &(post->hand[x]), sizeof(int) * MAX_HAND);	// Hand may have changed for this player however exact change unknown
			pre.handCount[x] = post->handCount[x];	// hand may be affected, due to possible shuffle exact change is unknown
		}
		
		pre.deckCount[pre.whoseTurn] = post->deckCount[pre.whoseTurn];	// Deck cound should be affected, due to possible shuffle exact change is unknown
		memcpy(&(pre.deck[pre.whoseTurn]), &(post->deck[pre.whoseTurn]), sizeof(int) * MAX_DECK);	// Deck should have changed for this player however exact change unknown
		
		pre.discardCount[pre.whoseTurn] = post->discardCount[pre.whoseTurn];	// Discard should be affected, due to possible shuffle exact change is unknown
		memcpy(&(pre.discard[pre.whoseTurn]), &(post->discard[pre.whoseTurn]), sizeof(int) * MAX_DECK);	// Discard should have changed for this player however exact change unknown
		
		memcpy(&(pre.hand[pre.whoseTurn]), &(post->hand[pre.whoseTurn]), sizeof(int) * MAX_HAND);	// Hand should have changed for this player however exact change unknown
		
		memcpy(&(pre.playedCards), &(post->playedCards), sizeof(int) * MAX_DECK);	// (possibly?) should have changed however exact change unknown
		pre.playedCardCount = post->playedCardCount;	// (possibly?) should have changed however exact change unknown
	}
	
	// If the pre and post do not match after pre is modified to expected output, display error with all unexpected changes shown
	if(memcmp(&pre, post, sizeof(struct gameState)) != 0)
	{
		fprintf(stdout, "Test run %d for playTribute caused unexpected result. inputs: handPos %d, players %d, whoseTurn %d, card1: %d, card2: %d\n",
			n, handPos, pre.numPlayers, w, revealed[0], revealed[1]);
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
	int i, j, n, handPos, failures = 0;
	struct gameState game;


	fprintf (stdout, "Random Testing for playTribute.\n");

	for (n = 0; n < 250000; n++)
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
			
			for(j = 0; j < MAX_DECK; j++)
			{
				game.deck[i][j] = (rand() % (treasure_map + 1));	// make sure cards in deck are valid cards
			}
			
			game.discardCount[i] = (rand() % (MAX_DECK/3));		// make sure all players have valid number of cards in discard
			for(j = 0; j < MAX_DECK; j++)
			{
				game.discard[i][j] = (rand() % (treasure_map + 1));	// make sure cards in discard pile are valid cards
			}
			
			game.handCount[i] = (rand() % (MAX_HAND/3));	// make sure all players have valid number of cards in hand
			for(j = 0; j < MAX_HAND; j++)
			{
				game.hand[i][j] = (rand() % (treasure_map + 1));	// make sure cards in hand are valid cards
			}
		}
		game.playedCardCount = (rand() % (MAX_HAND/3));	// make sure playedCardCount is valid number of cards
		
		game.handCount[game.whoseTurn] = ((rand() % (MAX_HAND - 1)) + 1);	// ensure the current player has at least one card in hand (card to play)
		handPos = (rand() % game.handCount[game.whoseTurn]);	// make sure handPos within the players hand
		game.hand[game.whoseTurn][handPos] = tribute;	// the card played must be a tribute card.

		failures += testPlayTribute(&game, handPos);
	}
	fprintf (stdout, "Testing Complete\n\t%d out of %d tests successful.\n", n+failures, n);

	return 0;
}
