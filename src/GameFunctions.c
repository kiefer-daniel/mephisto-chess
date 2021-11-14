
////////////////////////
/* GAME FUNCTIONS . C */
////////////////////////

#include "GameFunctions.h"
#include "MovePiece.h"
#include "Checks.h"
#include "FileIO.h"

GAME * CreateGame(void) {
	GAME * new = (GAME *) malloc( sizeof( GAME ));	
	new->captures = ( CAPTURES * )malloc(sizeof (CAPTURES) );
	
	assert(new);
	assert(new->captures);
	return new;
}

void DeleteGame(GAME * game) {
	assert (game);
	free(game->captures);
	game->captures = NULL;

	free(game);
	game = NULL;

	return;
} 

/* Called once, at the beginning of a game to initialize the board and all flags */
void InitializeNewGame(GAME * game){
	
	int x, y;
	const int defaultBoard[8][8] = 
	{ {-50, -11, 0, 0, 0, 0, 11, 50}, 
	  {-30, -11, 0, 0, 0, 0, 11, 30}, 
	  {-35, -11, 0, 0, 0, 0, 11, 35}, 
	  {-100,-11, 0, 0, 0, 0, 11,100}, 
	  {-900,-11, 0, 0, 0, 0, 11,900}, 
	  {-35, -11, 0, 0, 0, 0, 11, 35}, 
	  {-30, -11, 0, 0, 0, 0, 11, 30}, 
	  {-50, -11, 0, 0, 0, 0, 11, 50} };
 
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			game->board[x][y] = defaultBoard[x][y];
		} /* rof */
	} /* rof */

	game->count = 0;
	game->checkFlag = 0;
	game->enPassFlag = 0;
	game->enPassLoc[0] = 8;
	game->enPassLoc[1] = 8;
	game->enPassCapt = 0;
	game->castleFlag = 0; 
	game->lastMove[0] = 8;
	game->lastMove[1] = 8;
	game->pawnPromoFlag = 0;
	game->AITurnFlag = 0;
	game->AIThinkingFlag = 0;
	game->lateGame = 0;

	game->captures->whiteCount = 0;
	game->captures->blackCount = 0;
	strcpy(game->captures->whiteCapt, "                ");
	strcpy(game->captures->blackCapt, "                ");

	system("clear");
	return;
}
								

/* EOF */
