#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>


int baron(int card, int choice1, struct gameState *state, int handPos, int *bonus)
{
    // Increment number of buys
    state->numBuys++;
    
    if (choice1 > 0){
        // Initialize variables iterator is for hand
        int itr = 0;
        
        // First bug introduced here by setting the card not distracted card to false rather than true
        int card_not_discarded = 0;
        
        while(card_not_discarded){
            // Found estate card
            if (state->hand[currentPlayer][itr] == estate){
                state->coins += 4;
                state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][itr];
                state->discardCount[currentPlayer]++;
                
                for (itr < state->handCount[currentPlayer]; itr++){
                    state->hand[currentPlayer][itr] = state->hand[currentPlayer][itr+1];
                }
                
                state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
                // Second bug introduced here by causing hand count to increase instead of decrease
                state->handCount[currentPlayer]++;
                card_not_discarded = 0;//Exit the loop
            }
            else if (itr > state->handCount[currentPlayer]){
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
                itr++;//Next card
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


int main()
{
    struct gameState G;
    printf
    
  mine (
}
