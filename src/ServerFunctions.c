
/////////////////////////
/* SERVERFUNCTIONS . C */
/////////////////////////

/* Adapted from ClockServer.c: simple TCP/IP server example with timeout support
 * Author: Rainer Doemer, 2/17/15
 */

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
#include "ServerFunctions.h"

int Shutdown = 0;

/* create a socket on this server */
int MakeServerSocket( uint16_t PortNo ) {
	int ServerFD;
	struct sockaddr_in ServSocketName;

	/* create the socket */
	ServerFD = socket(PF_INET, SOCK_STREAM, 0);
	if (ServerFD < 0){   
		perror("service socket creation failed");
		exit(10);
	} /* fi */

	/* bind the socket to this server */
	ServSocketName.sin_family = AF_INET;
	ServSocketName.sin_port = htons(PortNo);
	ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);

	/* first bind to socket */
	if (bind(ServerFD, (struct sockaddr*)&ServSocketName, sizeof(ServSocketName)) < 0) {   
		perror("binding the server to a socket failed");
		exit(10);
	} /* fi */

	/* start listening to this socket */
	/* max 5 clients in backlog */
	if (listen(ServerFD, 8) < 0) {   
		perror("listening on socket failed");
		exit(10);
	} /* fi */

	return ServerFD;
} /* end of MakeServerSocket */

/* process a request by a client */
void ProcessRequest( int DataSocketFD, ULIST *userlist, ONLINELIST * onlineList, fd_set * ActiveFDs) {
	int n;
	char RecvBuf[4096];	/* message buffer for receiving a message */

	n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1);
	if (n < 0) {   
		perror("reading from data socket failed");
		exit(10);
	} /* fi */
	RecvBuf[n] = 0;
#ifdef DEBUG
	printf("Received message: %s\n", RecvBuf);
#endif
	/* send the received buffer to be handled */
	ProcessStream(RecvBuf, userlist, onlineList, DataSocketFD, ActiveFDs);
	return;
} /* end of ProcessRequest */

/* function of server */
/* timeout in microseconds */
void ServerMainLoop(ULIST *userList, ONLINELIST * onlineList, int Timeout)	{
	int DataSocketFD;	/* socket for a new client */
	socklen_t ClientLen;
	struct sockaddr_in ClientAddress;	/* client address we connect with */
	fd_set ActiveFDs;	/* socket file descriptors to select from */
	fd_set ReadFDs;	/* socket file descriptors ready to read from */
	struct timeval TimeVal;
	int res, i;

	FD_ZERO(&ActiveFDs);		/* set of active sockets */
	FD_SET(onlineList->first->FD, &ActiveFDs);	/* server socket is active */
	
	while(!Shutdown) {   
		ReadFDs = ActiveFDs;
		
		/* timeout handling */
		TimeVal.tv_sec  = 250000 / 1000000;	/* seconds */
		TimeVal.tv_usec = 250000 % 1000000;	/* microseconds */

		/* block until input arrives on active sockets or until timeout */
		res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);
		if (res < 0){   
			perror("wait for input or timeout (select) failed");
			exit(10);
		} /* fi */
		/* timeout occurred */
		if (res == 0) {
			#ifdef DEBUG
			printf("Handling timeout...\n");
			#endif
			HandleTimeout();
		} /* fi */
		/* some FDs have data ready to read */
		else {   
			for(i=0; i<FD_SETSIZE; i++) {   
				/* if FD is the same as server socket's FD, it is a connection request */
				if (FD_ISSET(i, &ReadFDs)){  
					/* connection request on server socket */
					if (i == onlineList->first->FD){	
						#ifdef DEBUG
						printf("Accepting new client %d...\n", i);
						#endif
						ClientLen = sizeof(ClientAddress);
						DataSocketFD = accept(onlineList->first->FD, (struct sockaddr*)&ClientAddress, &ClientLen);
						if (DataSocketFD < 0){   
							perror("data socket creation (accept) failed");
							exit(10);
						} /* fi */
						#ifdef DEBUG
						printf("Client %d connected from %s:%hu.\n", i, inet_ntoa(ClientAddress.sin_addr), ntohs(ClientAddress.sin_port));
						#endif
						FD_SET(DataSocketFD, &ActiveFDs);
							    			} /* fi */
					/* otherwise connect w someone else to preform a function */
					/* active communication with a client */
		    			else {  
						#ifdef DEBUG
						printf("Dealing with client %d...\n", i);
						#endif
						ProcessRequest(i, userList, onlineList, &ActiveFDs);
					//	close(i);
					//	FD_CLR(i, &ActiveFDs);
		    			} /* esle */
				} /* fi */
			} /* rof */
		} /* esle */
    	} /* elihw */
} /* end of ServerMainLoop */

/* EOF */
