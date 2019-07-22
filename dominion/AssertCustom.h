//
// Created by Yuning on 2019/07/21
//




#ifndef DOMINION_ASSERTCUSTOM_H
#define DOMINION_ASSERTCUSTOM_H

void assertCustom(int boolean, char * message);

void assertGameState(int player, struct gameState * oldG, struct gameState * newG);

#endif //DOMINION_ASSERTCUSTOM_H