/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for feastCase() function 
 *              from dominion.c
 ******************************************************************************/
  
#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

// Global variable 
int globalflag = 1;

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus)
	;
void* loopCheck(void* a){
	int s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if(s != 0)
		printf("Error setting thread's cancel state\n");
	int bonus = 0;
	struct gameState b;
	memcpy(&b, (struct gameState*)a, sizeof(struct gameState));
	cardEffect(feast, gold, 0, 0, &b, 0, &bonus);
	globalflag = 0;
	return NULL;
}

int main(){
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 10008;
	int errors = 0;
	int player = 0;
	int handpos = 0, choice1 = gold, choice2 = 0, choice3 = 0, bonus = 0;
	int flag = 0;
	int i, j;
	int* stateCheck;
	int expected[19] = {2, 9, 100, 11, 200, 12, 300, 13};
	int totalSleep = 10;
	int makeLoopCheck = 1; 	
	initializeGame(2, k, randSeed, &G);
	
	printf("\n---------------Testing feastCase() function-----------\n");
/******************************************************************************
 * 				Test 1
 ****************************************************************************/ 		
	printf("TEST 1: None of the card chosen left\n");
	G.supplyCount[gold]  = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Checking for infinite loops: ");
	pthread_t checker;
	int result = pthread_create(&checker, NULL, loopCheck, (void*)&G);
	if(result != 0)
		printf("loopChecker was not created\n");

	while(globalflag == 1 && totalSleep > 0){
		sleep(1);
		totalSleep--;
	}
	if(globalflag == 1){
		printf("FAILED. Kindly check for possible infinite loops in this scenario\n");
		errors++;
		int p = 0;
		p = pthread_cancel(checker);
		printf("%d \n", p);
	}	
	else{
		printf("PASSED\n");
		memcpy(&G, &testG, sizeof(struct gameState));
		cardEffect(feast, choice1, choice2, choice3, &G, handpos, &bonus);
		printf("No changes to game state: ");
		if(memcmp(&G, &testG, sizeof(struct gameState)) == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	}
	
/******************************************************************************
 * 				Test 2
 ****************************************************************************/ 		
	printf("\nTEST 2: Chosen card to buy is too expensive\n");
	memcpy(&G, &testG, sizeof(struct gameState));
	G.supplyCount[gold] = 10;
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Checking for infinite loops: ");
	if(makeLoopCheck == 0){
		printf("FAILED. The loop check is currenly switched off for graider convinience. Previous result was 'FAILED'. Turning the check on will flood the output file with 'This card is too expensive!' message\n");
		errors++;
	}

	else{
		printf("flag is %d\n", makeLoopCheck);
	
		globalflag = 1;
		result = pthread_create(&checker, NULL, loopCheck, (void*)&G);
		if(result != 0)
			printf("loopChecker was not created\n");

		while(globalflag == 1 && totalSleep > 0){
			sleep(1);
			totalSleep--;
		}

		if(globalflag == 1){
			printf("FAILED. Kindly check for possible infinite loops in this scenario\n");
			errors++;
			pthread_cancel(checker);
		}	
		else{
			printf("PASSED\n");
			memcpy(&G, &testG, sizeof(struct gameState));
			cardEffect(feast, choice1, choice2, choice3, &G, handpos, &bonus);
			printf("No changes to game state: ");
			if(memcmp(&G, &testG, sizeof(struct gameState)) == 0)
				printf("PASSED\n");
			else{
				printf("FAILED\n");
				errors++;
			}
		}
	}


	
/******************************************************************************
 * 				Test 3
 ****************************************************************************/ 	
	printf("\nTEST 3: Enough of cards and coins\n");
	choice1 = silver;
	memcpy(&G, &testG, sizeof(struct gameState));
	G.supplyCount[silver] = 10;
	G.hand[player][0] = feast;
	G.discardCount[player] = 5;
	for(i = 0; i < 5; i++)
		G.discard[player][i] = smithy;
	memcpy(&testG, &G, sizeof(struct gameState));

	cardEffect(feast, choice1, choice2, choice3, &G, handpos, &bonus);	
	
	printf("The hand count decreased by 1: ");
	if(G.handCount[player] == testG.handCount[player] - 1)
			printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("The discard count increased by 1: ");
	if(G.discardCount[player] == testG.discardCount[player] + 1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("The discard gained one silver card: ");
	for(i = 0; i < testG.discardCount[player] + 1; i++)
		if(G.discard[player][i] == silver)
			flag++;
	if(flag == 1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No change in other cards in discard: ");
	flag = 0;
	for(i = 0; i < testG.discardCount[player]; i++)
		if(G.discard[player][i] != smithy)
			flag++;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No change in deck count: ");
	if(G.deckCount[player] == testG.deckCount[player])
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No change in cards in deck: ");
	flag = 0;
	for(i = 0; i < testG.deckCount[player]; i++)
		if(G.deck[player][i] != testG.deck[player][i])
			flag++;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Feast card was removed from the hand: ");
	flag = 0;
	for(i = 0; i < testG.handCount[player] - 1; i++)
		if(G.deck[player][i] != feast)
			flag++;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("Silver supply count decreased by 1: ");	
	if(G.supplyCount[silver] == testG.supplyCount[silver] - 1)
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
			for(j = 0; j < 8; j++)
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
	printf("\nTotal errors in function feastCase(): %d\n\n", errors);
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
