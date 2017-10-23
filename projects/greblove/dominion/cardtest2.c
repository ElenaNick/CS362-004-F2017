/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for smithyCase() function 
 *              from dominion.c
 ******************************************************************************/
  
#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus);

int main(){
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 10008;
	int errors = 0;
	int player = 0;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int flag;
	int i, j;
	int* stateCheck;
	int expected[19] = {100, 11, 200, 12, 300, 13, 14, 15};
	initializeGame(2, k, randSeed, &G);
	
	
	printf("\n---------------Testing smithyCase() function-----------\n");
/******************************************************************************
 * 				Test 1
 ****************************************************************************/ 				
	printf("TEST 1: Deck count is more than 3\n");
	G.deckCount[player] = 10;
	for(i = 0; i < G.deckCount[player]; i++)
		G.deck[player][i] = smithy;
	memcpy(&testG, &G, sizeof(struct gameState));
	
	cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);
	
	printf("Hand count increased by 2: ");
	if(G.handCount[player] == testG.handCount[player] + 2)
			printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}		
	
	printf("Deck count decreased by 3: ");
	if(G.deckCount[player] == testG.deckCount[player] - 3)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Discard count didn't change: ");
	if(G.discardCount[player] == testG.discardCount[player])
			printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Played cards count increased by 1: ");
	if(G.playedCardCount == testG.playedCardCount + 1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Discard cards didn't change: ");
	flag = 0;
	for(i = 0; i < testG.discardCount[player]; i++)
		if(G.discard[player][i] != testG.discard[player][i])
			flag = 1;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
		
	printf("Deck cards didn't change: ");
	flag = 0;
	for(i = 0; i < testG.deckCount[player]; i++)
		if(G.deck[player][i] != testG.deck[player][i])
			flag = 1;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Hand lost 1 card and got cards from the deck: ");
	flag = 0;
	for(i = 1; i < testG.handCount[player]; i++)
		if(G.hand[player][i] != testG.hand[player][i])
			flag = 1;
	for(i; i < testG.handCount[player] + 2; i++)
		if(G.hand[player][i] != smithy)
			flag = 1;
	if(flag == 0 && G.hand[player][0] == smithy)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Smithy put in the Played cards pile: ");
	if(G.playedCards[0] == 4)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No other changes in game state: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] > 8){
		printf("FAILED\n");
		errors++;
	}
	else{
		for(i = 1; i < stateCheck[0]; i++){
			flag = 0;
			for(j = 0; j < 9; j++)
				if(stateCheck[i] == expected[j]){
					flag = 1;
					break;
				}
			if(flag == 0)
				break;
		}
		if(flag == 1)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	}	
	free(stateCheck);


/*****************************************************************************
 *                               Test 2
 ***************************************************************************/         	printf("\nTEST 2: Deck count is 0\n");
	memcpy(&G, &testG, sizeof(struct gameState));
	G.deckCount[player] = 0;
	G.discardCount[player] = 10;
	for(i=0; i < 10; i++)
		G.discard[player][i] = gold;
	memcpy(&testG, &G, sizeof(struct gameState));
	
	cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);
	
	printf("Hand count increased by 2: ");
	if(G.handCount[player] == testG.handCount[player] + 2)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}		
	
	printf("Deck count increased by 7: ");
	if(G.deckCount[player] == testG.deckCount[player] + 7)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Discard count is 0: ");
	if(G.discardCount[player] == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Played cards count increased by 1: ");
	if(G.playedCardCount == testG.playedCardCount + 1)
			printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Discard cards changed to -1: ");
	flag = 0;
	for(i = 0; i < testG.discardCount[player]; i++)
		if(G.discard[player][i] != -1)
			flag = 1;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
		
	printf("Deck cards are ex-discard cards: ");
	flag = 0;
	for(i = 0; i < G.deckCount[player]; i++)
		if(G.deck[player][i] != testG.discard[player][i])
			flag = 1;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Hand lost 1 card and got cards from the deck: ");
	flag = 0;
	for(i = 1; i < testG.handCount[player]; i++)
		if(G.hand[player][i] != testG.hand[player][i])
			flag = 1;
	for(i; i < testG.handCount[player] + 2; i++)
		if(G.hand[player][i] != gold)
			flag = 1;
	if(flag == 0 && G.hand[player][0] == gold)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Smithy put in the Played cards pile: ");
	if(G.playedCards[0] == 4)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No other changes in game state: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] > 8){
		printf("FAILED\n");
		errors++;
	}
	else{
		for(i = 1; i < stateCheck[0]; i++){
		flag = 0;
		for(j = 0; j < 9; j++)
			if(stateCheck[i] == expected[j]){
				flag = 1;
				break;
			}
		if(flag == 0)
			break;
		}
		if(flag == 1)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	}	
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
		printf("G %d testG %d\n", a.outpostTurn, b.outpostTurn);
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
