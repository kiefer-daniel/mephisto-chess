

/////////////////////////////////
/*        CONSTANTS.H          */
/////////////////////////////////


#ifndef CLIENTCONST_H
#define CLIENTCONST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  		/* tolower() */

/* gtk colors */
#define BACKR 	0xf0f0
#define BACKG 	0x8b8b
#define BACKB 	0x8b8b
#define BUTR	0xf2f2
#define BUTG	0xe4e4
#define BUTB	0x9b9b
#define BLACKR  0xd1d1
#define BLACKG  0x8b8b
#define BLACKB  0x4747
#define WHITER  0xffff
#define WHITEG  0xcece
#define WHITEB  0x9e9e

#define MAX_NAME_LEN	21
#define MAX_MSG_LEN 	161

typedef struct user USER;
typedef struct captures CAPTURES;
typedef struct game GAME;
typedef struct frnd FRIEND;
typedef struct friendList FRIENDLIST;

struct user {
	char username[MAX_NAME_LEN];
	int FD;
	GAME * game;
	FRIENDLIST * friendList;
	int guestFlag; /* indicates whether or not the user is logged in */
};

struct friendList {
	int length;
	FRIEND * first;
	FRIEND * last;
};

struct frnd {
	char username[MAX_NAME_LEN];
	int count; // # of messages
	FRIENDLIST * list;
	FRIEND * next;
	FRIEND * prev;
	char message[300][MAX_MSG_LEN + MAX_NAME_LEN + 2];
};

typedef struct {
	int quit;
	char username[MAX_NAME_LEN];
	int FD;
} STARTFLAGS;

/* struct of flags to pass into gtk functions */
typedef struct {
	int quit;
	int create;
	int undo;
	int returnToLogin;
	USER * user;
} FLAGPASS;

struct captures {
	int whiteCount;
	int blackCount;
	char whiteCapt[17];
	char blackCapt[17];
};

struct game {
	int board[8][8];
	int count;		/* move count */
	int checkFlag;  	/* 0 by default, 1 if in check, 2 if checkmate */
	int enPassFlag;		/* indicates that an enpass is possible */
	int enPassLoc[2];	/* indicates location that enpass would occur */
	int enPassCapt;		/* flag indicating enpass capture has been made */
	int castleFlag; 	/* 0 for no castle, 1 for kingside castle */
	int lastMove[2];	/* holds dest of last move to highlight on printed game board */
	int pawnPromoFlag;	/* indicates to logfile that pawn has been promo */
	int AITurnFlag;		/* indicates AI turn */
	int AIThinkingFlag;	/* indicates AI not making move but calculating possible moves */
	int lateGame;		/* indicates late-game conditions have been met */ 
					/* AI king now behaves differently */
	int captureFlag;	/* incidates to logfile that a capture has been made */
	char oppUsername[MAX_NAME_LEN];
	CAPTURES * captures;	
};

#endif /* CLIENTCONST_H */

/* EOF */
