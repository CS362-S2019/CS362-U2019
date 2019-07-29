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
	int tests = 100;

	for (int i = 0; i < tests; i++){
		//randomize player count, seed random
		int seed = rand();
		int players = rand() % 4;
		int currPlayer = 0;
		int choice1 = rand() % 2;
		int choice2 = rand() % 2;

		struct gameState *game = malloc(sizeof(struct gameState));
		initializeGame(players, k, seed, game);

		//randomize discard/deck/handcount for all players
		for (int i = 0; i < players; i++){
			game->handCount[i] = rand() % MAX_HAND;
			game->deckCount[i] = rand() % MAX_DECK;
			game->discardCount[i] = rand() % MAX_DECK;		
		}

		int preActions = game->numActions;
		int preCoins = game->coins;
		int preHandCount = game->handCount[currPlayer];

		cardEffect(tribute, choice1, choice2, 1, game, 0, 0);

		if (preActions + 2 == game->numActions || preCoins + 2 == game->coins || preHandCount + 2 == game->handCount[currPlayer]){
			passed++;
		} else{
			failed++;
		}

		free(game);
	}

	printf("Random Test of Tribute Card!\n");
	printf("There were %d successful tests and %d failed tests out of %d games!\n\n\n", passed, failed, tests);

	return 0;

}