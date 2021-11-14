
///////////////
/* CHECKS. c */
///////////////

#include "Checks.h"

int CheckValid( int move[4], GAME * game) {

	int whiteFlag;
	int orig = game->board[move[0]][move[1]];
	int dest = game->board[move[2]][move[3]];
	int moveX = move[2] - move[0];
	int moveY = move[3] - move[1];
	
	if (game->count%2 == 0) { 
		whiteFlag = 1;
	} else { whiteFlag = -1;} /* esle */
	
	/* Not moving (aka moving to the same spot you originated) */
	if ( moveX == 0 && moveY  == 0 ) {
		return 50;
	} /* fi */

	/* Trying to move opponent's piece */
	if ( orig * whiteFlag < 0)  {
		return 51;
	} /* fi */

	/* Trying to move piece to location occupied by ally piece */
	if (orig * dest > 0) {
		return 52;
	} /* fi */
	
	int tmpboardArr[8][8];
	int kingX, kingY;
	for (int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			tmpboardArr[i][j] = game->board[i][j];
			if (game->board[i][j] == whiteFlag * 900 || game->board[i][j] == whiteFlag * 901) {
			     kingX = i;
			     kingY = j;
			} /* fi */		
		} /* fi */
	} /* fi */

	/* rules by piece */
	switch ( abs( orig ) ){
		case 0: 
			//printf("No piece at move origin.\n");
			return 53;
			break;

		case 11: /* First move PAWN */
			if ( whiteFlag * moveY > 0 ) {
				return 101;
			} /* fi */
			if ( abs(moveY) < 1 || abs(moveY) > 2 ) {
				return 102;
			} /* fi */
			else if (  abs(moveY) == 1 && moveX == 0 && dest != 0) {
				return 500;
			} /* fi esle */
			else if ( abs(moveY) == 2 && moveX == 0 && ( dest != 0 || game->board[move[2]][(move[1] + move[3])/2] != 0) ) {
				return 500;
			} /* fi esle */
			if ( abs(moveY) == 1 && abs(moveX) == 1 && orig*dest >= 0 && game->enPassFlag != 1 ) {
				return 500;
			} /* fi */
			else if (abs(moveY) == 2  && moveX == 0 && move[2] == 0 ) {
				if ( orig*game->board[move[2]+1][move[3]] == -132 ) { 
					game->enPassFlag = 2;
					game->enPassLoc[0] = 0;
					game->enPassLoc[1] = (move[1] + move[3]) / 2;
				} /* fi */
			} /* fi */
			else if (abs(moveY) == 2 && moveX == 0 && move[2] == 7 ) {
				if ( orig*game->board[move[2]-1][move[3]] == -132 ) {
					game->enPassFlag = 2;
					game->enPassLoc[0] = 7;
					game->enPassLoc[1] = (move[1] + move[3]) / 2;
				} /* fi */
			} /* fi esle */
			else if ( abs(moveY) == 2 && moveX == 0 && ((orig*game->board[move[2]-1][move[3]] == -132) || (orig*game->board[move[2]+1][move[3]] == -132 )) )  {
				game->enPassFlag = 2;
				game->enPassLoc[0] = move[0];
				game->enPassLoc[1] = (move[1] + move[3]) / 2;
			} /* fi esle */
			else if ( abs(moveY) == 1 && abs(moveX) == 1 && orig*dest < 0 ) {
				;
			} /* fi esle */
			else if ( moveX != 0 ) {
				return 103;
			} /* fi esle */
			break;
			
		case 12: /* PAWN that has previously moved */
			if ( whiteFlag * moveY > 0 ) {
				//printf("Illegal move. Pawn must move forward.\n");
				return 101;
			} /* fi */
			if ( abs(moveY) != 1 ) {
				//printf("Illegal move. Pawn may move 1 square.\n");
				return 104;
			} /* fi */
			else if ( abs(moveY) == 1 && moveX == 0 && dest != 0) {
				return 500;
			} /* fi esle */
			else if ( abs(moveY) == 1 && abs(moveX) == 1 && orig*dest == 0 && game->enPassFlag != 1 ) {
				return 103;
			} /* fi esle */		
			else if ( abs(moveY) == 1 && abs(moveX) == 1 && orig*dest == 0 && game->enPassFlag == 1 && ( move[2] != game->enPassLoc[0] || move[3] != game->enPassLoc[1] ) ) {
				return 103;
			} /* fi esle */
			else if ( abs(moveX) == 1 && abs(moveY) == 1 && dest*orig < 0 ){
				;
			} /* fi esle */
			else if ( moveX != 0) {
				//printf("Illegal move pawn. Sideways move only in capture.\n");
				return 103;
			} /* fi esle */
			break;	

		case 50: 
		case 51: /* ROOK */
			if ( moveX * moveY  != 0 ) {
				//printf("Illegal move rook.\n");
				return 500;
			} /* fi */
			if (moveX < 0) {
				for (int x = -1; x > moveX; x--) {
					if (game->board[move[0] + x][move[1]] != 0) {
						//printf("Illegal move. Rook cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi */
			else if (moveX > 0) {
				for (int x = 1; x < moveX; x++) {
					if (game->board[move[0] + x][move[1]] != 0) {
						//printf("Illegal move. Rook cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */
			else if (moveY < 0) {
				for (int y = -1; y > moveY; y--) {
					if (game->board[move[0]][move[1] + y] != 0) {
						//printf("Illegal move. Rook cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */	
			else if (moveY > 0) {
				for (int y = 1; y < moveY; y++) {
					if (game->board[move[0]][move[1] + y] != 0) {
						//printf("Illegal move. Rook cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */	
			break;

		case 30: /* KNIGHT */
			if ( abs(moveX) + abs(moveY) != 3 || abs(moveX) > 2 || abs(moveX) < 1 ) {
				//printf("Illegal move knight.\n"); 
				return 500;
			} /* fi */
			break;

		case 35: /* BISHOP */
			if (abs(moveX) != abs(moveY) ) {
				//printf("Illegal move bishop.\n");
				return 500;
			} /* fi */	
			if (moveX < 0 && moveY > 0) {
				for (int x = -1; x > moveX; x--) {
					if (game->board[move[0] + x][move[1] - x] != 0) {
						//printf("Illegal move. Bishop cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi */
			else if (moveX > 0 && moveY < 0) {
				for (int x = 1; x < moveX; x++) {
					if (game->board[move[0] + x][move[1] - x] != 0) {
						//printf("Illegal move. Bishop cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */
			else if (moveX < 0 && moveY < 0) {
				for (int y = -1; y > moveY; y--) {
					if (game->board[move[0] + y][move[1] + y] != 0) {
						//printf("Illegal move. Bishop cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */	
			else if (moveX > 0 && moveY > 0) {
				for (int y = 1; y < moveY; y++) {
					if (game->board[move[0] + y][move[1] + y] != 0) {
						//printf("Illegal move. Bishop cannot jump.\n");
						return 404;
					} /* fi */
				} /* rof */
			} /* fi esle */	
			break;

		case 100: /* QUEEN */
			if (abs(moveX) != abs(moveY) && ( moveX*moveY != 0 ) ) {
				//printf("Illegal move queen.\n");
				return 500;
			} /* fi */
			if ( abs(moveX) == abs(moveY)) {
				if (moveX < 0 && moveY > 0) {
					for (int x = -1; x > moveX; x--) {
						if (game->board[move[0] + x][move[1] - x] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi */
				else if (moveX > 0 && moveY < 0) {
					for (int x = 1; x < moveX; x++) {
						if (game->board[move[0] + x][move[1] - x] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */
				else if (moveX < 0 && moveY < 0) {
					for (int y = -1; y > moveY; y--) {
						if (game->board[move[0] + y][move[1] + y] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */	
				else if (moveX > 0 && moveY > 0) {
					for (int y = 1; y < moveY; y++) {
						if (game->board[move[0] + y][move[1] + y] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */	 			
			} /* fi */
			if ( moveX*moveY == 0) {
				if (moveX < 0) {
					for (int x = -1; x > moveX; x--) {
						if (game->board[move[0] + x][move[1]] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi */
				else if (moveX > 0) {
					for (int x = 1; x < moveX; x++) {
						if (game->board[move[0] + x][move[1]] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */
				else if (moveY < 0) {
					for (int y = -1; y > moveY; y--) {
						if (game->board[move[0]][move[1] + y] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */	
				else if (moveY > 0) {
					for (int y = 1; y < moveY; y++) {
						if (game->board[move[0]][move[1] + y] != 0) {
							//printf("Illegal move. Queen cannot jump.\n");
							return 404;
						} /* fi */
					} /* rof */
				} /* fi esle */	
			} /* fi */
			break;

		case 900: /* KING that has never moved */
			/* checking castling conditions */
			if (moveY == 0 && whiteFlag == 1 && move[2] == 6 && moveX == 2 && game->board[7][7] == 50 && game->board[6][7] == 0 && game->board[5][7] == 0 ) {
				if ( !CheckDanger(4, 7, whiteFlag, tmpboardArr) && !CheckDanger(6, 7, whiteFlag, tmpboardArr) && !CheckDanger(5, 7, whiteFlag, tmpboardArr) ) {
					game->castleFlag = 1;
				} /* fi */
				else { return 500; } /* esle */
			} /* fi */
			else if ( moveY == 0 && whiteFlag == 1 && move[2] == 2 && moveX == -2 && game->board[0][7] == 50 && game->board[1][7] == 0 && game->board[2][7] == 0 && game->board[3][7] == 0 ) {
				if ( !CheckDanger(4, 7, whiteFlag, tmpboardArr) && !CheckDanger(2, 7, whiteFlag, tmpboardArr) && !CheckDanger(3, 7, whiteFlag, tmpboardArr) ) {
					game->castleFlag = 1;
				} /* fi */
				else { return 500; } /* esle */
			} /* fi esle */
			else if (moveY == 0 && whiteFlag == -1 && move[2] == 6 && moveX == 2 && game->board[7][0] == -50 && game->board[6][0] == 0 && game->board[5][0] == 0 ) {
				if ( !CheckDanger(4, 0, -1, tmpboardArr) && !CheckDanger(5, 0, -1, tmpboardArr) && !CheckDanger(6, 0, -1, tmpboardArr) ) {
						game->castleFlag = 1;
				} /* fi */
				else { return 500; } /* esle */
			} /* fi esle */	
			else if ( moveY == 0 && whiteFlag == -1 && move[2] == 2 && moveX == -2 && game->board[0][0] == -50 && game->board[1][0] == 0 && game->board[2][0] == 0 && game->board[3][0] == 0 ) {
				if ( !CheckDanger(4, 0, -1, tmpboardArr) && !CheckDanger(2, 0, -1, tmpboardArr) && !CheckDanger(3, 0, -1, tmpboardArr) ) {
					game->castleFlag = 1;
				} /* fi */
				else { return 500;} /* esle */		
			} /* fi esle */
			else if ( abs(moveX) > 1 || abs(moveY) > 1 ) {
				//printf("Illegal move king.\n");
				return 500; 
			} /* fi esle */
			break; 

		case 901: /* KING */
			if ( abs(moveX) > 1 || abs(moveY) > 1 ) {
				/* illegal king move */
				return 500;
			} /* fi */
			break;
		default: 
			break;
	} /* hctiws */

	/* lastly, check if the move made will put the king in check */ 
	tmpboardArr[move[0]][move[1]] = 0; 
	tmpboardArr[move[2]][move[3]] = orig; 

	if (abs(orig) == 900 || abs(orig) == 901) {
		kingX = move[2];
		kingY = move[3];
	} /* fi */

	if (CheckDanger(kingX, kingY, whiteFlag, tmpboardArr)) {
		//printf("King in check. Illegal move.\n");
		return 999; 
	} /* fi */
	
	return 0; /* move is valid */
}

int CheckDanger(int pieceX, int pieceY, int whiteFlag, int boardArr[8][8])
{
	int square = 0;
	int tmpcount = 0;
	int tmparr[4] = {1, 2, 2, 1}; 
	int i;
	
	// check 8 places for knights 
	for( i = -2; i <= 2; i++) {
		if(i == 0) { 
			i++; 
		} /* fi */
	
		if(0 <= pieceX + i && pieceX + i <= 7 && 0 <= (pieceY + tmparr[tmpcount]) && (pieceY + tmparr[tmpcount]) <= 7) { 
	        	square = boardArr[pieceX + i][pieceY + tmparr[tmpcount]];
	        	if(square == (whiteFlag * -30)) {
				return 1;
			} /* fi */ 
	    	} /* fi */
	    	
		if(0 <= pieceX + i && pieceX + i <= 7 && 0 <= (pieceY - tmparr[tmpcount]) && (pieceY - tmparr[tmpcount]) <= 7) {
			square = boardArr[pieceX + i][pieceY - tmparr[tmpcount]];
	        	if(square == (whiteFlag * -30)) {
		    		return 1; 
			} /* fi */
		} /* fi */
		tmpcount++; 
	} /* rof */	

	// check 9 squares for kings 
 	square = 0;
	for(int x = -1; x <= 1; x++) { 
		for (int y = -1; y <= 1; y++) {
			if((0 <= pieceX + x) && (pieceX + x) <= 7 && 0 <= (pieceY + y) && (pieceY + y) <= 7 && !(x == 0 && y == 0)) {
				square = boardArr[pieceX + x][pieceY + y];
				if(square == whiteFlag * -900 || square == whiteFlag * -901) {
					return 1; 
				} /* fi */
			} /* fi */	 
		} /* fi */	
	} /* fi */ 
	
	square = 0;
	// check all squares in immediate front two diagonals (pawn, queen, bishop)
	for ( i = -1; i <= 1; i+=2 ) {
		if (0 <= (pieceX + i) && (pieceX + i) <= 7 && 0 <= (pieceY - (1 *(whiteFlag)) && pieceY - (1 * (whiteFlag)) <= 7)) {
			square = boardArr[pieceX + i][pieceY - (1 * whiteFlag)];
			if (square  == (whiteFlag * -100) || square == (whiteFlag * -12) || square == (whiteFlag * -11) || square == (whiteFlag * -35)) {
				return 1; 
			} /* fi */
		} /* fi */
	} /* rof */
	 
	/* check all squares in front for white, back for black (rook, queen) */
	i = 1;
	square = 0; 

	 
	// while the square is either empty or not a friendly piece, and haven't reached end of the board
	while (0 <= (pieceY - i) && (pieceY - i) <= 7) {
		square = boardArr[pieceX][pieceY - i];
		if (square == (whiteFlag * -50) || square == (whiteFlag * -51) || square == (whiteFlag * -100)) {
			return 1; 
		} /* fi */
		else if (square == 0) { 
			i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */

	// check all squares in front for black, back for white (rook, queen)
	square = 0; 
	i = 1; 
	while ( 0 <= (pieceY + i) && (pieceY + i) <= 7) {
		square = boardArr[pieceX][pieceY + i];
		if (square == (whiteFlag * -50) || square == (whiteFlag * -51) || square == (whiteFlag * -100)) {
			return 1; 
		} /* fi */ 
	    	else if (square == 0) { 
			i++;
			continue; 
	    	} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */

	// check all squares right and left (rook, queen)
	square = 0; 
	i = 1;
	while (0 <= (pieceX - i) && (pieceX - i) <= 7) {
		square = boardArr[pieceX - i][pieceY]; 
		if (square == (whiteFlag * -50) || square == (whiteFlag * -51) || square == (whiteFlag * -100)) {
			return 1; 
		} /* fi */
		else if (square == 0) { 
	 		i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */	

	square = 0;
	i = 1; 
	while (0 <= (pieceX + i) && (pieceX +i) <= 7) {
		square = boardArr[pieceX + i][pieceY]; 
		if ( square == (whiteFlag * -50) || square == (whiteFlag * -51) || square == (whiteFlag * -100)) {
			return 1; 
		} /* fi */
		else if (square == 0) { 
			i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */

	// check all squares in one diagonal (queen, bishop)
	square = 0; 
	i = 1; 
	while (0 <= (pieceX + i) && (pieceX + i) <= 7 && 0 <= (pieceY - i) && (pieceY - i) <= 7) {
		square = boardArr[pieceX + i][pieceY - i]; 
		if ( square == (whiteFlag * -35) || square == (whiteFlag * -100)) {
	 		return 1;
		} /* fi */
		else if (square == 0) { 
			i++;
			continue; 
		} /* elihw */
		else { 
			break; 
		} /* esle */
	} /* elihw */
	
	square = 0;
	i = 1; 
	while (0 <= (pieceX - i) && (pieceX - i) <= 7 && 0 <= (pieceY + i) && (pieceY + i) <= 7) {
		square = boardArr[pieceX - i][pieceY + i];
		if ( square == (whiteFlag * -35) || square == (whiteFlag * -100)) {
			return 1; 
		} /* fi */ 
		else if (square == 0) { 
			i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */
	
	// check all squares in other diagonal (queen, bishop) 
	square = 0; 
	i = 1; 
	while (0 <= (pieceX - i) && (pieceX - i) <= 7 && 0 <= (pieceY - i) && (pieceY - i) <= 7) {
		square = boardArr[pieceX - i][pieceY - i]; 
		if ( square == (whiteFlag * -35) || square == (whiteFlag * -100)) {
	 		return 1;
		} /* fi */
		else if (square == 0) { 
			i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */

	
	square = 0; 
	i = 1; 
	while (0 <= (pieceX + i) && (pieceX + i) <= 7 && 0 <= (pieceY + i) && (pieceY + i) <= 7) {
		square = boardArr[pieceX + i][pieceY + i]; 
		if ( square == (whiteFlag * -35) || square == (whiteFlag * -100)) {
			return 1;
		} /* fi */
		else if (square == 0) { 
			i++;
			continue; 
		} /* fi esle */
		else { 
			break; 
		} /* esle */
	} /* elihw */
	return 0; 	
} 
	    	 


/* check if king is in check */
/* used in different context than check danger */
int CheckCheck( GAME * game) {

	int kingX, kingY; 
	int whiteFlag;

	if (game->count%2 == 0) {
		whiteFlag = 1;
	} /* fi */
	else {
		whiteFlag = -1; 
	} /* esle */

	// get position of the king 
	for (int i = 0; i < 8; i++) {
		for (int j =0; j < 8; j++) {
			if(game->board[i][j] == whiteFlag * 900 || game->board[i][j] == whiteFlag * 901) {
				kingX = i;
				kingY = j; 			
			} /* fi */		
		} /* rof */
	} /* fi */
	if (CheckDanger(kingX, kingY, whiteFlag, game->board) == 1 )  {
		return 1;
	} /* fi */
	else {
		return 0;
	} /* esle */
}	

/* EOF */ 
