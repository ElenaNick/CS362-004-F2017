/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-11-05
 * Class: CS362-400
 * Description: this is a random test program for smithyCase() function 
 *              card dominion.c
 ******************************************************************************/
 
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TESTS 10000
#define CASES 8

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus);
int checkSmithy(int player, struct gameState* G, int testResults[][CASES], int* testSaved, struct gameState* savedG, int* playerSaved);


int main(){
		srand(time(NULL));
	
	struct gameState G;
	struct gameState* savedG = malloc(TESTS*sizeof(struct gameState));
	int* playerSaved = malloc(TESTS*sizeof(int));
	int i, j, n, player;
	
	int testResults[TESTS][CASES];
	int testSaved = 0;
	int errors = 0;
	int flag;
	
	//generate random game state and run tests
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
		G.playedCardCount = rand() % MAX_DECK;
		flag = checkSmithy(player, &G, testResults, &testSaved, savedG, playerSaved);
		if(flag > 0)
			errors++;
	}
	
	printf("--------------------Smithy card random testing--------------------\n");
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
			for(j = 1; j < testResults[i][0] + 1; j++){
				if(testResults[i][j] == 1000)
					printf("\tResulting hand count: FAILED\n");
				else if(testResults[i][j] == 1001)
					printf("\tResulting deck count: FAILED\n");
				else if(testResults[i][j] == 1002)
					printf("\tResulting discard count: FAILED\n");
				else if(testResults[i][j] == 1003)
					printf("\tResulting played cards count: FAILED\n");
				else if(testResults[i][j] == 1004)
					printf("\tAll deck cards came from discard (initial deck count < 3): FAILED\n");
				else if(testResults[i][j] == 1005)
					printf("\tSmithy card was discarded: FAILED\n");
				else if(testResults[i][j] == 1006)
					printf("\tNo changes to the states of other players and unrelated game state: FAILED\n");
			}

		


		}			
	}	
	
	free(savedG);
	free(playerSaved);
	return 0;
}

/********************************************************************
 *                   checkSmithy()
 *Description: runs random test on the Smithy card. If no errors
 *             found, does nothing. If errors are found, prints it
 *             out if such type of error was not met before.
 *******************************************************************/
 int checkSmithy(int player, struct gameState* G, int testResults[][CASES], int* testSaved,struct gameState* savedG, int* playerSaved){
	 int i, j, k, result,  flag, count, handPos;
	 int* stateCheck;
	 struct gameState* testG = malloc(sizeof(struct gameState));
	 int errors = 0;
	 int choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	 int expected[19] = {11, 12, 13, 14, 15};
	 testResults[*testSaved][0] = 0;
	 
	expected[5] = 100 + player;
	expected[5] = 200 +player;
	expected[7] = 300 + player;
	
	if(G->handCount[player] > 0)
		handPos = rand() % G->handCount[player];
	else{
	       G->handCount[player] = 1;	
		handPos = 0;
	}
	G->hand[player][handPos] = 4;
	
	 //copy G to the testG for future comparisons
	memcpy(testG, G, sizeof(struct gameState));
	 
	
	 
	 //run the card function on the G
	 cardEffect(smithy, choice1, choice2, choice3, G, handPos, &bonus);
	 
	//Check hand count increased by 2
	if(G->handCount[player] != testG->handCount[player] + 2){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1000;
		errors++;
	}

	
	//Check deck count changed accordinely 
	if(testG->deckCount[player] < 3){
		if(G->deckCount[player] != testG->discardCount[player] - (3 - testG->deckCount[player])){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1001;
			errors++;
		}
	}
	else
		if(G->deckCount[player] != testG->deckCount[player] - 3){
						testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1001;
			errors++;
		}

	//Check discard count changed accordinely 
	if(testG->discardCount[player] < 3){
		if(G->discardCount[player] != 0){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1002;
			errors++;
		}
	}
	else
		if(G->discardCount[player] != testG->discardCount[player]){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1002;
			errors++;
		}
		
	//Check played cards count increased by 1
	if(G->playedCardCount != testG->playedCardCount + 1){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1003;
		errors++;
	}	
	
	
	//Check deck cards are ex discard cards if deckCoutn < 3 was the case test
	if(testG->deckCount[player] < 3){
		flag = 0;
		for(i = 0; i < G->deckCount[player]; i++)
			if(G->deck[player][i] != testG->discard[player][i])
				flag = 1;
		if(flag != 0){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1004;
			errors++;
		}
	}
		
	//check smithy card was discarded;
	if(G->playedCards[testG->playedCardCount] != 4){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1005;
		errors++;
	}

	//Check other changes in game state
	stateCheck = compareStates(*G, *testG);
	flag = 0;
	if(stateCheck[0] > 8){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1006;
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
		if(flag == 0){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1006;
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
