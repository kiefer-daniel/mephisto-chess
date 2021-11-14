
////////////////
/* FILEIO . C */
////////////////

#include "MovePiece.h"
#include "Checks.h"
#include "GameFunctions.h"
#include "FileIO.h"

/* write move to logfile */
void WriteToLog(GAME * game, char move[], int locMove[], FILE * svFile) {

	assert("./LogFile.txt");
	svFile = fopen("./LogFile.txt", "a");

	int whiteFlag;
	char piece;

	fprintf(svFile, "\nMove %d\n", game->count+1);
	if (game->count%2 == 0) {
		fprintf(svFile, "White");
		whiteFlag = 1;
	} /* fi */
	else {
		fprintf(svFile, "Black");
		whiteFlag = -1;
	} /* esle */ 
	fprintf(svFile, "\n%s", move);

	/* append captured piece */
	if ( game->captureFlag == 1) {
		/* if white's turn, then black piece was captured */
		if (whiteFlag == 1) {
			piece = game->captures->blackCapt[game->captures->blackCount];
		} /* fi */
		else {
			piece = game->captures->whiteCapt[game->captures->whiteCount];
		} /* esle */
		fprintf(svFile, "%c", piece);
	} /* fi */
	else if ( game->enPassCapt == 1 ) {
		fprintf(svFile, "p");
		game->enPassCapt = 0;
	} /* fi esle */

	if (game->checkFlag == 1) {
		fprintf(svFile, "+");
	} /* fi */
	else if (game->checkFlag == 2) {
		fprintf(svFile, "#");
	} /* fi esle */

	if ( game->pawnPromoFlag == 1 ) {
		char promoPiece[8];
		ValueToPiece(game->board[locMove[2]][locMove[3]], promoPiece);
		fprintf(svFile, "\nPawn promoted to %s", promoPiece);
	} /* fi */

	fprintf(svFile, "\n\n----------------------\n");
	
	fclose(svFile);
	return;
}

/* translate from array notation to smith notation */	
void ArrayToSmith( char smith[], int move[]) {
	
	assert(move);
	
	for(int x = 0; x < 4; x+=2){
		switch (move[x]) {
			case 0: 
				smith[x] = 'a';
				break;
			case 1:
				smith[x] = 'b';
				break;
			case 2:
				smith[x] = 'c';
				break;
			case 3: 
				smith[x] = 'd';
				break;
			case 4:
				smith[x] = 'e';
				break;
			case 5:
				smith[x] = 'f';
				break;
			case 6:
				smith[x] = 'g';
				break;
			case 7:
				smith[x] = 'h';
				break;
			default: 
				break;
		} /* hctiws */
	} /* rof */
	for(int x = 1; x < 5; x+=2){
		switch (move[x]) {
			case 0: 
				smith[x] = '8';
				break;
			case 1:
				smith[x] = '7';
				break;
			case 2:
				smith[x] = '6';
				break;
			case 3: 
				smith[x] = '5';
				break;
			case 4:
				smith[x] = '4';
				break;
			case 5:
				smith[x] = '3';
				break;
			case 6:
				smith[x] = '2';
				break;
			case 7:
				smith[x] = '1';
				break;
			default: 
				break;
		} /* hctiws */
	} /* rof */
	smith[4] = '\0';

	return;
} 
	
int PieceToValue(char piece){
	int value;
	
	switch (piece){
		case 'p':
			value = 11;
			break;
		case 'R':
			value = 51;
                        break;
		case 'r':
			value = 50;
			break;
		case 'n':
			value = 30;
                        break;
		case 'b':
			value = 35;
                        break;
		case 'q':
			value = 100;
                        break;
		case 'k':
			value = 900;
                        break;
		default:
			break;
	} /* hctiws */
	return value;
}

void ValueToPiece(int value, char piece[8]){
	
	switch ( abs(value) ){
		case 11:
		case 12:	
			strcpy(piece, "pawn");
			break;
		case 50:
		case 51:
			strcpy(piece, "rook");;
                        break;
		case 30:
			strcpy(piece, "knight");;
			break;
		case 35:
			strcpy(piece, "bishop");
                        break;
		case 100:
			strcpy(piece, "queen");;
                        break;
		case 900:
		case 901:
			strcpy(piece, "king");
                        break;
		default:
			break;
	} /* hctiws */
	return;
}

void DeleteSavedGame( char saveName[]) {

	/* remove file ext if present and append .txt */
	char * fileName = strtok(saveName, ".");
	strcat(fileName, ".txt");
	char filePath[100] = "./";
	strcat(filePath, fileName);

	int foundGame = FindSavedGame( saveName );
	if ( foundGame == 0 ) {
		printf("Unable to delete %s.\n", fileName);
		return;
	} /* fi */

	int removed = remove(filePath);
	if (removed == 0){
		printf("%s deleted.\n", fileName);
	} /* fi */
	else {
		printf("Unable to delete %s.\n", fileName);
	} /* esle */
	return;
} 

/* returns 0 if game not found */
/* returns 1 if game found */
/* returns 2 if no saved games in local directory */
int FindSavedGame( char saveName[] ) {

	/* make TempFile.txt in local directory so never empty */
	FILE * temp = fopen("./TempFile.txt", "w");
	fclose(temp);
	
	/* print names of local .txt files to lsFile.txt */
	system("ls ./*.txt > ./lsFile.txt");	
	char * save = strtok(saveName, ".");
	strcat(save, ".txt");
	char path[100] = "./";
	strcat(path, save);
	char saveGame[50];
	int count = 0;

	/* count how many files are in save directory */
	FILE * file1 = fopen("./lsFile.txt", "r");
	if (!file1){
                printf("The file is empty.\n");
                return 0;
        }

	while ( (fscanf(file1, "%s\n", saveGame) ) != EOF){
       		count++;
	} /* elihw */
	fclose(file1);

	if (count == 1) {
		remove("./lsFile.txt");
		remove("./TempFile.txt");
		return 2;
	} /* fi */
	else {
		FILE * file2 = fopen("./lsFile.txt", "r");
		while ( (fscanf(file2, "%s\n", saveGame) ) != EOF){
			if (strcmp(saveGame, path) == 0 ) {
				remove("./TempFile.txt");
				remove("./lsFile.txt");
				fclose(file2);
				return 1;
			} /* fi */
        	} /* elihw */
		fclose(file2);
	} /* esle */

	remove("./TempFile.txt");
	remove("./lsFile.txt");
	return 0;
}

/* Print saved games */
int PrintSavedGames(void){
	FILE * file;
	char game[50];
	int count = 0;

	/* make TempFile.txt in /save/ directory so never empty */
	FILE * temp = fopen("./TempFile.txt", "w");
	fclose(temp);

	system("ls ./*.txt > ./lsFile.txt");	
	file = fopen("./lsFile.txt", "r");
	
	// should never tbe NULL because there is a TempFile.txt
	if (file == NULL){
		printf("\nNo past games found.\n");
		printf("Exiting game...\n\n");
		exit(10);
	} /* fi */ 
	
	printf("\nSave files: \n");
	while ( (fscanf(file, "%s", game) ) != EOF){
		if (strcmp(game, "./TempFile.txt") == 0 || strcmp(game, "./lsFile.txt") == 0 ) {
			continue;
		} /* fi */
		else {
			/* removes the .txt end of string and the './' at beginning */
			char * gameName = strtok( game, ".");	
			printf("%s\n", gameName+1);
			count++;
		} /* esle */
        } /* elihw */
	printf("\n");

	fclose(file);
	system("rm ./lsFile.txt");
	return count;
}


int SaveGame( void ) {
	char command[100];
	char saveName[50];
	char yesno[30];
	
	strcpy(command, "cp ./LogFile.txt ./");
	printf("Enter name of save file: ");
	scanf("%s", saveName);
	char * save = strtok(saveName, ".");

	int reprompt = FindSavedGame(save);
	
	while (reprompt == 1) {
		printf("%s already exists. Do you want to overwrite? (y/n): ", save);
		scanf("%s", yesno);
		if (yesno[0] == 'y' || yesno[0] == 'Y') {
			reprompt = 0;
		} /* fi */
		else { 
			printf("Enter name of save file: ");
			scanf("%s", saveName);
			char * save = strtok(saveName, ".");
			reprompt = FindSavedGame(save); 
		} /* esle */
	} /* elihw */

	char * save1 = strtok(save, ".");
	strcat(command, save1);
	strcat(command, ".txt");
	system(command);	
	printf("Game saved to %s.txt\n\n", save);
	
	return 0;
}

GAME * LoadGame(char name[], int * AIcolor, int * diff){

	assert(name);

	int numMoves;
	char lineRead[100];
	char fileMove[4];
	int locMove[4];
	char promo[8];
 
	char * fileName = strtok(name, ".");
	GAME * chessGame = CreateGame();
	char fullFile[50] = "./";
	strcat( fullFile, fileName);
	strcat( fullFile, ".txt");
	FILE * file = fopen(fullFile, "r");

	if (!file){
		printf("The file %s is empty.\n", fullFile);
		return 0;
	}

   	InitializeNewGame(chessGame);
	// skips the text header
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	fscanf(file, "%s", lineRead);
	if ( strcmp(lineRead, "AI") == 0 ) {
		chessGame->AITurnFlag = 1;
		fscanf(file, "%s", lineRead);	
		fscanf(file, "%s", lineRead);
		if (strcmp(lineRead, "White") == 0 ) {
			*AIcolor = 1;
		} /* fi */
		else { *AIcolor = 0; } /* fi */
		fscanf(file, "%s", lineRead);
		fscanf(file, "%s", lineRead);
		if (strcmp(lineRead, "Easy") == 0 ) {
			*diff = 1;
		} /* fi */
		else if (strcmp(lineRead, "Average") == 0 ) {
			*diff = 2;
		} /* fi esle */
		else { *diff = 3; } /* fi */
	} /* fi */

	/* makes each move */	
	while (fscanf(file, "%s", lineRead) != EOF){
		fscanf(file, "%s %d", lineRead, &numMoves);
		fscanf(file, "%s", lineRead);
		fscanf(file, "%s", fileMove);
		SmithToArray(fileMove, locMove);
		fscanf(file, "%s", lineRead);
		if (strcmp(lineRead, "Pawn") == 0) {
			fscanf(file, "%s", lineRead);
			fscanf(file, "%s %s", lineRead, promo);
			fscanf(file, "%s", lineRead);
		} /* fi esle */
		
		MovePiece(locMove, chessGame, promo);
		chessGame -> count++; 
	} /* elihw */
	fclose(file);
	
	/* copy savefile into logfile */
	if ( strcmp( fileName, "LogFile" ) != 0 ) {
		char cmd[100] = "cp ";
		strcat(cmd, fullFile);
		strcat(cmd, " ./LogFile.txt");
		system(cmd);
	} /* fi */	

	return chessGame;
} 

/* EOF */
