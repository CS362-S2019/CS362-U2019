#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

// hughesc3-assignment2 helpers
int baronEffect(struct gameState* state, int choice, int currentPlayer);
int minionEffect(struct gameState* state, int choice1, int choice2, int currentPlayer, int handPos);
int ambassadorEffect(struct gameState* state, int choice1, int choice2, int currentPlayer, int handPos);
int mineEffect(struct gameState* state, int choice1, int choice2, int currentPlayer, int handPos);
int discardHelper(int handPos, int currentPlayer, struct gameState* state);

#endif
