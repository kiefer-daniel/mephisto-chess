
/////////////////////
/* CLIENT UTIL . C */
/////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include "ClientConst.h"
#include "ClientUtil.h"
#include "MovePiece.h"

USER * CreateUser() {
	USER * new = (USER *) malloc( sizeof( USER ));	
	CreateFriendList(new);
	if (!new || !new->friendList) {
		perror("Unable to create user list.\n");
		exit(10);
	} /* fi */
	new->game = NULL;	

	return new;
}

FLAGPASS * CreateFlagpass() {
	FLAGPASS * new = (FLAGPASS *) malloc(sizeof(FLAGPASS));
	if (!new) {
		perror("Unable to create Flagpass.\n");
		exit(10);
	} /* fi */
	new->user = CreateUser("self");
	new->quit = 0;
	new->create = 0;
	new->undo = 0;
	new->returnToLogin = 0;
	
	return new;
}	

void DeleteUser(USER * user) {
	assert (user);
	DeleteFriendList(user);
	user->friendList = NULL;

	free(user);
	user->game = NULL;
	user->friendList = NULL;
	return;
}

void CreateFriendList(USER * user) {
	user->friendList = malloc(sizeof(FRIENDLIST));
	if(!user->friendList) {
		perror("Could not allocate friendlist. Out of memory.\n");
		exit(10);
	} /* fi */
	user->friendList->length = 0;
	user->friendList->first = NULL;
	user->friendList->last = NULL;
	
	return;
}

void DeleteFriendList(USER * user) {
	assert(user->friendList);
	FRIEND * friend;
	FRIEND * select;

	friend = user->friendList->last;
	while(friend) {
		select = friend->prev;
		RemoveFriend(friend->username, user);
		friend = select;
	} /* elihw */
	
	free(user->friendList);
	friend = NULL;
	select = NULL;
	return;
}

void AppendFriend (char username[], USER * user) {

	FRIEND * newFriend = (FRIEND *) malloc( sizeof(FRIEND));
	newFriend->next = NULL;
	strcpy(newFriend->username, username);
	newFriend->count = 0;	

	if (user->friendList->length == 0) {
		newFriend->list = user->friendList;
		user->friendList->first = newFriend;
		user->friendList->last = newFriend;
		newFriend->prev = NULL;
	} /* fi */
	else {
		newFriend->list = user->friendList;
		newFriend->prev = user->friendList->last;
		newFriend->prev->next = newFriend;
		newFriend->next = NULL;
		user->friendList->last = newFriend;
	} /* esle */

	user->friendList->length++;
	return;
}

int RemoveFriend( char * username, USER * user) {
	FRIEND * select = user->friendList->first;
	char * friendName = user->friendList->first->username;

	for (int i = 0; i < user->friendList->length; i++ ){
		if ( 0 == strcmp(username, select->username)) {
			if( i == 0 ) {
				user->friendList->first = user->friendList->first->next;
				user->friendList->first->prev = NULL;
			} /* fi */
			else if (i == user->friendList->length - 1) {
				user->friendList->last = user->friendList->last->prev;
				user->friendList->last->next = NULL;
			} /* fi esle */
			else {
				select->prev->next = select->next;
				select->next->prev = select->prev;
			}
			select->next = NULL;
			select->prev = NULL;
			free(friendName);
			user->friendList->length--;
			select = NULL;
			friendName = NULL;
			return 1;
		} /* fi */
		else {
			select = select->next;
			friendName = select->username;	
		} /* esle */
	} /* rof */
	return 0;
}

void AddMessage( char * username, USER * user, char * message) {
	
	FRIEND * select = user->friendList->first;
	for (int i = 0; i < user->friendList->length; i++) {
		if ( 0 == strcmp(select->username, username)) {
			strcpy(select->message[select->count], message);
			return;
		} /* fi */
		else {
			select = select->next;
		} /* esle */
	} /* rof */
	return;
}

void ListenForServer( USER * user) {
	int n;
	char RBuff[4096];
	char * command;
	char * fromUN;
	char * message;
	char * move;
	int moveArr[4];

	fcntl(user->FD, F_SETFL, O_NONBLOCK);
	n = read( user->FD, RBuff, sizeof(RBuff)-1);
	if (n < 0 ) {
		return;
	} /* fi */

	printf("n = %d", n);	
	RBuff[n] = 0;

#ifdef DEBUG
	printf("Recieved message in Listen: %s\n", RBuff);
#endif
	/* get the command */
	command = strtok(RBuff, " ");
	
	/* handle commands accordingly */
	if ( 0 == strcmp(command, "MSG_RECV") ) {
		fromUN = strtok(NULL, " ");
		message = RBuff + 10 + strlen(fromUN);
//		AddMessage(user, fromUN, message);
	} /* fi */
	else if ( 0 == strcmp(command, "MOVE_RECV")) {
		move = strtok(NULL, " ");
		if (user->game) {
			SmithToArray(move, moveArr);
			MovePiece(moveArr, user->game, NULL);
		} /* fi */
	} /* fi esle */
	else if ( 0 == strcmp(command, "FR_REJ")) {
		fromUN = strtok(NULL, " ");
//		FriendReject(user, fromUN);
	} /* fi esle */
	else if ( 0 == strcmp(command, "FR_ACPT")) {
		fromUN = strtok(NULL, " ");
//		AddFriend(user, fromUN);
	} /* fi esle */
	else if ( 0 == strcmp(command, "FR_REQ")) {
		fromUN = strtok(NULL, " ");
//		FriendRequest(user, fromUN);
	} /* fi esle */
	else if ( 0 == strcmp(command, "GAME_REJ")) {
		fromUN = strtok(NULL, " ");
//		GameReject(user, fromUN);
	} /* fi esle */
	else if ( 0 == strcmp(command, "GAME_ACPT")) {
		fromUN = strtok(NULL, " ");
//		GameAccept(user, fromUN);
	} /* fi esle */
	else if ( 0 == strcmp(command, "GAME_REQ")) {
		fromUN = strtok(NULL, " ");
//		GameRequest(user, fromUN);
	} /* fi esle */
	return;
}

void HandleResponse(USER * user, char * RBuff) {
	char * command;
	char * newUN;

#ifdef DEBUG
	printf("Recieved message in Server Reply: %s\n", RBuff);
#endif
	/* get the command */
	command = strtok(RBuff, " ");
	
	/* handle commands accordingly */
	if ( 0 == strcmp(command, "LOGINSUCCESS") ) {
		newUN = strtok(NULL, " ");
		strcpy(user->username, newUN);
		/* add friends */
	} /* fi */
	
}	

int CheckForErrors(char * buff) {
	
	char * command = strtok(buff, " ");
	printf("command: %s \n", command);
	
	/* handle commands accordingly */
	if ( 0 == strcmp(command, "CONNECT") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "REGISTER") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "LOGIN") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "LOGOUT") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "REFRESH") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "REFRESH_FR") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "MOVE") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "IM") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "FRREQ") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "FRACPT") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "FRREJ") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "GAMEREQ") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "QUIT") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "CLOSE") ) {
		return 1;
	} /* fi */
	else if ( 0 == strcmp(command, "EXIT") ) {
		return 1;
	} /* fi */

	else {
		printf("Unrecognized command. Please try again: ");
	} /* esle */
	return 0;
}






/* EOF */
