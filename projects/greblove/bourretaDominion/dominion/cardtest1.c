/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-10-22
 * Class: CS362-400
 * Description: this is a unit test program for adventurerCase() function 
 *              card dominion.c
 ******************************************************************************/
  
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus);

int main(){
	//initialize the game
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int randSeed = 10008;
	int errors = 0;
	int player = 0;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int result, flag;
	int i, j;
	int* stateCheck;
	int expected[19] = {100, 11, 200, 12, 300, 13};
	initializeGame(2, k, randSeed, &G);
	
	printf("\n---------------Testing adventurerCase() function-----------\n");
	printf("TEST 1: Deck count is 0, no treasure cards in the discard\n");
	G.deckCount[player] = 0;
	for(i = 0; i < G.handCount[player]; i++)
		G.hand[player][i] = gardens;
	G.discardCount[player] = 10;
	for(i = 0; i < G.discardCount[player]; i++)
		G.discard[player][i] = smithy;
	memcpy(&testG, &G, sizeof(struct gameState));
	
	result = cardEffect(adventurer, choice1, choice2, choice3, &G, handpos, &bonus);

	printf("No changes to the game state: ");
	if(memcmp(&testG, &G, sizeof(struct gameState)) == 0 && result == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}		

/*****************************************************************************
								Test 2
******************************************************************************/
	printf("\nTEST 2: Deck is not empty, but doesn't have treasure coins. Discard = 0\n");	
	memcpy(&G, &testG, sizeof(struct gameState));
	G.deckCount[player] = 10;
	for(i = 0; i < 10; i++)
		G.deck[player][i] = smithy;
	G.discardCount[player] = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = cardEffect(adventurer, choice1, choice2, choice3, &G, handpos, &bonus);

	printf("Deck count s 0: ");
	if(G.deckCount[player] == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("Hand count didnt change: ");
	if(G.handCount[player] == testG.handCount[player])
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Discard is previous deck count: ");
	if(G.discardCount[player] == testG.deckCount[player])
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Cards in all 3 piles changed/not changed accordinly: \n");
	int hand=0, deck=0, discard=0;
	for(i = 0; i <  G.handCount[player]; i++)
		if(G.hand[player][i] != testG.hand[player][i]){
			hand++;
		}
	for(i = 0; i <  MAX_DECK; i++)
		if(G.deck[player][i] != testG.deck[player][i])
			deck++;
	for(i = 0; i <  testG.deckCount[player]; i++)
		if(G.discard[player][i] != testG.deck[player][i])
			discard++;
	if(hand == 0)
		printf("PASSED in hand\n");
	else{
		printf("FAILED in hand\n");
		for(i = 0; i < G.handCount[player]; i++)
			printf("%d ", G.hand[player][i]);
		printf("\n");
		errors++;
	}
	if(deck == 0)
		printf("PASSED in deck\n");
	else{
		printf("FAILED in deck\n");
		errors++;
	}

	if(discard == 0)
		printf("PASSED in discard\n");
	else{
		printf("FAILED in discard\n");
		errors++;
	}
	
	
	printf("No other changes in game state: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] > 6){
		printf("FAILED\n");
		errors++;
	}
	else{
		for(i = 1; i < stateCheck[0]; i++){
			flag = 0;
			for(j = 0; j < 7; j++)
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
								Test 3
******************************************************************************/
	printf("\nTEST 3: Deck is not empty, 1 treasure card in the deck\n");
	int coins[] = {copper, silver, gold};
	char* coinsNames[] = {"copper", "silver", "gold"};
	for(j = 0; j < 3; j++){
		memcpy(&G, &testG, sizeof(struct gameState));
		for(i = 0; i < 9; i++)
			G.deck[player][i] = smithy;
		G.deck[player][9] = coins[j];
		G.deckCount[player] = 10;

		memcpy(&testG, &G, sizeof(struct gameState));
		result = cardEffect(adventurer, choice1, choice2, choice3, &G, handpos, &bonus);

		printf("Case %s.\nDeck count is 0: ", coinsNames[j]);
		if(G.deckCount[player] == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}	
	
		printf("Hand count increased by 1: ");
		if(G.handCount[player] == testG.handCount[player] + 1)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	
		printf("Discard count is previous deck count - 1: ");
		if(G.discardCount[player] == testG.deckCount[player] - 1)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	
		printf("No new cards in the deck: ");
		flag = 0;
		for(i = 0; i < testG.deckCount[player] - 1; i++)
			if(G.deck[player][i] != testG.deck[player][i]){
				flag = 1;
				break;
			}
		if(flag == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}

		printf("Discard has all non-%s cards from deck: ", coinsNames[j]);
		flag = 0;
		for(i = 0; i < testG.deckCount[player] - 1; i++)
			if(G.discard[player][i] != testG.deck[player][i]){
				flag = 1;
				break;
			}
		if(flag == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	
		printf("No new cards in hand except 1 %s: ", coinsNames[j]);
		flag = 0;
		for(i = 0; i < testG.handCount[player]; i++)
			if(G.hand[player][i] != testG.hand[player][i])
				flag++;
		if(flag > 0)
			if(G.hand[player][testG.handCount[player]] != coins[j])
				flag++;
		if(flag == 0)
			printf("PASSED\n");
		else{
			printf("FAILED\n");
			errors++;
		}
	
		printf("No other changes in game state: ");
		int k;
		stateCheck = compareStates(G, testG);
		if(stateCheck[0] > 6){
			printf("FAILED\n");
			errors++;
		}
		else{
			for(i = 1; i < stateCheck[0]; i++){
				flag = 0;
				for(k = 0; k < 7; k++)
					if(stateCheck[i] == expected[k]){
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
	}
	
	
/*****************************************************************************
								Test 4
******************************************************************************/
	printf("\nTEST 4: Deck is not empty, 5 treasure card in the deck\n");
	memcpy(&G, &testG, sizeof(struct gameState));
	for(i = 0; i < 10; i++)
		if(i%2 == 0)
			G.deck[player][i] = smithy;
		else
			G.deck[player][i] = gold;
	G.deckCount[player] = 10;
	memcpy(&testG, &G, sizeof(struct gameState));

	result = cardEffect(adventurer, choice1, choice2, choice3, &G, handpos, &bonus);	

	printf("Deck count decreased by 3: ");
	if(G.deckCount[player] == testG.deckCount[player] - 3)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}	
	
	printf("Hand count increased by 2: ");
	if(G.handCount[player] == testG.handCount[player] + 2)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("Discard count increased by 1: ");
	if(G.discardCount[player] == testG.discardCount[player] + 1)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No new cards in the deck: ");
	int coin=0, smthy=0;
	for(i = 0; i < testG.deckCount[player]; i++)
		if(G.deck[player][i] == smithy)
			smthy++;
		else if (G.deck[player][i] == silver)
			coin++;	
	if(smthy == 5 && coin == 5)

		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}

	printf("One new card in discard from the deck: ");
	if(G.discard[player][0] == smithy)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No new cards in hand except 2 silver: ");
	flag = 0;
	for(i = 0; i < testG.handCount[player]; i++)
		if(G.hand[player][i] != testG.hand[player][i])
			flag++;
	if(flag == 0)
		if(G.hand[player][testG.handCount[player]] != silver || G.hand[player][testG.handCount[player]+1] != silver)
			flag++;
	if(flag == 0)
		printf("PASSED\n");
	else{
		printf("FAILED\n");
		errors++;
	}
	
	printf("No other changes in game state: ");
	stateCheck = compareStates(G, testG);
	if(stateCheck[0] > 6){
		printf("FAILED\n");
		errors++;
	}
	else{
		for(i = 1; i < stateCheck[0]; i++){
			flag = 0;
			for(j = 0; j < 7; j++)
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
