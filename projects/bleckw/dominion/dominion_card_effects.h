#ifndef _DOMINION_CARD_EFFECTS_H
#define _DOMINION_CARD_EFFECTS_H

int baron(int choice1, struct gameState* state);

int minion(int choice1, int choice2, struct gameState* state);

int ambassador(int choice1, int choice2, int handpos, int currentPlayer, struct gameState* state);

tribute(int currentPlayer, struct gameState* state);

int mine(int choice1, int choice2, int handpos, int currentPlayer, struct gameState* state);

#endif
