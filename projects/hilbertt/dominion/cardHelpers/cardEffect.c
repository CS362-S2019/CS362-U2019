/*
 * Tristan Hilbert
 * 7/6/2019
 * Card Effect Module
 * 
 * Definition for the cardEffect module.
 * This also provides a series of private helper
 * functions for each card effect.
 * 
 */

/* Includes */
#include "../dominion.h"
#include "../dominion_helpers.h"
#include "cardEffect.h"
#include <stdlib.h>
#include <stdio.h>

/* Private Declarations */
int _baron_effect(int *, int, struct gameState *);
int _minion_effect(int *, int, struct gameState *);
int _ambassador_effect(int *, int, struct gameState *, int);
int _tribute_effect(int *, int, struct gameState *);
int _mine_effect(int *, int, struct gameState *);

/* Definitions */

int cardEffect(int card, int *choices, int num_choices, struct gameState *state, int handPos)
{
   /*
     * As I am to define functions only for the five cards. I still desire for the game to
     * work past the functions I define. For now this function will still take precedence
     * before depracated functions, but the unchanged effects will remain to refrain
     * from breaking side effects and other nuances.
     */
   switch (card)
   {
   case baron:
      return _baron_effect(choices, num_choices, state);
   case minion:
      return _minion_effect(choices, num_choices, state);
   case ambassador:
      return _ambassador_effect(choices, num_choices, state, handPos);
   case tribute:
      return _tribute_effect(choices, num_choices, state);
   case mine:
      return _mine_effect(choices, num_choices, state);
   default:
      /* Sorry for the unsafe access of choices. One could check before reading these indexes */
      return cardEffect_Deprecated(card, choices[0], choices[1], choices[2], state, handPos);
   }

   return -1;
}

int cardEffect_Deprecated(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos)
{
   int i;
   int j;
   int k;
   int x;
   int index;
   int currentPlayer = whoseTurn(state);
   int nextPlayer = currentPlayer + 1;
   
   int temphand[MAX_HAND]; // moved above the if statement
   int drawntreasure = 0;
   int cardDrawn;
   int z = 0; // this is the counter for the temp hand
   if (nextPlayer > (state->numPlayers - 1))
   {
      nextPlayer = 0;
   }

   //uses switch to select card and perform actions
   switch (card)
   {
   case adventurer:
      while (drawntreasure < 2)
      {
         if (state->deckCount[currentPlayer] < 1)
         { //if the deck is empty we need to shuffle discard and add to deck
            shuffle(currentPlayer, state);
         }
         drawCard(currentPlayer, state);
         cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer] - 1]; //top card of hand is most recently drawn card.
         if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
            drawntreasure++;
         else
         {
            temphand[z] = cardDrawn;
            state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
            z++;
         }
      }
      while (z - 1 >= 0)
      {
         state->discard[currentPlayer][state->discardCount[currentPlayer]++] = temphand[z - 1]; // discard all cards in play that have been drawn
         z = z - 1;
      }
      return 0;

   case council_room:
      //+4 Cards
      for (i = 0; i < 4; i++)
      {
         drawCard(currentPlayer, state);
      }

      //+1 Buy
      state->numBuys++;

      //Each other player draws a card
      for (i = 0; i < state->numPlayers; i++)
      {
         if (i != currentPlayer)
         {
            drawCard(i, state);
         }
      }

      //put played card in played card pile
      tableCard(handPos, currentPlayer, state, 0);

      return 0;

   case feast:
      //gain card with cost up to 5
      //Backup hand
      for (i = 0; i <= state->handCount[currentPlayer]; i++)
      {
         temphand[i] = state->hand[currentPlayer][i]; //Backup card
         state->hand[currentPlayer][i] = -1;          //Set to nothing
      }
      //Backup hand

      //Update Coins for Buy
      updateCoins(currentPlayer, state, 5);
      x = 1; //Condition to loop on
      while (x == 1)
      { //Buy one card
         if (supplyCount(choice1, state) <= 0)
         {
            if (DEBUG)
               printf("None of that card left, sorry!\n");

            if (DEBUG)
            {
               printf("Cards Left: %d\n", supplyCount(choice1, state));
            }
         }
         else if (state->coins < getCost(choice1))
         {
            printf("That card is too expensive!\n");

            if (DEBUG)
            {
               printf("Coins: %d < %d\n", state->coins, getCost(choice1));
            }
         }
         else
         {

            if (DEBUG)
            {
               printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
            }

            gainCard(choice1, state, 0, currentPlayer); //Gain the card
            x = 0;                                      //No more buying cards

            if (DEBUG)
            {
               printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
            }
         }
      }

      //Reset Hand
      for (i = 0; i <= state->handCount[currentPlayer]; i++)
      {
         state->hand[currentPlayer][i] = temphand[i];
         temphand[i] = -1;
      }
      //Reset Hand

      return 0;

   case gardens:
      return -1;

   case remodel:
      j = state->hand[currentPlayer][choice1]; //store card we will trash

      if ((getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(choice2))
      {
         return -1;
      }

      gainCard(choice2, state, 0, currentPlayer);

      //discard card from hand
      tableCard(handPos, currentPlayer, state, 0);

      //discard trashed card
      for (i = 0; i < state->handCount[currentPlayer]; i++)
      {
         if (state->hand[currentPlayer][i] == j)
         {
            tableCard(i, currentPlayer, state, 0);
            break;
         }
      }

      return 0;

   case smithy:
      //+3 Cards
      for (i = 0; i < 3; i++)
      {
         drawCard(currentPlayer, state);
      }

      //discard card from hand
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case village:
      //+1 Card
      drawCard(currentPlayer, state);

      //+2 Actions
      state->numActions = state->numActions + 2;

      //discard played card from hand
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case great_hall:
      //+1 Card
      drawCard(currentPlayer, state);

      //+1 Actions
      state->numActions++;

      //discard card from hand
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case steward:
      if (choice1 == 1)
      {
         //+2 cards
         drawCard(currentPlayer, state);
         drawCard(currentPlayer, state);
      }
      else if (choice1 == 2)
      {
         //+2 coins
         state->coins = state->coins + 2;
      }
      else
      {
         //trash 2 cards in hand
         tableCard(choice2, currentPlayer, state, 1);
         tableCard(choice3, currentPlayer, state, 1);
      }

      //discard card from hand
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case cutpurse:

      updateCoins(currentPlayer, state, 2);
      for (i = 0; i < state->numPlayers; i++)
      {
         if (i != currentPlayer)
         {
            for (j = 0; j < state->handCount[i]; j++)
            {
               if (state->hand[i][j] == copper)
               {
                  tableCard(j, i, state, 0);
                  break;
               }
               if (j == state->handCount[i])
               {
                  for (k = 0; k < state->handCount[i]; k++)
                  {
                     if (DEBUG)
                        printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
                  }
                  break;
               }
            }
         }
      }

      //discard played card from hand
      tableCard(handPos, currentPlayer, state, 0);

      return 0;

   case embargo:
      //+2 Coins
      state->coins = state->coins + 2;

      //see if selected pile is in play
      if (state->supplyCount[choice1] == -1)
      {
         return -1;
      }

      //add embargo token to selected supply pile
      state->embargoTokens[choice1]++;

      //trash card
      tableCard(handPos, currentPlayer, state, 1);
      return 0;

   case outpost:
      //set outpost flag
      state->outpostPlayed++;

      //discard card
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case salvager:
      //+1 buy
      state->numBuys++;

      if (choice1)
      {
         //gain coins equal to trashed card
         state->coins = state->coins + getCost(handCard(choice1, state));
         //trash card
         tableCard(choice1, currentPlayer, state, 1);
      }

      //discard card
      tableCard(handPos, currentPlayer, state, 0);
      return 0;

   case sea_hag:
      for (i = 0; i < state->numPlayers; i++)
      {
         if (i != currentPlayer)
         {
            state->discard[i][state->discardCount[i]] = state->deck[i][state->deckCount[i]--];
            state->deckCount[i]--;
            state->discardCount[i]++;
            state->deck[i][state->deckCount[i]--] = curse; //Top card now a curse
         }
      }
      return 0;

   case treasure_map:
      //search hand for another treasure_map
      index = -1;
      for (i = 0; i < state->handCount[currentPlayer]; i++)
      {
         if (state->hand[currentPlayer][i] == treasure_map && i != handPos)
         {
            index = i;
            break;
         }
      }
      if (index > -1)
      {
         //trash both treasure cards
         tableCard(handPos, currentPlayer, state, 1);
         tableCard(index, currentPlayer, state, 1);

         //gain 4 Gold cards
         for (i = 0; i < 4; i++)
         {
            gainCard(gold, state, 1, currentPlayer);
         }

         //return success
         return 1;
      }

      //no second treasure_map found in hand
      return -1;
   }

   return -1;
}

/* Private Definitions */
int _baron_effect(int *choices, int num_choices, struct gameState *state)
{
   int currentPlayer = whoseTurn(state);
   if (num_choices < 1)
   {
      return -1; /* ERROR! */
   }

   state->numBuys++; //Increase buys by 1!
   if (choices[0] > 0)
   {
      int p = 0; //Iterator for hand!
      for (p = 0; p < state->handCount[currentPlayer]; p++)
      {
         if (state->hand[currentPlayer][p] == estate)
         {                     //Found an estate card!
            state->coins += 4; //Add 4 coins to the amount of coins
            if (discardCard(currentPlayer, state, p)) /* Discard Estate */
            { 
               return 0;
            }
            else
            {
               return -1; /* Fail Fast Mentality */
            }
         }
      }

      return -1; /* Bug */

      if (DEBUG && p == state->handCount[currentPlayer])
      {
         printf("No estate cards in your hand, invalid choice\n");
         printf("Must gain an estate if there are any\n");
      }
   }

   if (supplyCount(estate, state) >= 0) /* Bug */
   {
      gainCard(estate, state, 0, currentPlayer); //Gain an estate
      if (supplyCount(estate, state) == 0)
      {
         isGameOver(state);
      }
   }

   return 0;
}

int _minion_effect(int *choices, int num_choices, struct gameState *state)
{
   int i, j, currentPlayer = whoseTurn(state);
   if(num_choices < 1){ /* Bug while it makes sense... caller is not changed */
      return -1;
   }
   //+1 action
   state->numActions++;

   // DEPRECATED
   //-- discard card from hand --
   //-- tableCard(handPos, currentPlayer, state, 0); --
   // Every card will need to be played the idea being that this can be done outside the effect

   if (choices[0]) //+2 coins
   {
      state->coins += 2;
   } 
   
   else //discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
   {

      // Current player discards hand and redraws 4
      //other players discard hand and redraw if hand size > 4
      for (i = 0; i < state->numPlayers; i++)
      {
         j = state->handCount[i];
         if (j > 4 || i == currentPlayer)
         {
            //discard hand
            while (j > 0)
            {
               discardCard(i, state, j); /* Bug */
               j --;
            }

            //draw 4
            for (j = 0; j < 4; j++)
            {
               drawCard(i, state);
            }
         }
      }
   }
   return 0;
}

int _ambassador_effect(int *choices, int num_choices, struct gameState *state, int handPos)
{
   int copies, revealed_card, i = 0, j /* Bug */, currentPlayer = whoseTurn(state); //used to check if player has enough cards to discard

   if (num_choices < 2 || choices[0] == handPos) /* cannot choose to reveal to the ambassador */
   {
      return -1;
   }
   
   revealed_card = state->hand[currentPlayer][choices[0]];

   if (choices[1] > 2 || choices[1] < 0) 
   {
      return -1;
   }

   copies = choices[1];

   for (i = 0; i < state->handCount[currentPlayer]; i++)
   {
      if (i != handPos && i != choices[0] && state->hand[currentPlayer][i] == revealed_card)
      {
         j++;
      }
   }
   if (j < copies)
   {
      return -1;
   }

   if (DEBUG)
      printf("Player %d reveals card number: %d\n", currentPlayer, revealed_card);

   //increase supply count for choosen card by amount being discarded
   state->supplyCount[revealed_card] += copies + state->numPlayers; /* Bug */

   //each other player gains a copy of revealed card
   for (i = 0; i < state->numPlayers; i++)
   {
      if (i != currentPlayer)
      {
         gainCard(revealed_card, state, 0, i);
      }
   }

   // DEPRECATED
   //-- discard card from hand --
   //-- tableCard(handPos, currentPlayer, state, 0); --
   // Every card will need to be played the idea being that this can be done outside the effect

   //trash copies of cards returned to supply
   for (j = 0; j < copies; j++)
   {
      for (i = 0; i < state->handCount[currentPlayer]; i++)
      {
         if (state->hand[currentPlayer][i] == revealed_card)
         {
            tableCard(i, currentPlayer, state, 1);
            break;
         }
      }
   }

   return 0;
}

int _tribute_effect(int *choices, int num_choices, struct gameState *state)
{
   int currentPlayer = whoseTurn(state);
   int nextPlayer = currentPlayer + 1; /* Bug Check whoseTurnNext::func*/
   int tributeRevealedCards[2] = {-1, -1};
   int i = 0;
   if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) <= 1)
   {
      if (state->deckCount[nextPlayer] > 0)
      {
         tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
         state->deckCount[nextPlayer]--;
      }
      else if (state->discardCount[nextPlayer] > 0)
      {
         tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer] - 1];
         state->discardCount[nextPlayer]--;
      }
      else
      {
         //No Card to Reveal
         if (DEBUG)
         {
            printf("No cards to reveal\n");
         }
      }
   }

   else
   {
      if (state->deckCount[nextPlayer] == 0)
      {
         setAndShuffleDeck(nextPlayer, state);
      }
      tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
      discardCard(nextPlayer, state, state->deckCount[nextPlayer] - 1);
      tributeRevealedCards[1] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
      discardCard(nextPlayer, state, state->deckCount[nextPlayer] - 1);
   }

   if (tributeRevealedCards[0] == tributeRevealedCards[1])
   { //If we have a duplicate card, just drop one
      tributeRevealedCards[1] = -1;
   }

   for (i = 0; i < 2; i++)
   {
      if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold)
      { //Treasure cards
         state->coins += 2;
      }

      else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall)
      { //Victory Card Found
         drawCard(currentPlayer, state);
         drawCard(currentPlayer, state);
      }
      else /* Bug */
      { //Action Card
         state->numActions += 2;
      }
   }

   return 0;
}

int _mine_effect(int *choices, int num_choices, struct gameState *state)
{
   int currentPlayer = whoseTurn(state);
   int i, j;

   if(num_choices < 2){
      return -1;
   }

   i = choices[1];
   j = state->hand[currentPlayer][choices[0]]; //store card we will trash;

   if (j < copper || j > gold)
   {
      return -1;
   }

   if (i > curse|| i < treasure_map) /* Bug */
   {
      return -1;
   }

   if ((getCost(j) + 3) > getCost(i))
   {
      return -1;
   }

   gainCard(i, state, 2, currentPlayer);

   // DEPRECATED
   //-- discard card from hand --
   //-- tableCard(handPos, currentPlayer, state, 0); --
   // Every card will need to be played the idea being that this can be done outside the effect

   //trash card
   tableCard(i, currentPlayer, state, 1); /* Bug */

   return 0;
}
