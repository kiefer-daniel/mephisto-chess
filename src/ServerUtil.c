

//////////////////////////
/* SERVER UTILITIES . C */
//////////////////////////


#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "ServerUtil.h"
#include "ServerConst.h"
#include "ListUtil.h"

int RegisterUser(ULIST *userlist, char *username, char *password, char *outbuffer) {
	UENTRY *user1, *user2;

	user1 = userlist->first;
	while(user1) {
		user2 = user1 -> next;
		if((0 == strcmp(user1->username, username)) && (0 == strcmp(user1->password, password))) {
			strcpy(outbuffer, "REGISTERFAILED\n");
			return 1;
		}
		user1 = user2;
	}

	AppendUEntry(userlist, username, password);
	strcpy(outbuffer, "REGISTERSUCCESS\n");
	return 0;
}

int UsernameToFD(ONLINELIST *onlinelist, char *username)
{
	ONLINE *online1; 
	online1 = onlinelist->first;

	while(online1){

		if(0 == strcmp(online1->name, username)){
			return online1->FD; 
		} /* fi */

		online1 = online1->next; 
	} /* elihw */

	// return -1 if username not found  
	return -1; 
}

int LoginUser(ULIST *userlist, char *username, char *password, char *outbuffer, ONLINELIST * onlineList, int FD) {
	UENTRY *user1, *user2;

	user1 = userlist->first;
	while(user1) {
		user2 = user1 -> next;
		if ((0 == strcmp(user1->username, username)) && (0 == strcmp(user1->password, password))) {
			strcpy(outbuffer, "LOGINSUCCESS ");
			strcat(outbuffer, username);
			/* add username to ONLINE list */
			ONLINE * o = onlineList->first;
			while (FD != o->FD) {
				o = o->next;
			}
			strcpy(o->name, username);
//			SendUserList(userlist, username, outbuffer);	
//			SendFriendList(userlist, username, outbuffer);
			#ifdef DEBUG
			printf("Added user %s to ONLINELIST \n", o->name);
			#endif
			strcat(outbuffer, user1->buffer);
			memset(user1->buffer,' ', sizeof(user1->buffer));
			user1->buffer[0] = 0;
			return 0;
		}
		user1 = user2;
	}
	/* if login combo is not registered */
	strcpy(outbuffer, "LOGINFAILED\n");
	
	return 1;

}

void Logout(ONLINELIST * online, char *UN, char * replyBuff) {
	/* to replace ONLINE username with guest username */
	char name[21] = "guest";
	char FD[3];
	ONLINE * select = online->first;
	for (int i = 0; i < online->length; i++) {
		if ( 0 == strcmp(select->name, UN)) {
			snprintf(FD, sizeof(FD), "%d", select->FD );
			strcat(name, FD);
			strcpy(replyBuff, "OK_LOGOUT ");
			strcat(replyBuff, name);
			return;
		} /* fi */
		else {
			select = select->next;
		} /* esle */
	} /* rof */
	return;	
}

int ProcessFriend(ULIST *userlist, ONLINELIST *onlinelist, char *fromUN, char *toUN, char *replyBuff, char *forwardBuff, int flag, int *fwdFD) {
	
	// 0 for friend reject
	// 1 for friend accpt
	// 2 for friend request 
	// 3 for friend remove 	 
	if (flag == 0) {
		// tell the requester that the friend request was rejected
		strcpy(replyBuff, "OK"); 
		strcpy(forwardBuff, "NOFR ");
		strcat(forwardBuff, fromUN); 
	} /* fi */

	else if (flag == 1) {
		// search for destination username and add friend 'username'
		UENTRY *user1;
		ONLINE *online1; 
		user1 = userlist->first;
		online1 = onlinelist->first; 
		while (user1) {
			if (0 == strcmp(user1->username, toUN)) {
				// check that the user is online
				while(online1){
					if(0 == strcmp(online1->name, toUN)) {
					 	AppendFEntry(fromUN, user1); 
						strcpy(forwardBuff, "FRACPT ");	
						strcat(forwardBuff, fromUN);
					} /* fi */ 
				online1 = online1->next; 
				} /* elihw */ 
			} /* fi */
			user1 = user1->next;
		} /* elihw */
		// search for username and add friend 'dst username' 
		UENTRY *user2;
		user2 = userlist->first; 
		while (user2) {
			if (0 == strcmp(user2->username, fromUN)) {
				AppendFEntry(toUN, user2);
				strcpy(replyBuff, "OKFR ");
				strcat(replyBuff, toUN); 
			} /* fi */
			user2 = user2->next;
		} /* elihw */				
	} /* fi esle */

	/* handle friend request */
	/* if friend is found, forward request, otherwise reply to sender NOFRUSR */
	else if (flag == 2){
		ONLINE * select = onlinelist->first;
		/* find the friend in ONLINELIST */
		for (int i = 0; i < onlinelist->length; i++) {
			#ifdef DEBUG
			printf("%s\n", select->name);
			#endif
			if (strcmp(select->name, toUN) == 0) {
				strcpy(replyBuff, "OKFRREQ");
				strcpy(forwardBuff, "FRREQ "); 
				strcat(forwardBuff, fromUN); 	
				int FWDlen = strlen(forwardBuff);
				write(select->FD, forwardBuff, FWDlen); 
				return 0;
		} /* fi */
			else {
				select = select->next;
			} /* esle */
		} /* rof */
		
		/* username is not online */
		strcpy(replyBuff, "NOFRUSR");
		
	} /* fi esle */
	
	else if (flag == 3)
	{
		// tell the requester 'ok'	
		UENTRY *user1;
		ONLINE *online1;  
		user1 = userlist->first;
		online1 = onlinelist ->first;

		while (user1) {
			if (0 == strcmp(user1->username, toUN)) {
				// check that the user is online
				while(online1){
					if(0 == strcmp(online1->name, toUN)) {
						strcpy(forwardBuff, "OKRM ");	
						strcat(forwardBuff, fromUN);
					 DeleteFEntry(fromUN, user1); 
					} /* fi */ 
				online1 = online1->next; 
				} /* elihw */ 
			} /* fi */
			user1 = user1->next;
		} /* elihw */
		// search for username and add friend 'dst username' 
		UENTRY *user2;
		user2 = userlist->first; 
		while (user2) {
			if (0 == strcmp(user2->username, fromUN)) {
				DeleteFEntry(toUN, user2);
				strcpy(replyBuff, "OKRM ");
				strcat(replyBuff, toUN); 
			} /* fi */
			user2 = user2->next;
		} /* elihw */				
	 /* fi esle */
	}
/*
	forwardFD = UsernameToFD(onlinelist, toUN);
	int FWDlen = strlen(forwardBuff);
	int n = write(forwardFD, forwardBuff, FWDlen); 
*/
	return 0; 
}

int ProcessGame(ONLINELIST * onlinelist, ULIST *userlist, char *fromUN, char *toUN, char *replyBuff, char *forwardBuff, int flag) {
	// 0 for game reject
	// 1 for game accept
	// 2 for game request 
	
	// 0 : game reject
	// tell the replyBuff 'ok' and tell the forward buff the game they requested was rejected 
	if (flag == 0){
		strcpy(replyBuff, "OK");
		strcpy(forwardBuff, "NOGM ");
		strcat(forwardBuff, fromUN); 
	} /* fi */ 

	// 1 : game accept
	// change both client structs the specify they are in game with each other 
	
	else if (flag == 1){
	// tell both they are playing game with each other: OKGM <game with> and client can set up game 
		strcpy(replyBuff, "OKGM ");
		strcat(replyBuff, toUN);
		strcpy(forwardBuff, "GAMEACPT "); 
		strcat(forwardBuff, fromUN); 
	} /* fi esle */ 

	// 2 : game request
	// tell the replyBuff 'ok' and tell the forward buff that user1 has requested a game with them 	
	
	else if (flag == 2){
		ONLINE * select = onlinelist->first;
		/* find the friend in ONLINELIST */
		for (int i = 0; i < onlinelist->length; i++) {
			#ifdef DEBUG
			printf("%s\n", select->name);
			#endif
			if (strcmp(select->name, toUN) == 0) {
				strcpy(replyBuff, "OKGAMEREQ");
				strcpy(forwardBuff, "GAMEREQ "); 
				strcat(forwardBuff, fromUN); 	
				int FWDlen = strlen(forwardBuff);
				write(select->FD, forwardBuff, FWDlen); 
				return 0;
		} /* fi */
			else {
				select = select->next;
			} /* esle */
		} /* rof */
		
		/* username is not online */
		strcpy(replyBuff, "NOGAMEREQ");
	} /* fi esle */ 
	return 0; 
}

int SendMessage(ULIST *userlist, char *username, char *dstusername, char *message, char *replyBuff, char *forwardBuff) {
	// tell client 1 that their message was received and is being sent
	strcpy(replyBuff, "OK");

	// send the receiving client the message: <fromUser> <message>  
	strcpy(forwardBuff, username);
	strcat(forwardBuff, message); 
	return 0;
}

int ProcessStream(char *receive, ULIST *userlist, ONLINELIST * onlineList, int replyFD, fd_set * ActiveFDs) {
	char fromUN[USERLENGTH]; /* username of requesting client */
	char toUN[USERLENGTH];	/* username of 2nd client to forward msg to */
	char password[PASSLENGTH]; 
	char replyBuff[BUFFERSIZE]; /* buffer used to reply to requesting client */
	char forwardBuff[BUFFERSIZE]; /* buffer used to forward message to 2nd client */
	char message[MESSAGESIZE]; 
	char move[4];
	char * command;
	int forwardFD; /* FD to forward message to */

	memset(replyBuff, ' ', sizeof(replyBuff));
	replyBuff[0] = '\0';
	
	/* get the command */
	command = strtok(receive, " "); 
	
	if (0 == strcmp(command, "CONNECT")){
		char name[21] = "guest";
		char FD[3];
		snprintf(FD, sizeof(FD), "%d", replyFD );
		strcat(name, FD);
		AppendOnline(onlineList, name, replyFD);
		strcpy(replyBuff, "OKCN ");
		strcat(replyBuff, name);
		strcat(replyBuff, " ]"); 
	} /* fi */
	
	else if (0 == strcmp(command, "REFRESH")){
		ONLINE * username = onlineList->first;
		// parse for username
                strcpy(fromUN, strtok(NULL, " "));
		strcpy(replyBuff, "OK_REFRESH ");
		while(username) {
			if((0 != strcmp(username->name, fromUN)) && (0 != strcmp(username->name, "Server"))) {
				strcat(replyBuff, username->name);
				strcat(replyBuff, " ");
			} /* fi */
			username = username->next;
		} /* elihw */
		strcat(replyBuff," ]"); 
	} /* fi esle */
	
	else if (0 == strcmp(command, "REGISTER")) {
		// parse for username and password
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(password, strtok(NULL, " "));
		AppendUEntry(userlist, fromUN, password);
	#ifdef DEBUG
		printf("Registering user\n");
		printf("added username: %s, password: %s\n", userlist->last->username, userlist->last->password);
	#endif
		/* check if this username already exists */
		strcpy(replyBuff, "OKUSR ");
		strcat(replyBuff, " ]"); 
	
	} /* fi  esle */

	else if (0 == strcmp(command, "REFRESH_FR")){
		UENTRY * user = userlist->first; // for finding user in userlist
		ONLINE * checkfrnd; // for checking if frnd is online
		FENTRY * frnd;
		// parse for username
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(replyBuff, "OK_REFRESH_FR");
		while(user) {
			if(0 == strcmp(user->username, fromUN)) {
				frnd = user->friendlist->first;
				checkfrnd = onlineList->first;
				while(frnd) {
					if(0 == strcmp(frnd->username, checkfrnd->name)) {
						strcat(replyBuff, frnd->username);
						strcat(replyBuff, " ");
					} /* fi */
					checkfrnd = checkfrnd->next;
				} /* elihw */
			} /* fi */
			user = user->next;
		} /* elihw */
		strcat(replyBuff, " ]"); 
	} /* fi esle */

	else if (0 == strcmp(command, "LOGIN")){
		// parse for username and password 
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(password, strtok(NULL, " ")); 
		LoginUser(userlist, fromUN, password, replyBuff, onlineList, replyFD );
	} /* fi esle */
	
	else if (0 == strcmp(command, "LOGOUT")){
		// parse for username 
		strcpy(fromUN, strtok(NULL, " ")); 
		Logout(onlineList, fromUN, replyBuff); 
		strcat(replyBuff, " ]"); 
	}

	else if (0 == strcmp(command, "IM")){
		// parse for username, destination username,  and message
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		strcpy(message, strtok(NULL, "*")); /* contents of the message */ 
		strcpy(forwardBuff, "MSG ");
		strcat(forwardBuff, fromUN);
		strcat(forwardBuff, " ");
		strcat(forwardBuff, message);
		strcat(forwardBuff, " ]"); 
		forwardFD = UsernameToFD(onlineList, toUN);
		int FWDlen = strlen(forwardBuff);
		write(forwardFD, forwardBuff, FWDlen);
		strcpy(replyBuff, "OKSENT ");	
		strcat(replyBuff, " ]"); 
	}
	
	else if (0 == strcmp(command, "FRREQ")){
		// parse for username and destination username
		strcpy(toUN, strtok(NULL, " "));
		strcpy(fromUN, strtok(NULL, " "));

		ProcessFriend(userlist, onlineList, fromUN, toUN, replyBuff, forwardBuff, 2, &forwardFD);
		strcat(replyBuff, " ]"); 
	}

	else if (0 == strcmp(command, "FRACPT")){
		//parse for username and destination username 
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " ")); 
		ProcessFriend(userlist, onlineList, fromUN, toUN, replyBuff, forwardBuff, 1, &forwardFD);
	
		int FWDlen = strlen(forwardBuff);
		write(forwardFD, forwardBuff, FWDlen); 
	}

	else if (0 == strcmp(command, "FRREJ")){
		// parse for username and destination username 
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		ProcessFriend(userlist, onlineList, fromUN, toUN, replyBuff, forwardBuff, 0, &forwardFD);
		
		int FWDlen = strlen(forwardBuff);
		write(forwardFD, forwardBuff, FWDlen); 
	}

	else if (0 == strcmp(command, "FRRM")) {
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));	
		ProcessFriend(userlist, onlineList, fromUN, toUN, replyBuff, forwardBuff, 3, &forwardFD); 
	
		int FWDlen = strlen(forwardBuff);
		write(forwardFD, forwardBuff, FWDlen); 
	}
	else if (0 == strcmp(command, "GAMEREQ")){
		// parse for username and destination username 
		strcpy(toUN, strtok(NULL, " "));
		strcpy(fromUN, strtok(NULL, " "));
		ProcessGame(onlineList, userlist, fromUN, toUN, replyBuff, forwardBuff, 2); 
	}

	else if (0 == strcmp(command, "GAMEACPT")){
		// parse for username and destination username
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		ProcessGame(onlineList, userlist, fromUN, toUN, replyBuff, forwardBuff, 1); 
	}

	else if (0 == strcmp(command, "GAMEREJ")){
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		ProcessGame(onlineList, userlist, fromUN, toUN, replyBuff, forwardBuff, 0); 
	}
	
	else if (0 == strcmp(command, "MOVE")) {
		// parse for username, destination username,  and message
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		strcpy(move, strtok(NULL, " ")); /* chess move */ 
		strcpy(forwardBuff, "MOVE ");
		strcat(forwardBuff, move);
		strcat(replyBuff, " ]"); 
		forwardFD = UsernameToFD(onlineList, toUN);
		int FWDlen = strlen(forwardBuff);
		write(forwardFD, forwardBuff, FWDlen);
		strcpy(replyBuff, "OKMOVE");	
		strcat(replyBuff, " ]"); 
	}
	
	// for quiting chess game
	else if (0 == strcmp(command, "QUIT_GAME")) {
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(toUN, strtok(NULL, " "));
		strcpy(replyBuff, "OK_QUIT");
		strcat(replyBuff, " ]");
		strcpy(forwardBuff, "OK_QUIT");	
		strcat(forwardBuff, " ]"); 
	}
	
	else if (0 == strcmp(command, "DISCONNECT")){
		ONLINE * user = onlineList->first;
		// parse for username
		strcpy(fromUN, strtok(NULL, " "));
		strcpy(replyBuff, "OK_DISCONNECT");
		int len = strlen(replyBuff);
		write(user->FD, replyBuff, len);
		while(user) {
			if(0 == strcmp(user->name, fromUN)) {
				write(user->FD, replyBuff, len);
				close(user->FD);
				FD_CLR(user->FD, ActiveFDs);
				DeleteOnline(user->name, onlineList);
				return 0;
			} /* fi */
			user = user->next;
		} /* elihw */
	} /* fi esle */
	
	else if (0 == strcmp(command, "QUIT_SERVER")) {
		ONLINE * onlineuser = onlineList->first;
		char response[BUFFERSIZE] = "SHUTDOWN";
		int FWDlen = strlen(response);
		// send 'SHUTDOWN' command to all online users
		while(onlineuser) {
			if(0 != strcmp(onlineuser->name, "Server")) {
				write(onlineuser->FD, response, FWDlen);
				close(onlineuser->FD);
			} /* fi */
			onlineuser = onlineuser->next;
		} /* elihw */
		onlineuser = onlineList->first;
		close(onlineuser->FD);
		DeleteOnlinelist(onlineList);
		DeleteUList(userlist);
		onlineuser = NULL;
		
		exit(10);
	}

	if (replyBuff[0] == '\0'){
//		UserUpdate(userlist, fromUN, replyBuff); 
	}

	int len = strlen(replyBuff);
	int n = write(replyFD, replyBuff, len);
	printf("The size of the data send is: %d\n", n);
	memset(replyBuff,' ', sizeof(replyBuff));
	if (n < 0) {
		perror("Writing to data socket failed.");
		exit(10);
	} /* fi */

	return 0;
}

/* prints out a spinning wheel */
void HandleTimeout(void) {
    char   Wheel,
	   *WheelChars = "|/-\\";
    static int WheelIndex = 0;

    WheelIndex = (WheelIndex+1) % 4;
    Wheel = WheelChars[WheelIndex];
    printf("\rTimeout: %c",	/* print from beginning of current line */
	 Wheel);	/* print time plus a rotating wheel */
    fflush(stdout);
} /* end of HandleTimeout */ 


/* EOF */
