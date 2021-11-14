
////////////////
/* SERVER . C */
////////////////

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


int main (int argc, char *argv[]) {
	int ServerFD;	/* socket file descriptor for service */
	int PortNo;		/* port number */
	ULIST * userList = CreateUList();
	ONLINELIST * onlineList = CreateOnlinelist();

#ifdef DEBUG
	printf("Starting...\n");
#endif
	if (argc < 2){   
		fprintf(stderr, "Usage: port\n");
		exit(10);
	} /* fi */
	PortNo = atoi(argv[1]);	/* get the port number */
	if (PortNo <= 2000) {   
		fprintf(stderr, "Invalid port number %d, should be >2000\n", PortNo);
		exit(10);
	} /* fi */
#ifdef DEBUG
	printf("Creating the server socket...\n");
#endif
	ServerFD = MakeServerSocket(PortNo);
	AppendOnline(onlineList, "Server", ServerFD);

	printf("Providing current time at port %d...\n", PortNo);
	ServerMainLoop( userList, onlineList, 250000);
	printf("\nShutting down.\n");

	DeleteUList(userList);
	DeleteOnlinelist(onlineList);
	close(ServerFD);
	return 0;
}

/* EOF */
