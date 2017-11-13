/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for scoreFor() function from dominion.c
 ******************************************************************************/
  
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	//initialize the game
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 18;
	int errors = 0;
	int player = 0;
	int result;
	int i, j;
	char* cardNames[] = {"Curse", "Estate", "Duchy", "Province", "Great_hall", "Gardens"};
	int victoryCards[] = {curse, estate, duchy, province, great_hall, gardens};
	int cardValue[] = {-1, 1, 3, 6, 1, 0.1};
	initializeGame(2, k, randSeed, &G);
	
	printf("\n---------------Testing scoreFor() function-----------\n");

	
	/*Test1: there are only curse cards, first in hand, then in deck, then in discard, then in al piles*/
	printf("TEST 1: score count with only certain victory cards present\n");
	for(j = 0; j < 6; j++){
		printf("%s cards only:\n", cardNames[j]);
		//only in hand
		G.handCount[player] = 30;
		for(i = 0; i < G.handCount[player]; i++)
			if(i%2 == 0)
				G.hand[player][i] = adventurer;
			else
				G.hand[player][i] = victoryCards[j];
		G.deckCount[player] = 0;
		G.discardCount[player] = 0;
	
		result = scoreFor(player, &G);
		if(j == 5){
			if(result == 1)
					printf("PASSED in hand\n");
			else{
				printf("FAILED in hand\n");
				errors++;
			}
		}	
		else{
			if(result == cardValue[j]*15)
				printf("PASSED in hand\n");
			else{
				printf("FAILED in hand\n");
				errors++;
			}
		}
	
		//only in deck
		G.deckCount[player] = 30;
		for(i = 0; i < G.deckCount[player]; i++)
			if(i%2 == 0)
				G.deck[player][i] = adventurer;
			else
				G.deck[player][i] = victoryCards[j];
		G.handCount[player] = 0;

		result = scoreFor(player, &G);
		if(j == 5){
			if(result == 1)
				printf("PASSED in deck\n");
			else{
				printf("FAILED in deck\n");
				errors++;
			}
		}
		else{
			if(result == cardValue[j]*15)
				printf("PASSED in deck\n");
			else{
				printf("FAILED in deck\n");
				errors++;
			}
		}
	
		//only in discard
		G.discardCount[player] = 30;
		for(i = 0; i < G.discardCount[player]; i++)
			if(i%2 == 0)
				G.discard[player][i] = adventurer;
			else
				G.discard[player][i] = victoryCards[j];
		G.deckCount[player] = 0;
	
		result = scoreFor(player, &G);
		if(j == 5){
			if(result == 1)
				printf("PASSED in discard\n");
			else{
				printf("FAILED in discard\n");
				errors++;
			}
		}
		else{
			if(result == cardValue[j]*15)
				printf("PASSED in discard\n");
			else{
				printf("FAILED in discard\n");
				errors++;
			}
		}
	
		//in all piles
		G.handCount[player] = 30;
		G.deckCount[player] = 30;
		memcpy(&testG, &G, sizeof(struct gameState));
	
		result = scoreFor(player, &G);
		if(j == 5){
			if(result == 4)
				printf("PASSED in total\n");
			else{
				printf("FAILED in total\n");
				errors++;
			}
		}
		else{
			if(result == cardValue[j]*15)
				printf("PASSED in total\n");
			else{
				printf("FAILED in total\n");
				errors++;
			}
		}	
	
		printf("No changes to game state: ");
		if(memcmp(&G, &testG, sizeof(struct gameState)) == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	}
	
	/*Test 2: with different cards present*/
	printf("\nTEST 2: score count with different victory cards present\n");
	j = 0;
	printf("Returned score is correct: ");
	for(i = 0; i < 30; i++){
		if(i%2 == 0)
			G.hand[player][i] = G.deck[player][i] = G.discard[player][i] = smithy;
		else
			while(j < 6)
				G.hand[player][i] = G.deck[player][i] = G.discard[player][i] = victoryCards[j++];
	}
	memcpy(&testG, &G, sizeof(struct gameState));
	
	result = scoreFor(player, &G);

	if(result == 10)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("No changes to game state: ");
	if(memcmp(&G, &testG, sizeof(struct gameState)) == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("\nTotal errors in scoreFor(): %d\n\n", errors);
	return 0;
}
