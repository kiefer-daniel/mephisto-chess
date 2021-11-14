
/////////////////////
/* AI UTILITIES .H */
/////////////////////

#ifndef AIUTILITIES_H
#define AIUTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ClientConst.h"
//#include "CheckValidMove.h"
//#include "GameFunctions.h"

void CopyGame (GAME * game, GAME * tempGame);
double HeatMap(int origX, int origY, int destX, int destY, GAME * game);
void CheckLateGame( GAME * game); 
int Value( int abs);

#endif /* AIUTILITIES_H */


/* EOF */

