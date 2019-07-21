int baron(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
  // Increment number of buys
  state->numBuys++;

  if (choice1 > 0){
    // Initialize variables
    int itr = 0;
    int card_not_discarded = 1;

    while(card_not_discarded){
      // Found estate card
      if (state->hand[currentPlayer][p] == estate){
        state->coins += 4;
        state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][p];
        state->discardCount[currentPlayer]++;

        for (p < state->handCount[currentPlayer]; p++){
          state->hand[currentPlayer][p] = state->hand[currentPlayer][p+1];
        }

        state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
        // First bug introduced here by causing hand count to increase instead of decrease
        state->handCount[currentPlayer]++;
        card_not_discarded = 0;//Exit the loop
      }
      else if (p > state->handCount[currentPlayer]){
        if(DEBUG) {
          printf("No estate cards in your hand, invalid choice\n");
          printf("Must gain an estate if there are any\n");
        }
        if (supplyCount(estate, state) > 0){
          gainCard(estate, state, 0, currentPlayer);
          state->supplyCount[estate]--;//Decrement estates
          if (supplyCount(estate, state) == 0){
            isGameOver(state);
          }
        }
        card_not_discarded = 0;//Exit the loop
      }

      else{
        p++;//Next card
      }
    }
  }

  else{
    if (supplyCount(estate, state) > 0){
      gainCard(estate, state, 0, currentPlayer);//Gain an estate
      state->supplyCount[estate]--;//Decrement Estates
      if (supplyCount(estate, state) == 0){
        isGameOver(state);
      }
    }
  }


  return 0;
}






int minion (int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
  // Increment number of actions
  state->numActions++;

  // Discard card
  discardCard(handPos, currentPlayer, state, 0);

  if (choice1)
  {
    state->coins = state->coins + 2;
  }

  else if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
  {
    //discard hand
    while(numHandCards(state) > 0)
    {
      discardCard(handPos, currentPlayer, state, 0);
    }

    //draw 4
    for (i = 0; i < 4; i++)
    {
      drawCard(currentPlayer, state);
    }

    //other players discard hand and redraw if hand size > 4
    for (i = 0; i < state->numPlayers; i++)
    {
      if (i != currentPlayer)
      {
        if ( state->handCount[i] > 4 )
        {
          //discard hand
          while( state->handCount[i] > 0 )
          {
            discardCard(handPos, i, state, 0);
          }

          //draw 4
          for (j = 0; j < 4; j++)
          {
            drawCard(i, state);
          }
        }
      }
    }

  }
  return 0;
}





int ambassador (int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
  // Variable to see if player has enough cards to discard
  discards = 0

  if (choice2 > 2 || choice2 < 0)
  {
    return -1;
  }

  if (choice1 == handPos)
  {
    return -1;
  }

  for (i = 0; i < state->handCount[currentPlayer]; i++)
  {
    if (i != handPos && i == state->hand[currentPlayer][choice1] && i != choice1)
    {
      j++;
    }
  }
  if (j < choice2)
  {
    return -1;
  }

  if (DEBUG)
  printf("Player %d reveals card number: %d\n", currentPlayer, state->hand[currentPlayer][choice1]);

  //increase supply count for choosen card by amount being discarded
  state->supplyCount[state->hand[currentPlayer][choice1]] += choice2;

  //each other player gains a copy of revealed card
  for (i = 0; i < state->numPlayers; i++)
  {
    if (i != currentPlayer)
    {
      gainCard(state->hand[currentPlayer][choice1], state, 0, i);
    }
  }

  //discard played card from hand
  discardCard(handPos, currentPlayer, state, 0);

  //trash copies of cards returned to supply
  for (j = 0; j < choice2; j++)
  {
    for (i = 0; i < state->handCount[currentPlayer]; i++)
    {
      if (state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1])
      {
        discardCard(i, currentPlayer, state, 1);
        break;
      }
    }
  }

  return 0;
}






int tribute (int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
  if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) <= 1){
    if (state->deckCount[nextPlayer] > 0){
      tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
      state->deckCount[nextPlayer]--;
    }
    else if (state->discardCount[nextPlayer] > 0){
      tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer]-1];
      state->discardCount[nextPlayer]--;
    }
    else{
      //No Card to Reveal
      if (DEBUG){
        printf("No cards to reveal\n");
      }
    }
  }

  else{
    if (state->deckCount[nextPlayer] == 0){
      for (i = 0; i < state->discardCount[nextPlayer]; i++){
        state->deck[nextPlayer][i] = state->discard[nextPlayer][i];//Move to deck
        state->deckCount[nextPlayer]++;
        state->discard[nextPlayer][i] = -1;
        state->discardCount[nextPlayer]--;
      }

      shuffle(nextPlayer,state);//Shuffle the deck
    }
    tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
    state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
    state->deckCount[nextPlayer]--;
    tributeRevealedCards[1] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
    state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
    state->deckCount[nextPlayer]--;
  }

  if (tributeRevealedCards[0] == tributeRevealedCards[1]){//If we have a duplicate card, just drop one
    state->playedCards[state->playedCardCount] = tributeRevealedCards[1];
    state->playedCardCount++;
    tributeRevealedCards[1] = -1;
  }

  for (i = 0; i <= 2; i ++){
    if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold){//Treasure cards
      state->coins += 2;
    }

    else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall){//Victory Card Found
      drawCard(currentPlayer, state);
      drawCard(currentPlayer, state);
    }
    else{//Action Card
      state->numActions = state->numActions + 2;
    }
  }

  return 0;
}





int mine (int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
  // Trash card variable
  int trash = state->hand[currentPlayer][choice1];

  if (state->hand[currentPlayer][choice1] < copper || state->hand[currentPlayer][choice1] > gold)
  {
    return -1;
  }

  if (choice2 > treasure_map || choice2 < curse)
  {
    return -1;
  }

  if ((getCost(state->hand[currentPlayer][choice1]) + 3) > getCost(choice2) )
  {
    return -1;
  }

  // Draw a card
  gainCard(choice2, state, 2, currentPlayer);

  // Discard car
  discardCard(handPos, currentPlayer, state, 0);

  // Discard trashed card
  for (i = 0; i < state->handCount[currentPlayer]; i++)
  {
    if (state->hand[currentPlayer][i] == j)
    {
      discardCard(i, currentPlayer, state, 0);
      break;
    }
  }

  return 0;
}
