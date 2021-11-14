
/////////////////////////
/* SERVERFUNCTIONS . H */
/////////////////////////

#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

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
#include <assert.h>
#include "ServerConst.h"
#include "ServerUtil.h"
#include "ListUtil.h"



int MakeServerSocket( uint16_t PortNo );
void ProcessRequest( int DataSocketFD, ULIST *userlist, ONLINELIST * onlineList, fd_set * ActiveFDs);
void ServerMainLoop( ULIST *userList, ONLINELIST * onlineList, int Timeout);


#endif /* SERVERFUNCT_H */
	
/* EOF */
