
/////////////////////
/* CLIENT UTIL . H */
/////////////////////

#ifndef CLIENTUTIL_H
#define CLIENTUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h> 
#include <sys/select.h> 
#include <arpa/inet.h>
#include <assert.h>

#include "ClientConst.h"

USER * CreateUser();
void DeleteUser(USER * user);
FLAGPASS * CreateFlagpass();

void CreateFriendList(USER * user);
void DeleteFriendList(USER * user);
void AppendFriend (char * username, USER * user);
int RemoveFriend( char * username, USER * user);


void AddMessage( char * username, USER * user, char * message);
void ListenForServer( USER * user);
void HandleResponse(USER * user, char * RBuff);
int CheckForErrors(char * buff);

#endif /* CLIENTUTIL_H */

/* EOF */
