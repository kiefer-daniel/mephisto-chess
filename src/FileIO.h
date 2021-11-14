
/////////////////
/* FILE IO . H */
/////////////////

#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ClientConst.h"

void WriteToLog(GAME * game, char move[], int locMove[], FILE * svFile); 
void ArrayToSmith( char smith[], int move[]);
void UndoMove(GAME *game);
int PieceToValue(char piece);
void ValueToPiece(int value, char piece[8]);
int FindSavedGame( char saveName[] );
void DeleteSavedGame( char saveName[]);
int PrintSavedGames(void);
int SaveGame( void ); 
GAME * LoadGame(char name[], int * AIcolor, int * diff);

#endif /* FILEIO_H */

/* EOF */
