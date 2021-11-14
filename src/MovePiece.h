
////////////////////
/* MOVE PIECE . H */
////////////////////

#ifndef MOVEPIECE_H
#define MOVEPIECE_H

#include <ctype.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ClientConst.h"

int SmithToArray(char moveEntry[], int locMove[]);
void MovePiece( int move[], GAME * game, char promoFromLoad[8]); 
char * PrintErrorCode(int illegalMove, int count);

#endif /* MOVEPIECE_H */

/* EOF */
