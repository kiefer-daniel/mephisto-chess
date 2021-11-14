

//////////////////////////
/* SERVER UTILITIES . H */
//////////////////////////

#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include "ServerConst.h"
#include "ListUtil.h"

#define BUFFERSIZE 1024
#define MESSAGESIZE 500
#define HISTORYSIZE 500


int RegisterUser(ULIST *userlist, char *username, char *password, char *replyBuff);
int UsernameToFD(ONLINELIST *onlinelist, char *username);
int LoginUser(ULIST *userlist, char *username, char *password, char *replyBuff, ONLINELIST * onlineList, int FD) ;
void Logout(ONLINELIST * online, char *UN, char * replyBuff);
int ProcessFriend(ULIST *userlist, ONLINELIST *onlinelist, char *fromUN, char *toUN, char *replyBuff, char *forwardBuff, int flag, int *fwdFD);
int ProccessGame(ONLINELIST * onlinelist, ULIST *userlist, char *fromUN, char *toUN, char *replyBuff, char *forwardBuff, int flag);
int SendMessage(ULIST *userlist, char *fromUN, char *toUN, char *message, char *replyBuff, char *forwardBuff);

int ProcessStream(char *receive, ULIST *userlist, ONLINELIST * onlineList, int replyFD, fd_set * ActiveFDs);

void HandleTimeout(void);

#endif /* SERVERUTIL_H */

/* EOF */
