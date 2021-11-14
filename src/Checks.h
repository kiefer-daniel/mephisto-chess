
//////////////////////////
///* CHECK VALID MOVE . H */
////////////////////////////


#ifndef CHECKVALIDMOVE_H
#define CHECKVALIDMOVE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ClientConst.h"
#include <assert.h>
#include "AIUtilities.h"

int CheckValid( int move[4], GAME * game); 
int CheckDanger(int pieceX, int pieceY, int whiteFlag, int boardArr[8][8]); 
int EndGame(GAME *game);
void PrintEndGame(int code, GAME *game);
int CheckCheck( GAME * game);

#endif /* CHECKVALIDMOVE_H */

/* EOF */ 
