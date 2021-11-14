

/////////////////////
/* AI UTILITIES .C */
/////////////////////

#include "AIUtilities.h"

/* checks to see if game has entered late-game */
void CheckLateGame( GAME * game ) {

	int whiteQueen = 0;
	int blackQueen = 0;
	int whiteCaptureCount = 0;
	int blackCaptureCount = 0;

	/* condition 1: only queens & kings left */
	/* condition 2: side has only a queen and minor pieces */
	for (int i = 0; i < game->captures->whiteCount; i++ ){
		if ( game->captures->whiteCapt[i] == 100 ) {
			whiteQueen = 1;
		} /* fi */
		whiteCaptureCount += game->captures->whiteCapt[i];
	} /* rof */
	for (int i = 0; i < game->captures->blackCount; i++ ){
		if ( game->captures->blackCapt[i] == -100 ) {
			blackQueen = 1;
		} /* fi */
		blackCaptureCount += game->captures->blackCapt[i];
	} /* rof */

	if ( (whiteQueen == 0 && blackQueen == 0) || whiteCaptureCount >= 428 || abs(blackCaptureCount) >= 428 ) {
		game->lateGame = 1;
	} /* fi */	
	return;

}

void CopyGame (GAME * game, GAME * tempGame)
{
	int x, y;

	for (x=0; x<8; x++) {
		for (y=0; y<8; y++) {
			tempGame->board[x][y] = game->board[x][y];
		} /* rof */
	} /* rof */

	tempGame->count = game->count;
	tempGame->checkFlag = game->checkFlag;
	tempGame->enPassFlag = game->enPassFlag;

	for (x=0; x<2; x++) 
	{
		tempGame->enPassLoc[x] = game->enPassLoc[x];
		tempGame->lastMove[x] = game->lastMove[x];
	} /* rof */

	tempGame->enPassCapt = game->enPassCapt;
	tempGame->castleFlag = game->castleFlag;
	tempGame->pawnPromoFlag = game->pawnPromoFlag;
	tempGame->AITurnFlag = game->AITurnFlag;
	tempGame->AIThinkingFlag = game->AIThinkingFlag;
	tempGame->lateGame = game->lateGame;

	tempGame->captures->whiteCount = game->captures->whiteCount;
	tempGame->captures->blackCount = game->captures->blackCount;

	for (x=0; x<17; x++) 
	{
		tempGame->captures->whiteCapt[x] = game->captures->whiteCapt[x];
		tempGame->captures->blackCapt[x] = game->captures->blackCapt[x];
	} /* rof */
}

double HeatMap(int origX, int origY, int destX, int destY, GAME * game) {

	int whiteFlag;

	if (game->count%2 == 0 ) {
		whiteFlag = 1;
        } else { whiteFlag = -1; } /* esle */

	const double King[8][8] = 
	{{-3.0, -3.0, -3.0, -3.0, -2.0, -1.0, 2.0, 2.0}, 
	 {-4.0, -4.0, -4.0, -4.0, -3.0, -2.0, 2.0, 3.0}, 
	 {-4.0, -4.0, -4.0, -4.0, -3.0, -2.0, 0.0, 1.0}, 
	 {-5.0, -5.0, -5.0, -5.0, -4.0, -2.0, 0.0, 0.0}, 
	 {-5.0, -5.0, -5.0, -5.0, -4.0, -2.0, 0.0, 0.0},   
	 {-4.0, -4.0, -4.0, -4.0, -3.0, -2.0, 0.0, 1.0},  
	 {-4.0, -4.0, -4.0, -4.0, -3.0, -2.0, 2.0, 3.0}, 
	 {-3.0, -3.0, -3.0, -3.0, -2.0, -1.0, 2.0, 2.0}};

	const double Queen[8][8] = 
	{{-2.0, -1.0, -1.0, -0.5,  0.0, -1.0, -1.0, -2.0}, 
	 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.5,  0.0, -1.0}, 
	 {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.5, -1.0}, 
	 {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5}, 
	 {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5}, 
	 {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0}, 
	 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0}, 
	 {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}};

	const double Rook[8][8] = 
	{{0.0, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.0}, 
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0}, 
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0},  
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.5},  
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.5}, 
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0}, 
	 {0.0, 1.0,  0.0,  0.0,  0.0,  0.0,  0.0, 0.0},  
	 {0.0, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.0}};

	const double Bishop[8][8] = 
	{{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}, 
	 {-1.0,  0.0,  0.0,  0.5,  0.0,  1.0,  0.5, -1.0}, 
	 {-1.0,  0.0,  0.5,  0.5,  1.0,  1.0,  0.0, -1.0}, 
	 {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0}, 
	 {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0}, 
	 {-1.0,  0.0,  0.5,  0.5,  1.0,  1.0,  0.0, -1.0}, 
	 {-1.0,  0.0,  0.0,  0.5,  0.0,  1.0,  0.5, -1.0}, 
	 {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}};
   
	const double openingKnight[8][8] = 
	{{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
	 {-4.0, -2.0,  0.0,  0.5,  0.0,  0.5, -2.0, -2.0},
	 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	 {-4.0, -2.0,  0.0,  0.5,  0.0,  0.5, -2.0, -2.0},
	 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}};

	const double Knight[8][8] = 
	{{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
	 {-4.0, -2.0,  0.0,  0.5,  0.0,  0.5, -2.0, -4.0},
	 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	 {-4.0, -2.0,  0.0,  0.5,  0.0,  0.5, -2.0, -4.0},
	 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}};

	const double Pawn[8][8] = 
	{{ 9.0,  5.0,  1.0,  0.5,  0.0,  0.5,  0.5,  0.0}, 
	 { 9.0,  5.0,  1.0,  0.5,  0.0, -0.5,  1.0,  0.0},
	 { 9.0,  5.0,  2.0,  1.0,  0.0, -1.0,  1.0,  0.0},
	 { 9.0,  5.0,  3.0,  2.5,  2.0,  0.0, -2.0,  0.0},
	 { 9.0,  5.0,  3.0,  2.5,  2.0,  0.0, -2.0,  0.0},
	 { 9.0,  5.0,  2.0,  1.0,  0.0, -1.0,  1.0,  0.0},
	 { 9.0,  5.0,  1.0,  0.5,  0.0, -0.5,  1.0,  0.0},
	 { 9.0,  5.0,  0.0,  0.5,  0.0,  0.5,  0.5,  0.0}}; 

	/* printf("%d %d to %d %d\n", origX, origY, destX, destY); 
	printf("%f minus %f\n", Pawn[destX][destY], Pawn[origX][origY]); */

/*	const double KingMid[8][8] = 
	{{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	 {-3.0, -4.0, -4.0, -5.0, -4.0, -4.0, -4.0, -3.0},
	 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	 {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
	 {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
	 { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
	 { 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}};  */

	const double KingEnd[8][8] = 
	{{-5.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -5.0},
	 {-4.0, -2.0, -1.0, -1.0, -1.0, -1.0, -3.0, -3.0},
	 {-3.0, -1.0,  2.0,  3.0,  3.0,  2.0,  0.0, -3.0},
	 {-2.0,  0.0,  3.0,  4.0,  4.0,  3.0,  0.0, -3.0},
	 {-2.0,  0.0,  3.0,  4.0,  4.0,  3.0,  0.0, -3.0},
	 {-3.0, -1.0,  2.0,  3.0,  3.0,  2.0,  0.0, -3.0},
	 {-4.0, -2.0, -1.0, -1.0, -1.0, -1.0, -3.0, -3.0},
	 {-5.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -5.0}}; 	

	int piece = game->board[origX][origY];

	/* effectively flip black heat maps */
	if (whiteFlag == -1) {
		destY = 7 - destY;
		origY = 7 - origY;
	} /* fi */

	double value;
	if (game->count < 5 && abs(piece) == 30 ) {
		value = openingKnight[destX][destY] - openingKnight[origX][origY];
		return value;
	} /* fi */

	/* apply late-game king map */
	if ( abs(piece) == 900 || abs(piece) == 901 ) {
		if ( game->lateGame == 1 ) {
			value = KingEnd[destX][destY] - KingEnd[origX][origY];
		} /* fi */
		else {	
			value = King[destX][destY] - King[origX][origY];
		} /* esle */
	} /* fi */ 

	switch ( abs(piece) ) {
		case 11:
		case 12:
			value = Pawn[destX][destY] - Pawn[origX][origY];
			break;
		case 30: 
			value = Knight[destY][destX] - Knight[origY][origX];
			break;
		case 35:
			value = Bishop[destY][destX] - Bishop[origY][origX];
			break;
		case 50:
		case 51:
			value = Rook[destY][destX] - Rook[origY][origY];
			break;
		case 100:
			value = Queen[destX][destY] - Queen[origX][origY];
			break;
		default:
			value = 0;
			break;
	} /* hctiws */
	return value;
}

int Value(int abs)
{
	switch(abs)
	{
		case 11:
			return 10;
			break;
		case 12:
			return 10;
			break;
		case 30:
			return 30;
			break;
		case 35:
			return 30;
			break;
		case 50:
			return 50;
			break;
		case 51:
			return 50;
			break;
		case 100:
			return 90;
			break;
		case 900:
			return 900;
			break;
		case 901:
			return 900;
			break;
		case 0:
			return 0;
			break;
		default:
			return 0;
			break;
	}
}



/* EOF */
