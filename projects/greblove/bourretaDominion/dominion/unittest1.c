/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for shuffle() function from dominion.c
 ******************************************************************************/
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* compareStates(struct gameState a, struct gameState b);
int compare(const void* a, const void* b);

int main(){
	//initialize the game
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 18;
	int errors = 0;
	int temp = 0;
	int i = 0;
	initializeGame(2, k, randSeed, &G);


	printf("---------------Testing shuffle() function-----------\n");

	
	/*Test1: the deck of player 0 should be shuffled, returned value should be 0*/
	printf("TEST 1: non-empty deck shuffle\n");
	G.deckCount[0] = 20;
	for(i = 0; i < 20; i++)
		G.deck[0][i] = i;
	memcpy(&testG, &G, sizeof(struct gameState));
	int result = shuffle(0, &G);
	int* stateCheck;
	printf("Returned value is 0: ");
	if(result == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	printf("Card number in the deck didn't change: ");
	if(G.deckCount[0] == testG.deckCount[0])
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	printf("Deck of player 0 is shuffled: ");
	for(i = 0; i < G.deckCount[0]; i++)
		if(G.deck[0][i] == testG.deck[0][i])
			temp++;
	if(temp > G.deckCount[0]/3){
		printf("FAILED\n");
		errors++;
	}
	else
		printf("PASSED\n");

	printf("No other changes to the game state: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] == 1 && stateCheck[1] == 200)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("No new cards introduced to the deck: ");
	qsort ((void*)(G.deck[0]), G.deckCount[0], sizeof(int), compare); 
	qsort ((void*)(testG.deck[0]), testG.deckCount[0], sizeof(int), compare); 
	for(i = 0; i < G.deckCount[0]; i++)
		if(G.deck[0][i] != testG.deck[0][i]){
			printf("FAILED\n");
			errors++;
			break;
		}

	if(i == G.deckCount[0])
		printf("PASSED\n");
		
	/*Test2: function should return -1 if dec is empty. No changes to the game state*/
	printf("\nTEST 2: empty deck shuffle\n");
	G.deckCount[0] = 0;
	//copy current game state into testG
	memcpy(&testG, &G, sizeof(struct gameState));
	result = shuffle(0, &G);
	printf("Returned value is -1: ");
	if(result == -1)
		printf("PASSED\n");
	else{
		printf("FALED\n");
		errors++;
	}
	printf("No changes to the game state: ");
	if(memcmp(&G, &testG, sizeof(struct gameState)) == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("\nTotal errors in function shuffle(): %d\n\n", errors);

	free(stateCheck);
	return 0;
}




int* compareStates(struct gameState a, struct gameState b){
	int* result = malloc(19*sizeof(int));
	result[0] = 0;
	int i, j;
	int* players = malloc(sizeof(int)*MAX_PLAYERS);
	for(i = 0; i < MAX_PLAYERS; i++)
		players[i] = 0;
	
	if(a.numPlayers != b.numPlayers){
		result[0]++;
		result[1]=1;
	}
	
	for(i = 0; i < treasure_map+1; i++){
		if(a.supplyCount[i] != b.supplyCount[i]){
			result[0]++;
			result[result[0]] = 2;
			break;
		}
	}
	
	for(i = 0; i < treasure_map+1; i++){
		if(a.embargoTokens[i] != b.embargoTokens[i]){
			result[0]++;
			result[result[0]] = 3;
			break;
		}
	}
	
	if(a.outpostPlayed != b.outpostPlayed){
		result[0]++;
		result[result[0]] = 4;
	}
	
	if(a.outpostTurn != b.outpostTurn){
		result[0]++;
		result[result[0]] = 5;
	}
	
	if(a.whoseTurn != b.whoseTurn){
		result[0]++;
		result[result[0]] = 6;
	}
	
	if(a.phase != b.phase){
		result[0]++;
		result[result[0]] = 7;
	}
	
	if(a.numActions != b.numActions){
		result[0]++;
		result[result[0]] = 8;
	}
	
	if(a.coins != b.coins){
		result[0]++;
		result[result[0]] = 9;
	}
	
	if(a.numBuys != b.numBuys){
				result[0]++;
		result[result[0]] = 10;
	}
	
	for(i = 0; i < MAX_PLAYERS; i++)
		for(j = 0; j < MAX_HAND; j++)
			if(a.hand[i][j] != b.hand[i][j])
				players[i] = 1;
	for(i = 0; i < MAX_PLAYERS; i++)
		if(players[i] == 1){
			result[0]++;
			result[result[0]] = 100 + i;
	}
	
	for(i = 0; i < MAX_PLAYERS; i++)
		if(a.handCount[i] != b.handCount[i]){
			result[0]++;
			result[result[0]] = 11;
			break;
	}
	
	for(i = 0; i < MAX_PLAYERS; i++)
		players[i] = 0;	
	for(i = 0; i < MAX_PLAYERS; i++)
		for(j = 0; j < MAX_DECK; j++){
			if(a.deck[i][j] != b.deck[i][j])
				players[i] = 1;
		}
	for(i = 0; i < MAX_PLAYERS; i++)
		if(players[i] == 1){
			result[0]++;
			result[result[0]] = 200 + i;
	}
	
	for(i = 0; i < MAX_PLAYERS; i++)
		if(a.deckCount[i] != b.deckCount[i]){
			result[0]++;
			result[result[0]] = 12;
			break;
		}
	for(i = 0; i < MAX_PLAYERS; i++)
		players[i] = 0;	
	for(i = 0; i < MAX_PLAYERS; i++)
		for(j = 0; j < MAX_DECK; j++)
			if(a.discard[i][j] != b.discard[i][j])
				players[i] = 1;
	for(i = 0; i < MAX_PLAYERS; i++)
		if(players[i] == 1){
			result[0]++;
			result[result[0]] = 300 + i;
		}
	for(i = 0; i < MAX_PLAYERS; i++)
		if(a.discardCount[i] != b.discardCount[i]){
			result[0]++;
			result[result[0]] = 13;
			break;
		}

	for(i = 0; i < MAX_DECK; i++)
		if(a.playedCards[i] != b.playedCards[i]){
			result[0]++;
			result[result[0]] = 14;
			break;
		}
	if(a.playedCardCount != b.playedCardCount){
			result[0]++;
			result[result[0]] = 15;
	}
	return result;
}


