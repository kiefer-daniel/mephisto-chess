/* ClockClient.c: simple interactive TCP/IP client for ClockServer
 * Author: Rainer Doemer, 2/16/15 (updated 2/20/17)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h> 
#include <fcntl.h>
#include <sys/select.h> 
#include <arpa/inet.h>
#include "ClientUtil.h"

/* #define DEBUG */	/* be verbose */

/*** global variables ****************************************************/

const char *Program	/* program name for descriptive diagnostics */
	= NULL;

/*** global functions ****************************************************/

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg) {
	fputs(Program, stderr);
	fputs(": ", stderr);
	perror(ErrorMsg);
	fputs(Program, stderr);
	fputs(": Exiting!\n", stderr);
	exit(20);
} /* end of FatalError */

int main(int argc, char *argv[]) {
	int l, n;
	int PortNo;		/* port number */
	struct sockaddr_in ServerAddress;	/* server address we connect with */
	struct hostent *Server;	/* server host */
	char SendBuf[256];	/* message buffer for sending a message */
	char RecvBuf[256];	/* message buffer for receiving a response */
	USER * user = CreateUser("self");

	Program = argv[0];	/* publish program name (for diagnostics) */
	#ifdef DEBUG
    	printf("%s: Starting...\n", argv[0]);
	#endif
	if (argc < 3){   
		fprintf(stderr, "Not enough arguments\n");
		exit(10);
	} /* fi */

	Server = gethostbyname(argv[1]);
	if (Server == NULL) {   
		fprintf(stderr, "%s: no such host named '%s'\n", Program, argv[1]);
        	exit(10);
	} /* fi */

	PortNo = atoi(argv[2]);
	if (PortNo <= 2000){   
		fprintf(stderr, "%s: invalid port number %d, should be >2000\n", Program, PortNo);
        	exit(10);
	} /* fi */

	/* setting members of sin_address */
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];

	/* set the socket FD */
	user->FD = socket(AF_INET, SOCK_STREAM, 0);
	if (user->FD < 0){   
		FatalError("socket creation failed");
	} /* fi */
	printf("Connecting to the server at port %d...\n", PortNo);
	/* this is where connection happens */
	if (connect(user->FD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0) {   
		FatalError("connecting to server failed");
	} /* fi */
			
	do {   

		/* timeout occurred */
		printf("%s: Enter a command to send to the server:\n"
					"'CONNECT' \n"
					"'REGISTER' <fromUsername> <password>\n"
					"'LOGIN' <fromUsername> <password>\n"
					"'LOGOUT' <fromUsername> \n"
					"'REFRESH' <fromUsername>\n"
					"'REFRESH_FR' <fromUsername>\n"
					"'MOVE' <fromUsername> <toUsername> <move>\n"
					"'IM' <fromUsername> <toUsername> <message>\n"
					"'FRREQ' <fromUsername> <toUsername>\n"
					"'FRACPT' <fromUsername> <toUsername>\n"
					"'FRREJ' <fromUsername> <toUsername>\n"
					"'FRRM' <fromUsername> <toUsername>\n"
					"'GAMEREQ' <fromUsername> <toUsername>\n"
					"'QUIT' <fromUsername> <toUsername>\n"
					"'CLOSE' <fromUsername>\n"
					"'QUIT_SERVER'\n"
					"command: ", argv[0]); 
		
		fgets(SendBuf, sizeof(SendBuf), stdin);
		l = strlen(SendBuf);
		if (SendBuf[l-1] == '\n') {   
			SendBuf[--l] = 0;
		} /* fi */

		ListenForServer(user);
		if (l){   
			printf("Sending message '%s'...\n", SendBuf);
			/* this is where the sending to the server happens */   
			n = write(user->FD, SendBuf, l);
	    		if (n < 0){   
				FatalError("writing to socket failed");
	    		} /* fi */
			
			fcntl(user->FD, F_SETFL, NULL);
			n = read(user->FD, RecvBuf, sizeof(RecvBuf)-1);
			RecvBuf[n] = 0;
			printf("Received message: %s\n", RecvBuf);
			
			HandleResponse(user, RecvBuf);
		} /* fi */
	
	} while(0 != strcmp("EXIT", SendBuf));

	printf("%s: Exiting...\n", Program);
	return 0;
}

/* EOF ClockClient.c */


