#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int main (){
	int *k = kingdomCards(ambassador, village, minion, gardens, smithy, tribute, baron, cutpurse, mine, outpost);
	int passed = 0;
	int failed = 0;
	int tests = 50;


	for (int i = 0; i < tests; i++){
		//randomize player count, seed random
		int seed = rand();
		int players = rand() % 4;
		int currPlayer = 0;
		int choice1 = rand() % 2;

		struct gameState *game = malloc(sizeof(struct gameState));
		initializeGame(players, k, seed, game);

		//randomize discard/deck/handcount for all players
		for (int i = 0; i < players; i++){
			game->handCount[i] = rand() % MAX_HAND;
			game->deckCount[i] = rand() % MAX_DECK;
			game->discardCount[i] = rand() % MAX_DECK;		
		}

		//Save stats before effect
		int preBuys = game->numBuys;
		int preCoins = game->coins;
		int preEstateCount = 0;
		int preHandCount = game->handCount[currPlayer];
		for (int i = 0; i <= preHandCount; i++){
			if (game->hand[currPlayer][i] == estate){
				preEstateCount++;
			}
		}




		cardEffect(baron, choice1, 1, 1, game, 0, 0);


		int postEstateCount = 0;

		if (preEstateCount > 0 && choice1 == 1 && preHandCount - 2 == game->handCount[currPlayer] &&  preBuys + 1 == game->numBuys && preCoins + 4 == game->coins){
			for (int i = 0; i <= game->handCount[currPlayer]; i++){
				if (game->hand[currPlayer][i] == estate){
					postEstateCount++;
				}
			}
			if (postEstateCount == preEstateCount - 1){
				passed++;
			} else{
				failed++;
			}
		}
		else if (choice1 == 0 && preBuys + 1 == game->numBuys){
			for (int i = 0; i <= game->handCount[currPlayer]; i++){
				if (game->hand[currPlayer][i] == estate){
					postEstateCount++;
				}
			}
			if (postEstateCount == preEstateCount + 1){
				passed++;
			} else{
				failed++;
			}
		} else {
			failed++;
		}


		free(game);
	}
	printf("Random Test of Baron Card!\n");
	printf("There were %d successful tests and %d failed tests out of %d games!\n\n\n", passed, failed, tests);

	return 0;

}