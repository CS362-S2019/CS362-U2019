/*
 * Tristan Hilbert
 * 7/6/2019
 * Card Effects Module Interface
 * 
 * DESC:
 * This represents each of the functions for respective card behaviors.
 * As this is a procedurally designed project, each card will have a function
 * which will access other utility functions (draw, discard, etc.)
 * 
 */

#ifndef _DOMINION_CARD_EFFECT_H
#define _DOMINION_CARD_EFFECT_H

#include "../dominion.h"

int cardEffect(int card, int* choices, int num_choices, struct gameState *state, int handPos);

int cardEffect_Deprecated(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos);

#endif