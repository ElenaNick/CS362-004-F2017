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

int* compareStates(struct gameState a, struct gameState b);

int main(){
	//initialize the game
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 18;
	int errors = 0;
	int player = 0;
	int result;
	int i;
	int* stateCheck;
	initializeGame(2, k, randSeed, &G);
	
	printf("\n---------------Testing buyCard() function-----------\n");
	
	/*Test1: there are not enouh numBuys, supply count, or coins*/
	printf("TEST 1: conditions when purchase cannot be made\n");
	printf("Number of buys = 0: ");
	G.numBuys = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = buyCard(adventurer, &G);
	if(result == -1 && memcmp(&G, &testG, sizeof(struct gameState)) == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Supply count = 0: ");
	G.numBuys = 1;
	G.supplyCount[adventurer] = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = buyCard(adventurer, &G);
	if(result == -1 && memcmp(&G, &testG, sizeof(struct gameState)) ==0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Not enough coins for purchase: ");
	G.supplyCount[adventurer] = 10;
	G.coins = 3;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = buyCard(adventurer, &G);
	if(result == -1 && memcmp(&G, &testG, sizeof(struct gameState)) == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	/*Test2: there are not enouh numBuys, supply count, or coins*/
	printf("\nTEST 2: valid card purchase\n");
	G.coins = 10;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = buyCard(adventurer, &G);
	printf("Returned value 0: ");
	if(result == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Number of buys decreased by 1: ");
	if(G.numBuys == testG.numBuys - 1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Number of coins decreased by the card value: ");
	if(G.coins == testG.coins - 6)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("An adventurer card was added to discard: ");
	if(G.discard[player][testG.discardCount[player]] == adventurer)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("Supply count decreased by 1: ");
	if(G.supplyCount[adventurer] == testG.supplyCount[adventurer]-1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("Discard count increased by 1: ");
	if(G.discardCount[player] == testG.discardCount[player]+1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No changes in game state except required ones: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] > 7){
		printf("FAILED\n");
		errors++;
	}
	else{
		int count = 0;
		int errCount = 0;
		int j;
		int expected[] = {2, 7, 9, 10, 300, 13};
		for(i = 1; i < stateCheck[0] + 1; i++){
			count = 0;
			for(j = 0; j < 7; j++){
				if(stateCheck[i] == expected[j])
					break;
				else
					count++;
			}
			
				if(count == 7)
				errCount++;
		}
		if(errCount == 0)	
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	}
	
	printf("\nTotal errors in function buyCard(): %d\n\n", errors);
	
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
