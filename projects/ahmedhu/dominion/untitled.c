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

		int preActions = game->numActions;