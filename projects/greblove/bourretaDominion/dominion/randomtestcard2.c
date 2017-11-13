/******************************************************************************
 * Author: Elena Greblova
 * Date: 2017-11-05
 * Class: CS362-400
 * Description: this is a random test program for councile_roomCase() function 
 *              card dominion.c
 ******************************************************************************/
 
#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TESTS 10000
#define CASES 9

int* compareStates(struct gameState a, struct gameState b);
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int* bonus);
int checkCouncil(int player, struct gameState* G, int testResults[][CASES], int* testSaved, struct gameState* savedG, int* playerSaved, int handPos);


int main(){
	srand(time(NULL));
	
	struct gameState G;
	struct gameState* savedG = malloc(TESTS*sizeof(struct gameState));
	int* playerSaved = malloc(TESTS*sizeof(int));
	int i, j, n, player;
	
	int testResults[TESTS][CASES];
	int testSaved = 0;
	int errors = 0;
	int flag, handPos;
	
	//generate random game state and run tests
	for(n = 0; n < TESTS; n++){
		for(i = 0; i < sizeof(struct gameState); i++){
			 ((char*)&G)[i] = rand()% 256;
		}
		G.numPlayers = (rand()% MAX_PLAYERS)+1;
		player = rand() % G.numPlayers;
		G.whoseTurn = player;
		G.deckCount[player] = rand()% MAX_DECK;
		for(i = 0; i < G.deckCount[player]; i++)
			G.deck[player][i] = rand()% 27;

		G.discardCount[player] = rand() % MAX_DECK;
		for(i = 0; i < G.discardCount[player]; i++)
			G.discard[player][i] = rand()% 27;

		G.playedCardCount = rand() % MAX_DECK;

		G.handCount[player] = rand() % MAX_HAND;

		if(G.handCount[player] > 0)
			handPos = rand() % G.handCount[player];
		else{
			G.handCount[player] = 1;			
			handPos = 0;
		}
		G.hand[player][handPos] = 8;

		for(j = 0; j < G.handCount[player]; j++)
			if(j != handPos)
				G.hand[player][j] = rand()% 27;
		G.numBuys = 0;
		flag = checkCouncil(player, &G, testResults, &testSaved, savedG, playerSaved, handPos);
		if(flag > 0)
			errors++;
	}
	
	printf("--------------------Council_room card random testing--------------------\n");
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
					printf("\tResulting hand count for the player: FAILED\n");
				else if(testResults[i][j] == 1001)
					printf("\tResulting hand count for other players: FAILED\n");
				else if(testResults[i][j] == 1002)
					printf("\tResulting deck count for the player: FAILED\n");
				else if(testResults[i][j] == 1003)
					printf("\tResulting deck count for the other players: FAILED\n");
				else if(testResults[i][j] == 1004)
					printf("\tPlayed cards count increased: FAILED\n");
				else if(testResults[i][j] == 1005)
					printf("\tCouncil room card was discarded: FAILED\n");
				else if(testResults[i][j] == 1006)
					printf("\tNumber of buys increased: FAILED\n");
				else if(testResults[i][j] == 1007)
					printf("\tNo changes to the states of other players and unrelated game state: FAILED\n");
			}

		


		}			
	}	
	
	free(savedG);
	free(playerSaved);
	return 0;
}

/********************************************************************
 *                   checkCouncil()
 *Description: runs random test on the Council. If no errors
 *             found, does nothing. If errors are found, saves 
 *             distinct failure modes.
 *******************************************************************/
 int checkCouncil(int player, struct gameState* G, int testResults[][CASES], int* testSaved,struct gameState* savedG, int* playerSaved, int handPos){
	 int i, j, k, result,  flag, count;
	 int* stateCheck;
	 struct gameState* testG = malloc(sizeof(struct gameState));
	 int errors = 0;
	 int choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	 int expected[19] = {10, 11, 12, 14, 15};
	 testResults[*testSaved][0] = 0;
	 for(i = 0; i < G->numPlayers; i++)
		 expected[5 + i] = 100 + i;
	
	
	 //copy G to the testG for future comparisons
	memcpy(testG, G, sizeof(struct gameState));
	 
	
	 
	 //run the card function on the G
	 cardEffect(council_room, choice1, choice2, choice3, G, handPos, &bonus);
	 
	//Check hand count increased by 4 for the chosen player
	if(G->handCount[player] != testG->handCount[player] + 4){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1000;
		errors++;
	}

	//Check hand count increased by 1 for the rest of  players
	for(i = 0; i < G->numPlayers; i++){
		if(i != player)
			if(G->handCount[i] != testG->handCount[i] + 1){
				testResults[*testSaved][0] +=1;
				testResults[*testSaved][1 + errors] = 1001;
				errors++;
				break;
			}
	}
	
	//Check deck count decreased by 4 for player
	if(G->deckCount[player] != testG->deckCount[player] - 4){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1002;
		errors++;
	}
	
	//Check deck count decreased by 1 for other player
	for(i = 0; i < G->numPlayers; i++)
		if(i != player)
			if(G->deckCount[player] != testG->deckCount[player] - 1){
				testResults[*testSaved][0] +=1;
				testResults[*testSaved][1 + errors] = 1003;
				errors++;
				break;
	}
	
	
	//Check played cards count increased by 1
	if(G->playedCardCount != testG->playedCardCount + 1){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1004;
		errors++;
	}
	
	//Check council_room put in the Played cards pile
	if(G->playedCards[testG->playedCardCount] != 8){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1005;
		errors++;
	}
	
	//Check number of buys increased by 1
	if(G->numBuys != testG->numBuys + 1){
		printf("%d %d\n", G->numBuys, testG->numBuys);
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1006;
		errors++;
	}




	//Check other changes in game state
	stateCheck = compareStates(*G, *testG);
	flag = 0;
	if(stateCheck[0] > 7){
		testResults[*testSaved][0] +=1;
		testResults[*testSaved][1 + errors] = 1007;
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
		if(flag == 0){
			testResults[*testSaved][0] +=1;
			testResults[*testSaved][1 + errors] = 1007;
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
