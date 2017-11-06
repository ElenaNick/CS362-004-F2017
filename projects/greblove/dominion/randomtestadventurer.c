/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-11-05
 * Class: CS362-400
 * Description: this is a random test program for adventurerCase() function 
 *              card dominion.c
 ******************************************************************************/
 
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TESTS 10000

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus);
int checkAdventurer(int player, struct gameState* G, int testResults[][7], int* testSaved, struct gameState* savedG, int* playerSaved);

int main(){
	srand(time(NULL));
	
	struct gameState G;
	struct gameState* savedG = malloc(TESTS*sizeof(struct gameState));
	int* playerSaved = malloc(TESTS*sizeof(int));
	int i, n, j, player;
	int testResults[TESTS][7];
	int testSaved = 0;
	int errors = 0;
	int flag;
	for(n = 0; n < TESTS; n++){
		for(i = 0; i < sizeof(struct gameState); i++){
			 ((char*)&G)[i] = rand()% 256;
		}
		player = rand()%MAX_PLAYERS;
		G.whoseTurn = player;
		G.deckCount[player] = rand()% MAX_DECK;
		for(i = 0; i < G.deckCount[player]; i++)
			G.deck[player][i] = rand()% 27;
		G.discardCount[player] = rand() % MAX_DECK;
		for(i = 0; i < G.discardCount[player]; i++)
			G.discard[player][i] = rand()% 27;
		G.handCount[player] = rand() % MAX_HAND;
		for(i = 0; i < G.handCount[player]; i++)
			G.hand[player][i] = rand()% 27;
		flag = checkAdventurer(player, &G, testResults, &testSaved, savedG, playerSaved);
		if(flag > 0)
			errors++;
	}
	
	printf("--------------------Adventurer card random testing--------------------\n");
	printf("Total test ran: %d\n", TESTS);
	if(errors == 0)
		printf("All tests PASSED\n\n");
	else{
		printf("Tested FAILED: %d\n", errors);
		printf("Distinct failure modes: %d\n", testSaved);
		for(i = 0; i < testSaved; i++){
			printf("Failure %d.\n", i+1);
			printf("Initial game state: \n");
			printf("\tPlayer - %d, deck count - %d, hand count - %d, discard count - %d\n", playerSaved[i], savedG[i].deckCount[playerSaved[i]], savedG[i].handCount[playerSaved[i]], savedG[i].discardCount[playerSaved[i]]);
			int coins = 0;
			for(j = 0; j < savedG[i].deckCount[playerSaved[i]]; j++)
				if(savedG[i].deck[playerSaved[i]][j] == copper || savedG[i].deck[playerSaved[i]][j] == silver || savedG[i].deck[playerSaved[i]][j] == gold)
					coins++;			
			printf("\tInitial number of treasure cards in deck: %d\n", coins);
			for(j = 1; j < testResults[i][0] + 1; j++){
				if(testResults[i][j] == 1000)
					printf("\tResulting deck count: FAILED\n");
				else if(testResults[i][j] == 1001)
					printf("\tResulting hand count: FAILED\n");
				else if(testResults[i][j] == 1002)
					printf("\tResulting discard count: FAILED\n");
				else if(testResults[i][j] == 1003)
					printf("\tAll new cards in discard came from the deck: FAILED\n");
				else if(testResults[i][j] == 1004)
					printf("\tAll new cards in hand are treasure cards: FAILED\n");
				else if(testResults[i][j] == 1005)
					printf("\tNo changes to states of other players or card piles: FAILED\n");
			}

		


		}			
	}	

	//clean up
	 free(savedG);
	 free(playerSaved);
	return 0;
}

/********************************************************************
 *                   checkAdventurer()
 *Description: runs random test on the Adventurer card. If no errors
 *             found, does nothing. If errors are found, prints it
 *             out if such type of error was not met before.
 *******************************************************************/
 int checkAdventurer(int player, struct gameState* G, int testResults[][7], int* testSaved,struct gameState* savedG, int* playerSaved){
	 int i, j, k, index, result,  flag, count = 0;
	 int* stateCheck;
	 struct gameState* testG = malloc(sizeof(struct gameState));
	 int errors = 0;
	 int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	 int coins[] = {0, 0, 0};
	 int expected[19] = {11, 12, 13};
	 testResults[*testSaved][0] = 0;
	 
	expected[3] = 100 + player;
	expected[4] = 200 +player;
	expected[5] = 300 + player;

	
	 //copy G to the testG for future comparisons
	memcpy(testG, G, sizeof(struct gameState));
	 
	 //count treasure cards in deck and record what are they
	 //coins[0] is copper, coins[1] is silver, coins[2] = gold.
	 for(i = G->deckCount[player] - 1; i >= 0 && count < 2; i--){
		 if(G->deck[player][i] == copper){
			 coins[0]++;
			 count++;
		 }
		 if(G->deck[player][i] == silver){
			 coins[1]++;
			 count++;
		 }
		 if(G->deck[player][i] == gold){
			 coins[2]++;
			 count++;
		 }
	 }
	 index = i; //index of the second treasure card if there were 2 of them or == -1 there were not enough.
	 
	 //run the card function on the G
	 result = cardEffect(adventurer, choice1, choice2, choice3, G, handpos, &bonus);
	 
	//Check deck count decreased by all revealed cards
	if(G->deckCount[player] != index + 1){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1000;
		errors++;
	}
	
	//Check if hand count increased by count
	if(G->handCount[player] != testG->handCount[player] + count){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1001;
		errors++;
	}
	
	//Check discardCount increased by index-count + 1 ");
	if(G->discardCount[player] != testG->discardCount[player] - count + 1){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1002;
		errors++;
	}
	
	//New cards in discard came from the deck
	j = 0;
	for(i = testG->discardCount[player]; i < G->discardCount[player]; i++){
		if(testG->deck[player][index + 1 + j] != G->discard[player][i]){
			if(testG->deck[player][index+1+j] != copper || testG->deck[player][index+1+j] != silver || testG->deck[player][index+1+j] != gold){
				testResults[*testSaved][0] +=1;
				testResults[*testSaved][1 + errors] = 1003;
				errors++;
				break;
			}
			else i--;;
		}
		j++;
	}
	
	//Check all new cards in hand are treasure cards
	for(i = G->handCount[player]-1; i > G->handCount[player]- 1 - count; i--){
		if(G->hand[player][i] != copper || G->hand[player][i] != silver || G->hand[player][i] != gold){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1004;
			errors++;
			break;
		}
	}
	
	//Check other changes in game state
	stateCheck = compareStates(*G, *testG);
	flag = 0;
	if(stateCheck[0] > 6){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1005;
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
		if(flag == 0){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1005;
			errors++;
		}
	}
	
	//check if errors are dictinct
	flag = 0;
	count = 0;

	for(i = 0; i < *testSaved; i++){
		if(testResults[i][0]!= testResults[*testSaved][0]){
			flag++;
		}
		else{
			for(j = 1; j <= testResults[i][0]; j++)
				for(k = 1; k <= testResults[i][0]; k++)
					if(testResults[i][j] == testResults[*testSaved][k]){
						count++;
						break;
					}
			if( count != testResults[i][0])
				flag++;
		}
		count = 0;
	}
	//update testSaved if the errors are distinct or whipe the erroros
	if(flag == *testSaved){
		memcpy(&savedG[*testSaved], testG, sizeof(struct gameState));
		playerSaved[*testSaved] = player;
		*testSaved += 1;

	}
		
	///clean up
	free(stateCheck);
	free(testG);
	
	return errors;
 }
/*****************************************************************
 *                   compareStates()
 *Description: campare states of 2 struct gameState. Returns
 *             an array, where the first element is # of dis-
 *             crepancies found, followed by codes for the 
 *             found discrepancies.
 *****************************************************************/
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
