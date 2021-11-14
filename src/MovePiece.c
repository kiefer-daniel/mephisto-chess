
////////////////////
/* MOVE PIECE . C */
////////////////////

#include "MovePiece.h"
#include "Checks.h"
#include "FileIO.h"



char * PrintErrorCode(int illegalMove, int count) {
	char currentPlayer[5];
	char * error;

	if(count%2 == 0)
	{
		strcpy(currentPlayer, "White");
	}
	else
	{
		strcpy(currentPlayer, "Black");
	} 

	switch(illegalMove)
	{    // general errors
        case 404:
			error = ("Illegal move. Piece can not jump\n");
			break;
		case 500:
			error = ("Illegal move "); 
			break;
		case 999:
			error = ("Illegal move. King check\n");
			break;	    
	     // non piece specific  
		case 50:
			error = ("Illegal move. Must make a move\n");
			break;
		case 51:
			error = ("Illegal move. Can't move enemy piece\n");
			break;
		case 52:
			error = ("Illegal move. Moved piece may not land on its own color\n"); 
			break; 
		case 53:
			error = ("Illegal move. No piece at move origin\n");
			break; 
	    // pawn 
		case 101:
			error = ("Illegal move. Pawn must move forward\n");
			break;
		case 102:
			error = ("Illegal move. Pawn may only move 1 or 2 squares\n");
			break;
		case 103:
			error = ("Illegal move. Sideways move only when capturing\n");
			break;
		case 104:
			error = ("Illegal move. Pawn may move 1 square\n");
			break;
		default: 
			break;
	} /* hctiws */
	return error;
}

int SmithToArray(char smithMove[], int arrMove[]){
	//Parses through user inputted move
	//Outputs move in the form of x,y to x,y

	assert(smithMove);
	
	int num;
	char tempStr[3];

	for(int x = 0; x < 4; x+=2){
		switch (smithMove[x]) {
			case 'A': 
			case 'a':
				arrMove[x] = 0;
				break;
			case 'B':
			case 'b':
				arrMove[x] = 1;
				break;
			case 'C':
			case 'c':
				arrMove[x] = 2;
				break;
			case 'D':
			case 'd': 
				arrMove[x] = 3;
				break;
			case 'E':
			case 'e':
				arrMove[x] = 4;
				break;
			case 'F':
			case 'f':
				arrMove[x] = 5;
				break;
			case 'G':
			case 'g':
				arrMove[x] = 6;
				break;
			case 'H':
			case 'h':
				arrMove[x] = 7;
				break;
			default: 
				return 1;
		} /* hctiws */
	} /* rof */

	for (int j = 1; j < 5; j+=2) {
		strcpy(tempStr, smithMove + j);
		num = atoi(tempStr);
		if ( num < 1 || num > 8 ) {
			return 1;
		} /* fi */
		else {
			arrMove[j] = 8-num;
		} /* esle */
	} /* rof */	  

	return 0;
}

void MovePiece( int move[], GAME * game, char promoFromLoad[8])  {

	assert(move[0] < 8 && move[0] >=0 );
	assert(move[1] < 8 && move[1] >=0 );
	assert(move[2] < 8 && move[2] >=0 );
	assert(move[3] < 8 && move[3] >=0 );
	assert(game);

	int whiteFlag;
	char promo[10];
	int orig = game->board[move[0]][move[1]];
	int dest = game->board[move[2]][move[3]];

	/* reset GAME flags */
	game->pawnPromoFlag = 0;
	game->captureFlag = 0;
	game->checkFlag = 0;
	game->enPassCapt = 0;
		
	/* whiteFlag is 1 if white turn, -1 if black turn */
	if (game->count%2 == 0) { 
		whiteFlag = 1;
	} else { whiteFlag = -1;} /* esle */

	/* Change pawn from never moved to has moved */
	if ( abs(orig) == 11) {
		orig = whiteFlag*12;
	} /* fi */

	/* Change rook from never moved to has moved */
	if (abs(orig) == 50) {
		orig = whiteFlag*51;
	} /* fi */

	/* handle enpassant capture */
	if (game->enPassFlag == 1 && move[2] == game->enPassLoc[0] && move[3] == game->enPassLoc[1] && whiteFlag*orig == 12 && (whiteFlag * game->board[move[2]][move[1]]) == -12 ) {
		if (whiteFlag == -1) {
			game->captures->whiteCapt[game->captures->whiteCount++] = 'p';
		} /* fi */
		else {
			game->captures->blackCapt[game->captures->blackCount++] = 'p';
		} /* esle */	
		game->board[move[2]][move[1]] = 0;
		game->enPassCapt = 1;
	} /* fi */ 

	/* pawn promotion */
	if (orig == 12*whiteFlag && ( move[3] == 0 || move[3] == 7 ) ) {
		game->pawnPromoFlag = 1;
		if (promoFromLoad) {
			/* if loading from savefile */ 
			switch (promoFromLoad[0]) {
				case 'r': /* rook */
					orig = 51 * whiteFlag;
					break;
				case 'k': /* knight */
					orig = 30 * whiteFlag;
					break;
				case 'b': /* bishop */
					orig = 35 * whiteFlag;
					break;
				case 'q': /* queen */
					orig = 100 * whiteFlag;
					break;
				default:
					break;
			} /* hctiws */
		} /* fi */
		/* AI pawn always promoted to queen */
		else if ( game->AITurnFlag == 1) {
			/* don't print statement if AI is just calculating moves */
			if ( game->AIThinkingFlag == 0 ) { 
				printf("AI pawn promoted to queen.\n");
			} /* fi */
			orig = 100 * whiteFlag;
		} /* fi esle */
		else {
			printf ("Pawn is promoted. Enter character corresponding to desired promotion (Q, R, B, N): ");
			scanf("%s", promo);
			while ( tolower(promo[0]) != 'r' && tolower(promo[0]) != 'q' && tolower(promo[0]) != 'b' && tolower(promo[0]) != 'n') {
				printf ("Pawn may only be promoted to queen, bishop, knight, or rook.\n");
				printf ("Please enter Q, R, B, or N: ");
				scanf("%s", promo);
			} /* elihw */
			switch (tolower(promo[0])) {
				case 'q': 	
					orig = 100 * whiteFlag;
					break;
				case 'b':
					orig = 35 * whiteFlag;
					break;
				case 'n':
					orig = 30 * whiteFlag;
					break;
				case 'r':
					orig = 51 * whiteFlag;
					break;
				default:
					break;
			} /* hctiws */
		} /* esle */
	} /* fi */

	/* TO INDICATE A CAPTURED PIECE */ 
	if ( dest != 0 ) {
		if (dest > 0) {
			switch (dest) {
				case 11:
				case 12: 
					game->captures->whiteCapt[game->captures->whiteCount++] = 'p';
					break;
				case 50:
				case 51: 
					game->captures->whiteCapt[game->captures->whiteCount++] = 'R';
					break;
				case 30: 
					game->captures->whiteCapt[game->captures->whiteCount++] = 'N';
					break;
				case 35: 
					game->captures->whiteCapt[game->captures->whiteCount++] = 'B';
					break;
				case 100: 
					game->captures->whiteCapt[game->captures->whiteCount++] = 'Q';
					break;
				default: 
					break;
			} /* hctiws */
		} /* fi */
		else {
			switch ( abs(dest) ) {
				case 11:
				case 12: 
					game->captures->blackCapt[game->captures->blackCount++] = 'p';
					break;
				case 50:
				case 51: 
					game->captures->blackCapt[game->captures->blackCount++] = 'R';
					break;
				case 30: 
					game->captures->blackCapt[game->captures->blackCount++] = 'N';
					break;
				case 35: 
					game->captures->blackCapt[game->captures->blackCount++] = 'B';
					break;
				case 100: 
					game->captures->blackCapt[game->captures->blackCount++] = 'Q';
					break;
				default: 
					break;
			} /* hctiws */
		} /* esle */
	game->captureFlag = 1;
	} /*  fi */

	/* castling */
	if (game->castleFlag == 1 && abs(orig) == 900 ) {
		if (move[2] == 6) {
			game->board[5][move[1]] = whiteFlag * 51;
			game->board[7][move[1]] = 0;
		} /* fi */
		else if (move[2] == 2) {
			game->board[3][move[1]] = whiteFlag * 51;
			game->board[0][move[1]] = 0;
		} /* fi esle */
	} /* fi */	

	/* Change king from never moved to has moved */
	if (abs(orig) == 900) {
		orig = whiteFlag*901;
	} /* fi */

	game->checkFlag = CheckCheck(game);

	game->castleFlag = 0; 
	game->enPassFlag--;
	game->board[move[2]][move[3]] = orig;	
	game->board[move[0]][move[1]] = 0;
	return;
}

/* EOF */
