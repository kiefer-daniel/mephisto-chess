

/////////////////////////////////
/*      SERVERCONST.H          */
/////////////////////////////////

#ifndef SERVERCONST_H
#define SERVERCONST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  		/* tolower() */

#define USERLENGTH 21
#define PASSLENGTH 21
#define MSGLENGTH 500
#define BUFFERSIZE 1024


typedef struct UserList ULIST;
typedef struct FriendList FLIST;
typedef struct MessageList MLIST;

typedef struct Account UENTRY;
typedef struct Friend FENTRY;
typedef struct Message MENTRY;

struct UserList {
	UENTRY *first;
	UENTRY *last;
	int length;
};

struct FriendList {
	FENTRY *first;
	FENTRY *last;
	int length;
};

struct Account {
	ULIST *list;
	UENTRY *prev;
	UENTRY *next;
	char username[USERLENGTH];
	char password[PASSLENGTH];
	char buffer[BUFFERSIZE];
	FLIST *friendlist;
};

struct Friend {
	FLIST *list;
	FENTRY *prev;
	FENTRY *next;
	char username[USERLENGTH];
};

typedef struct online ONLINE;
typedef struct onlineList ONLINELIST;

struct online {
	char name[21];
	int FD;
	ONLINELIST * list;
	ONLINE * prev;
	ONLINE * next;
} ;

struct onlineList {
	int length;
	ONLINE * first;
	ONLINE * last;
} ;

#endif /* SERVERCONST_H */

/* EOF */
