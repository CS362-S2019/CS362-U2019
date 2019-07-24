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
		int choice2 = rand() % 2;

		struct gameState *game = malloc(sizeof(struct gameState));
		initializeGame(players, k, seed, game);

		game->handCount[currPlayer] = rand() % MAX_HAND;
		game->deckCount[currPlayer] = rand() % MAX_DECK;
		game->discardCount[currPlayer] = rand() % MAX_DECK;	

		int preActions = game->numActions;
		int preCoins = game->coins;

		cardEffect(minion, choice1, choice2, 1, game, 0, 0);

		if(choice1 == 1 && preCoins == game->coins + 2 && game->numActions == preActions + 1){
			passed++;
		} else if (choice2 == 1 && game->handCount[currPlayer] == 4 && game->numActions == preActions + 1){
			passed++;
		} else {
			failed++;
		}

		free(game);
	}

	printf("Random Test of Minion Card!\n");
	printf("There were %d successful tests and %d failed tests out of %d games!\n\n\n", passed, failed, tests);

	return 0;

}