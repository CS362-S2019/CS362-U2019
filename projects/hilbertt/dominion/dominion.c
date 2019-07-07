#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "cardHelpers/cardEffect.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
   if (*(int *)a > *(int *)b)
      return 1;
   if (*(int *)a < *(int *)b)
      return -1;
   return 0;
}

struct gameState *newGame()
{
   struct gameState *g = malloc(sizeof(struct gameState));
   return g;
}

int *kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
                  int k8, int k9, int k10)
{
   int *k = malloc(10 * sizeof(int));
   k[0] = k1;
   k[1] = k2;
   k[2] = k3;
   k[3] = k4;
   k[4] = k5;
   k[5] = k6;
   k[6] = k7;
   k[7] = k8;
   k[8] = k9;
   k[9] = k10;
   return k;
}

int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state)
{

   int i;
   int j;
   int it;
   //set up random number generator
   SelectStream(1);
   PutSeed((long)randomSeed);

   //check number of players
   if (numPlayers > MAX_PLAYERS || numPlayers < 2)
   {
      return -1;
   }

   //set number of players
   state->numPlayers = numPlayers;

   //check selected kingdom cards are different
   for (i = 0; i < 10; i++)
   {
      for (j = 0; j < 10; j++)
      {
         if (j != i && kingdomCards[j] == kingdomCards[i])
         {
            return -1;
         }
      }
   }

   //initialize supply
   ///////////////////////////////

   //set number of Curse cards
   if (numPlayers == 2)
   {
      state->supplyCount[curse] = 10;
   }
   else if (numPlayers == 3)
   {
      state->supplyCount[curse] = 20;
   }
   else
   {
      state->supplyCount[curse] = 30;
   }

   //set number of Victory cards
   if (numPlayers == 2)
   {
      state->supplyCount[estate] = 8;
      state->supplyCount[duchy] = 8;
      state->supplyCount[province] = 8;
   }
   else
   {
      state->supplyCount[estate] = 12;
      state->supplyCount[duchy] = 12;
      state->supplyCount[province] = 12;
   }

   //set number of Treasure cards
   state->supplyCount[copper] = 60 - (7 * numPlayers);
   state->supplyCount[silver] = 40;
   state->supplyCount[gold] = 30;

   //set number of Kingdom cards
   for (i = adventurer; i <= treasure_map; i++) //loop all cards
   {
      for (j = 0; j < 10; j++) //loop chosen cards
      {
         if (kingdomCards[j] == i)
         {
            //check if card is a 'Victory' Kingdom card
            if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
            {
               if (numPlayers == 2)
               {
                  state->supplyCount[i] = 8;
               }
               else
               {
                  state->supplyCount[i] = 12;
               }
            }
            else
            {
               state->supplyCount[i] = 10;
            }
            break;
         }
         else //card is not in the set choosen for the game
         {
            state->supplyCount[i] = -1;
         }
      }
   }

   ////////////////////////
   //supply intilization complete

   //set player decks
   for (i = 0; i < numPlayers; i++)
   {
      state->deckCount[i] = 0;
      for (j = 0; j < 3; j++)
      {
         state->deck[i][j] = estate;
         state->deckCount[i]++;
      }
      for (j = 3; j < 10; j++)
      {
         state->deck[i][j] = copper;
         state->deckCount[i]++;
      }
   }

   //shuffle player decks
   for (i = 0; i < numPlayers; i++)
   {
      if (shuffle(i, state) < 0)
      {
         return -1;
      }
   }

   //draw player hands
   for (i = 0; i < numPlayers; i++)
   {
      //initialize hand size to zero
      state->handCount[i] = 0;
      state->discardCount[i] = 0;
      //draw 5 cards
      // for (j = 0; j < 5; j++)
      //	{
      //	  drawCard(i, state);
      //	}
   }

   //set embargo tokens to 0 for all supply piles
   for (i = 0; i <= treasure_map; i++)
   {
      state->embargoTokens[i] = 0;
   }

   //initialize first player's turn
   state->outpostPlayed = 0;
   state->phase = 0;
   state->numActions = 1;
   state->numBuys = 1;
   state->tabledCardCount = 0;
   state->whoseTurn = 0;
   state->handCount[state->whoseTurn] = 0;
   //int it; move to top

   //Moved draw cards to here, only drawing at the start of a turn
   for (it = 0; it < 5; it++)
   {
      drawCard(state->whoseTurn, state);
   }

   updateCoins(state->whoseTurn, state, 0);

   return 0;
}

int shuffle(int player, struct gameState *state)
{

   int newDeck[MAX_DECK];
   int newDeckPos = 0;
   int card;
   int i;

   if (state->deckCount[player] < 1)
      return -1;
   qsort((void *)(state->deck[player]), state->deckCount[player], sizeof(int), compare);
   /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

   while (state->deckCount[player] > 0)
   {
      card = floor(Random() * state->deckCount[player]);
      newDeck[newDeckPos] = state->deck[player][card];
      newDeckPos++;
      for (i = card; i < state->deckCount[player] - 1; i++)
      {
         state->deck[player][i] = state->deck[player][i + 1];
      }
      state->deckCount[player]--;
   }
   for (i = 0; i < newDeckPos; i++)
   {
      state->deck[player][i] = newDeck[i];
      state->deckCount[player]++;
   }

   return 0;
}

int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
{
   int card;
   int coin_bonus = 0; //tracks coins gain from actions

   /* Make number of player selections an array for better extendability */
   int choices[3] = {choice1, choice2, choice3};
   int num_choices = 3;
   for (int i = 2; i >= 0; i--)
   {
      if (choices[i] == -1)
      {
         num_choices--;
      }
      else
      {
         break;
      }
   }

   //check if it is the right phase
   if (state->phase != 0)
   {
      return -1;
   }

   //check if player has enough actions
   if (state->numActions < 1)
   {
      return -1;
   }

   //get card tabled
   card = handCard(handPos, state);

   //check if selected card is an action
   if (card < adventurer || card > treasure_map)
   {
      return -1;
   }

   //play card
   if (cardEffect(card, choices, num_choices, state, handPos) < 0)
   {
      return -1;
   }

   //reduce number of actions
   state->numActions--;

   //update coins (Treasure cards may be added with card draws)
   updateCoins(state->whoseTurn, state, coin_bonus);

   return 0;
}

int buyCard(int supplyPos, struct gameState *state)
{
   int who;
   if (DEBUG)
   {
      printf("Entering buyCard...\n");
   }

   // I don't know what to do about the phase thing.

   who = state->whoseTurn;

   if (state->numBuys < 1)
   {
      if (DEBUG)
         printf("You do not have any buys left\n");
      return -1;
   }
   else if (supplyCount(supplyPos, state) < 1)
   {
      if (DEBUG)
         printf("There are not any of that type of card left\n");
      return -1;
   }
   else if (state->coins < getCost(supplyPos))
   {
      if (DEBUG)
         printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
      return -1;
   }
   else
   {
      state->phase = 1;
      //state->supplyCount[supplyPos]--;
      gainCard(supplyPos, state, 0, who); //card goes in discard, this might be wrong.. (2 means goes into hand, 0 goes into discard)

      state->coins = (state->coins) - (getCost(supplyPos));
      state->numBuys--;
      if (DEBUG)
         printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos, getCost(supplyPos), state->numBuys, state->coins);
   }

   //state->discard[who][state->discardCount[who]] = supplyPos;
   //state->discardCount[who]++;

   return 0;
}

int numHandCards(struct gameState *state)
{
   return state->handCount[whoseTurn(state)];
}

int handCard(int handPos, struct gameState *state)
{
   int currentPlayer = whoseTurn(state);
   return state->hand[currentPlayer][handPos];
}

int supplyCount(int card, struct gameState *state)
{
   return state->supplyCount[card];
}

int fullDeckCount(int player, int card, struct gameState *state)
{
   int i;
   int count = 0;

   for (i = 0; i < state->deckCount[player]; i++)
   {
      if (state->deck[player][i] == card)
         count++;
   }

   for (i = 0; i < state->handCount[player]; i++)
   {
      if (state->hand[player][i] == card)
         count++;
   }

   for (i = 0; i < state->discardCount[player]; i++)
   {
      if (state->discard[player][i] == card)
         count++;
   }

   return count;
}

int whoseTurn(struct gameState *state)
{
   return state->whoseTurn;
}

int whoseNextTurn(struct gameState *state){
   return state->whoseTurn + 1 == state->numPlayers ? 0 : state->whoseTurn + 1;
}

int endTurn(struct gameState *state)
{
   int k;
   int i;
   int currentPlayer = whoseTurn(state);

   //Discard hand
   for (i = 0; i < state->handCount[currentPlayer]; i++)
   {
      state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i]; //Discard
      state->hand[currentPlayer][i] = -1;                                                                  //Set card to -1
   }
   state->handCount[currentPlayer] = 0; //Reset hand count

   //Code for determining the player
   if (currentPlayer < (state->numPlayers - 1))
   {
      state->whoseTurn = currentPlayer + 1; //Still safe to increment
   }
   else
   {
      state->whoseTurn = 0; //Max player has been reached, loop back around to player 1
   }

   state->outpostPlayed = 0;
   state->phase = 0;
   state->numActions = 1;
   state->coins = 0;
   state->numBuys = 1;
   state->tabledCardCount = 0;
   state->handCount[state->whoseTurn] = 0;

   //int k; move to top
   //Next player draws hand
   for (k = 0; k < 5; k++)
   {
      drawCard(state->whoseTurn, state); //Draw a card
   }

   //Update money
   updateCoins(state->whoseTurn, state, 0);

   return 0;
}

int isGameOver(struct gameState *state)
{
   int i;
   int j;

   //if stack of Province cards is empty, the game ends
   if (state->supplyCount[province] == 0)
   {
      return 1;
   }

   //if three supply pile are at 0, the game ends
   j = 0;
   for (i = 0; i < 25; i++)
   {
      if (state->supplyCount[i] == 0)
      {
         j++;
      }
   }
   if (j >= 3)
   {
      return 1;
   }

   return 0;
}

int scoreFor(int player, struct gameState *state)
{

   int i;
   int score = 0;
   //score from hand
   for (i = 0; i < state->handCount[player]; i++)
   {
      if (state->hand[player][i] == curse)
      {
         score = score - 1;
      };
      if (state->hand[player][i] == estate)
      {
         score = score + 1;
      };
      if (state->hand[player][i] == duchy)
      {
         score = score + 3;
      };
      if (state->hand[player][i] == province)
      {
         score = score + 6;
      };
      if (state->hand[player][i] == great_hall)
      {
         score = score + 1;
      };
      if (state->hand[player][i] == gardens)
      {
         score = score + (fullDeckCount(player, 0, state) / 10);
      };
   }

   //score from discard
   for (i = 0; i < state->discardCount[player]; i++)
   {
      if (state->discard[player][i] == curse)
      {
         score = score - 1;
      };
      if (state->discard[player][i] == estate)
      {
         score = score + 1;
      };
      if (state->discard[player][i] == duchy)
      {
         score = score + 3;
      };
      if (state->discard[player][i] == province)
      {
         score = score + 6;
      };
      if (state->discard[player][i] == great_hall)
      {
         score = score + 1;
      };
      if (state->discard[player][i] == gardens)
      {
         score = score + (fullDeckCount(player, 0, state) / 10);
      };
   }

   //score from deck
   for (i = 0; i < state->discardCount[player]; i++)
   {
      if (state->deck[player][i] == curse)
      {
         score = score - 1;
      };
      if (state->deck[player][i] == estate)
      {
         score = score + 1;
      };
      if (state->deck[player][i] == duchy)
      {
         score = score + 3;
      };
      if (state->deck[player][i] == province)
      {
         score = score + 6;
      };
      if (state->deck[player][i] == great_hall)
      {
         score = score + 1;
      };
      if (state->deck[player][i] == gardens)
      {
         score = score + (fullDeckCount(player, 0, state) / 10);
      };
   }

   return score;
}

int getWinners(int players[MAX_PLAYERS], struct gameState *state)
{
   int i;
   int j;
   int highScore;
   int currentPlayer;

   //get score for each player
   for (i = 0; i < MAX_PLAYERS; i++)
   {
      //set unused player scores to -9999
      if (i >= state->numPlayers)
      {
         players[i] = -9999;
      }
      else
      {
         players[i] = scoreFor(i, state);
      }
   }

   //find highest score
   j = 0;
   for (i = 0; i < MAX_PLAYERS; i++)
   {
      if (players[i] > players[j])
      {
         j = i;
      }
   }
   highScore = players[j];

   //add 1 to players who had less turns
   currentPlayer = whoseTurn(state);
   for (i = 0; i < MAX_PLAYERS; i++)
   {
      if (players[i] == highScore && i > currentPlayer)
      {
         players[i]++;
      }
   }

   //find new highest score
   j = 0;
   for (i = 0; i < MAX_PLAYERS; i++)
   {
      if (players[i] > players[j])
      {
         j = i;
      }
   }
   highScore = players[j];

   //set winners in array to 1 and rest to 0
   for (i = 0; i < MAX_PLAYERS; i++)
   {
      if (players[i] == highScore)
      {
         players[i] = 1;
      }
      else
      {
         players[i] = 0;
      }
   }

   return 0;
}

int drawCard(int player, struct gameState *state)
{
   int count;
   int deckCounter;
   if (state->deckCount[player] <= 0)
   { //Deck is empty

      //Step 1 Shuffle the discard pile back into a deck
      int i;
      //Move discard to deck
      for (i = 0; i < state->discardCount[player]; i++)
      {
         state->deck[player][i] = state->discard[player][i];
         state->discard[player][i] = -1;
      }

      state->deckCount[player] = state->discardCount[player];
      state->discardCount[player] = 0; //Reset discard

      //Shufffle the deck
      shuffle(player, state); //Shuffle the deck up and make it so that we can draw

      if (DEBUG)
      { //Debug statements
         printf("Deck count now: %d\n", state->deckCount[player]);
      }

      state->discardCount[player] = 0;

      //Step 2 Draw Card
      count = state->handCount[player]; //Get current player's hand count

      if (DEBUG)
      { //Debug statements
         printf("Current hand count: %d\n", count);
      }

      deckCounter = state->deckCount[player]; //Create a holder for the deck count

      if (deckCounter == 0)
         return -1;

      state->hand[player][count] = state->deck[player][deckCounter - 1]; //Add card to hand
      state->deckCount[player]--;
      state->handCount[player]++; //Increment hand count
   }

   else
   {
      int count = state->handCount[player]; //Get current hand count for player
      int deckCounter;
      if (DEBUG)
      { //Debug statements
         printf("Current hand count: %d\n", count);
      }

      deckCounter = state->deckCount[player];                            //Create holder for the deck count
      state->hand[player][count] = state->deck[player][deckCounter - 1]; //Add card to the hand
      state->deckCount[player]--;
      state->handCount[player]++; //Increment hand count
   }

   return 0;
}

int getCost(int cardNumber)
{
   switch (cardNumber)
   {
   case curse:
      return 0;
   case estate:
      return 2;
   case duchy:
      return 5;
   case province:
      return 8;
   case copper:
      return 0;
   case silver:
      return 3;
   case gold:
      return 6;
   case adventurer:
      return 6;
   case council_room:
      return 5;
   case feast:
      return 4;
   case gardens:
      return 4;
   case mine:
      return 5;
   case remodel:
      return 4;
   case smithy:
      return 4;
   case village:
      return 3;
   case baron:
      return 4;
   case great_hall:
      return 3;
   case minion:
      return 5;
   case steward:
      return 3;
   case tribute:
      return 5;
   case ambassador:
      return 3;
   case cutpurse:
      return 4;
   case embargo:
      return 2;
   case outpost:
      return 5;
   case salvager:
      return 4;
   case sea_hag:
      return 4;
   case treasure_map:
      return 4;
   }

   return -1;
}

int tableCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
{

   //if card is not trashed, added to tabled pile
   if (trashFlag < 1)
   {
      //add card to tabled pile
      state->tabledCards[state->tabledCardCount] = state->hand[currentPlayer][handPos];
      state->tabledCardCount++;
   }

   //set tabled card to -1
   state->hand[currentPlayer][handPos] = -1;

   //remove card from player's hand
   if (handPos == (state->handCount[currentPlayer] - 1)) //last card in hand array is tabled
   {
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
   }
   else if (state->handCount[currentPlayer] == 1) //only one card in hand
   {
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
   }
   else
   {
      //replace discarded card with last card in hand
      state->hand[currentPlayer][handPos] = state->hand[currentPlayer][(state->handCount[currentPlayer] - 1)];
      //set last card to -1
      state->hand[currentPlayer][state->handCount[currentPlayer] - 1] = -1;
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
   }

   return 0;
}

int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
{
   //Note: supplyPos is enum of choosen card

   //check if supply pile is empty (0) or card is not used in game (-1)
   if (supplyCount(supplyPos, state) < 1)
   {
      return -1;
   }

   //added card for [whoseTurn] current player:
   // toFlag = 0 : add to discard
   // toFlag = 1 : add to deck
   // toFlag = 2 : add to hand

   if (toFlag == 1)
   {
      state->deck[player][state->deckCount[player]] = supplyPos;
      state->deckCount[player]++;
   }
   else if (toFlag == 2)
   {
      state->hand[player][state->handCount[player]] = supplyPos;
      state->handCount[player]++;
   }
   else
   {
      state->discard[player][state->discardCount[player]] = supplyPos;
      state->discardCount[player]++;
   }

   //decrease number in supply pile
   state->supplyCount[supplyPos]--;

   return 0;
}

int updateCoins(int player, struct gameState *state, int bonus)
{
   int i;

   //reset coin count
   state->coins = 0;

   //add coins for each Treasure card in player's hand
   for (i = 0; i < state->handCount[player]; i++)
   {
      if (state->hand[player][i] == copper)
      {
         state->coins += 1;
      }
      else if (state->hand[player][i] == silver)
      {
         state->coins += 2;
      }
      else if (state->hand[player][i] == gold)
      {
         state->coins += 3;
      }
   }

   //add bonus
   state->coins += bonus;

   return 0;
}

int discardCard(int currentPlayer, struct gameState *state, int handPos)
{
   state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][handPos];
   state->discardCount[currentPlayer]++;
   for (; handPos < state->handCount[currentPlayer]; handPos++)
   {
      state->hand[currentPlayer][handPos] = state->hand[currentPlayer][handPos + 1];
   }
   state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
   state->handCount[currentPlayer]--;

   return 0;
}

int setAndShuffleDeck(int player, struct gameState *state){
   int i = 0;
   if(state->deckCount[player] != 0){
      return -1;
   }

   for (i = 0; i < state->discardCount[player]; i++)
   {
      state->deck[player][i] = state->discard[player][i]; //Move to deck
      state->deckCount[player]++;
      state->discard[player][i] = -1;
      state->discardCount[player]--;
   }

   shuffle(player, state); //Shuffle the deck
   return 0;
}

//end of dominion.c