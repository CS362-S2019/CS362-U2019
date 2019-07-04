#ifndef _DOMINION_CARD_EFFECTS_H
#define _DOMINION_CARD_EFFECTS_H

int baron(int choice1, int currentPlayer, struct gameState* state);

int minion(int choice2, int choice1, int handPos, int currentPlayer, struct gameState* state);

int ambassador(int choice1, int choice2, int handPos, int currentPlayer, struct gameState* state);

int tribute(int currentPlayer, int nextPlayer, struct gameState* state);

int mine(int choice1, int choice2, int handPos, int currentPlayer, struct gameState* state);

#endif
