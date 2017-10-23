/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for fullDeckCount() function from dominion.c
 ******************************************************************************/
 
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 1000;
	int player = 0;
	int errors = 0;
	int result;
	int i = 0;
	initializeGame(player, k, randSeed, &G);
	
	printf("\n---------------Testing fullDeckCount() function-----------\n");
	/*Test1: all counts are 0, return count should be 0, no changes to anything else.*/
	printf("TEST 1: total cards count is 0\n");
	G.deckCount[player] = 0;
	G.handCount[player] = 0;
	G.discardCount[player] = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Returned value is 0: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 0)
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
	
	/*Test2: all counts are 10, all cards are the required card, return count should be 30, no changes to anything in the state.*/
	printf("\nTEST 2: all cards are the required one\n");
	G.deckCount[player] = 10;
	G.handCount[player] = 10;
	G.discardCount[player] = 10;
	for(i = 0; i < 10; i++){
		G.deck[player][i] = adventurer;
		G.hand[player][i] = adventurer;
		G.discard[player][i] = adventurer;
	}
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Returned value is 30: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 30)
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
	
	/*Test3: all counts are 10, half of cards are the required card, return count should be 15, no changes to anything in the state.*/
	printf("\nTEST 3: half of cards are the required one\n");
	for(i = 0; i < 10; i++){
		if(i%2 == 0){
			G.deck[player][i] = smithy;
			G.hand[player][i] = smithy;
			G.discard[player][i] = smithy;
		}
	}
	
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Returned value is 15: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 15)
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
	
	/*Test4: all counts are 10, all equired cards are in the deck, then hand, then discard, return count should be 5, no changes to anything in the state.*/
	printf("\nTEST 4: all cards are in on pile.\n");
	for(i = 0; i < 10; i++){
			G.hand[player][i] = smithy;
			G.discard[player][i] = smithy;
	}
	
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Cards are in deck. Returned value is 5: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 5)
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
	
	for(i = 0; i < 10; i++){
			G.hand[player][i] = G.deck[player][i];
			G.deck[player][i] = G.discard[player][i];
		}
	
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Cards are in hand. Returned value is 5: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 5)
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
	
		for(i = 0; i < 10; i++){
			G.discard[player][i] = G.hand[player][i];
			G.hand[player][i] = G.deck[player][i];
		}
	
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Cards are in discard. Returned value is 5: ");
	result = fullDeckCount(player, adventurer, &G);
	if(result == 5)
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
	
	printf("\nTotal errors in fullDeckCount(): %d\n\n", errors);
	
	return 0;
}
