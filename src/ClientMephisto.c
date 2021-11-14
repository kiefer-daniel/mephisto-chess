
/*////////////////////////////////////////////////////////////////////
   //||   //|  ||^^^^  ||^^\\   ||   ||  ||   //^\\  ^^^||^^^  //^^\\
  // ||  //||  ||____  ||   ||  ||   ||  ||  ||         ||    ||    ||
 //  || // ||  ||      ||^^^    ||^^^||  ||   ^^^^\\    ||    ||    ||
//   ||//  ||  ||____  ||       ||   ||  ||   ____//    ||     \\__//
   ___
 //   \\  ||      ||  ||^^^^  ||  ||\  ||  ^^^||^^^
||        ||      ||  ||____  ||  ||\\ ||     ||
||        ||      ||  ||      ||  || \\||     ||
 \\___//  ||____  ||  ||____  ||  ||  \\|     ||

Created by Aammarah Idris, Darlena Nguyen, 
Kiefer Daniel, Kyle Mach, Sofia Bernstein
EECS 22L Spring
4-4-2020
*/////////////////////////////////////////////////////////////////////
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include "ClientConst.h"
#include "FileIO.h"
#include "Checks.h"
#include "GameFunctions.h"
#include "ClientUtil.h"
#include "MovePiece.h"


/*
GSourceFunc Timeout (gpointer user_data );
void on_loginButton1_clicked( GtkButton *b, gpointer flags);
void on_loginButton2_clicked( GtkButton *b, gpointer flags);
void on_loginButton3_clicked( GtkButton *b, gpointer flags);
*/

gboolean on_acceptButton_clicked (GtkButton *button, gpointer user);
gboolean on_refreshButton_clicked(GtkButton *button, gpointer user);
void EndProgram( USER * user );
void on_removeButton_clicked (GtkButton *button, gpointer user);
void on_rejectButton_clicked(GtkButton *button);

//////////
/* Main */
//////////


/* widget pointers for Start and login windows */
GtkWidget * LoginWindow;
GtkWidget * StartWindow; 
GtkWidget * ProgStart, * ProgQuit;
GtkEntry * usernameEntry;
GtkEntry * passwordEntry;
GtkWidget * incorrectLabel;
GtkWidget * loginButton1, * loginButton2, * loginButton3;

/* widget pointers for createnewwindow */
GtkWidget	*CreateNewWindow, *fixed2, *mephistoLogo2,
		*newUsernameLabel, *newPwLabel1, *newPwLabel2,
		*newButton1, *newButton2, *takenLabel;
GtkEntry	*newUsernameEntry, *newPwEntry1, *newPwEntry2;

/* widget pointers for main menu */
GtkWidget	*MainMenuWindow, *fixed3, *mephistoLogo3,
		*usernameLabel, *onlineLabel, *friendsLabel,
		*mainButton1, *mainButton2, *mainButton3, *mainButton5, 
		*scrolledwindow1, *scrolledwindow2, *scrolledwindow3,
		 *textview2, *textview3,
		*sendFrButton, *addRemoveFrButton, *entry1, *refresh;
GtkTextView *textview1; 
/* chess interface */
GtkWidget *chessWindow;
GtkWidget *board;
GtkWidget *a1button, *a2button, *a3button, *a4button, *a5button, *a6button, *a7button, *a8button, *b1button, *b2button, *b3button, *b4button, *b5button, *b6button, *b7button, *b8button, *c1button, *c2button, *c3button, *c4button, *c5button, *c6button, *c7button, *c8button, *d1button, *d2button, *d3button, *d4button, *d5button, *d6button, *d7button, *d8button, *e1button, *e2button, *e3button, *e4button, *e5button, *e6button, *e7button, *e8button, *f1button, *f2button, *f3button, *f4button, *f5button, *f6button, *f7button, *f8button, *g1button, *g2button, *g3button, *g4button, *g5button, *g6button, *g7button, *g8button, *h1button, *h2button, *h3button, *h4button, *h5button, *h6button, *h7button, *h8button;
GtkWidget *graveyard;
GtkWidget *refreshButton;
GtkWidget *moveCount;
GtkWidget *playerColor;
GtkWidget *gameMessage;
GtkWidget *capturedPieces;
GtkWidget *friendBox;
GtkTextView *chattingBox;
GtkEntry  *message;
GtkWidget *sendButton;
GtkWidget *HelpButton;
GtkWidget *QuitButton;
GtkWidget *AddRemoveButton;
GtkWidget *returnToLobby;
GtkWidget *yourUsername;
GtkWidget *opponentUsername;
GtkWidget *bknight1, *bknight2, *bpawn1, *bpawn2, *bpawn3, *bpawn4, *bpawn5, *bpawn6, *bpawn7, *bpawn8, *bking, *bqueen, *brook1, *brook2, *bbishop1, *bbishop2, *wknight1, *wknight2, *wpawn1, *wpawn2, *wpawn3, *wpawn4, *wpawn5, *wpawn6, *wpawn7, *wpawn8, *wking, *wqueen, *wrook1, *wrook2, *wbishop1, *wbishop2;

/* addRemove */
GtkWidget *addRemoveWindow;
GtkWidget *addButton;
GtkWidget *removeButton;
GtkWidget *username;
GtkWidget *errorMessage;

/* REQUEST GAME */
GtkWidget * playGameWindow, * fixed3, * playGameButton,
		*returnButton1, *userGameLabel, *logoGame,
		* errorMessage1;
GtkEntry *gameUsername;

/* help */
GtkWidget *helpWindow;
GtkWidget *returnToGameButton;
GtkWidget *helpMessage;

/* Game request window */
GtkWidget * gameReqWindow, * gameReqFixed, * acceptGameButton, 
		* rejectGameButton, *requestGamesLabel;

/* friend Request pop-up */
GtkWidget * friendReqWindow, * reqFixed, * MephistoSmall, * acceptButton, * rejectButton, * requestLabel;

/* you win pop up */
GtkWidget *youWinWindow, *okButton;

/* you lose pop up */
GtkWidget *youLoseWindow, *ok;

/* i guess you win pop up */
GtkWidget *iGuessWindow, *okay;

GtkBuilder *builder;
GtkTextBuffer *buffer1; 
gchar *mymessage1 = "";
int sendFlag = 0, undo = 0, quitLogin = 0, quit = 0, help = 0, addRemove = 0, lobby = 0;
int create = 0, returnToLogin = 0;
char position[5], oppMove[5];
char uName[MAX_NAME_LEN], friendName[100], oppName[500];
int FD;
int guestFlag = 0, chessFlag = 0, moveFlag = 0;

int main(int argc, char *argv[]) {
	int PortNo;		/* port number */
	struct sockaddr_in ServerAddress;	/* server address we connect with */
	struct hostent *Server;	/* server host */

	USER * user = CreateUser(uName);
	#ifdef DEBUG
    	printf("Starting %s...\n", argv[0]);
	#endif
	if (argc < 3){   
		printf("Too few arguments.\nPlease include server address (ex: zuma.eecs.uci.edu) and port number (ex: 14222)\n");
		exit(10);
	} /* fi */

	gtk_init (&argc, &argv);
	Server = gethostbyname(argv[1]);
	if (Server == NULL) {   
		printf("No such host named '%s'\nTry zuma.eecs.uci.edu\n", argv[1]);
        	exit(10);
	} /* fi */

	PortNo = atoi(argv[2]);
	if (PortNo <= 2000){   
		printf("Invalid port number %d, should be >2000\n", PortNo);
        	exit(10);
	} /* fi */

	/* setting members of sin_address */
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];

	/* set the socket FD */
	FD = socket(AF_INET, SOCK_STREAM, 0);
	if (FD < 0){   
		perror("socket creation failed\n");
		exit(10);
	} /* fi */
	printf("Connecting to the server at port %d...\n", PortNo);
	/* this is where connection happens */
	if (connect(FD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0) {   
		perror("connecting to server failed\n");
		exit(10);
	} /* fi */

	/* color declarations */
	GdkColor backColor;
	backColor.red = BACKR;
	backColor.green = BACKG;
	backColor.blue = BACKB;
	GdkColor butColor;
	butColor.red = BUTR;
	butColor.green = BUTG;
	butColor.blue = BUTB;
	GdkColor black;
	black.red = BLACKR;
	black.green = BLACKG;
	black.blue = BLACKB;
	GdkColor white;	
	white.red = WHITER;
	white.green = WHITEG;
	white.blue = WHITEB;
 
	/* START WINDOW GTK STUFF */
	GtkBuilder * builder;
	builder = gtk_builder_new();
	if (gtk_builder_add_from_file (builder, "gui/StartMenu.glade", NULL) == 0 ) {
		printf("Error loading from StartMenu.glade\n");
		exit(0);
	} /* fi */
	StartWindow = GTK_WIDGET(gtk_builder_get_object(builder, "StartWindow"));
	ProgQuit = GTK_WIDGET(gtk_builder_get_object(builder, "ProgQuit"));
	ProgStart = GTK_WIDGET(gtk_builder_get_object(builder, "ProgStart"));
	g_signal_connect(StartWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_modify_bg(StartWindow, GTK_STATE_NORMAL, &backColor);

	/* LOGIN WINDOW GTK STUFF */	
	if (gtk_builder_add_from_file (builder, "gui/Login.glade", NULL) == 0 ) {
		printf("Error loading from Login.glade\n");
		exit(0);
	} /* fi */
	LoginWindow = GTK_WIDGET(gtk_builder_get_object(builder, "LoginWindow"));
	g_signal_connect(LoginWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	loginButton1 = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton1"));
	loginButton2 = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton2"));
	loginButton3 = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton3"));
	incorrectLabel = GTK_WIDGET(gtk_builder_get_object(builder, "incorrectLabel"));
	usernameEntry = GTK_ENTRY(gtk_builder_get_object(builder, "usernameEntry"));
	passwordEntry = GTK_ENTRY(gtk_builder_get_object(builder, "passwordEntry"));
	gtk_widget_modify_bg(loginButton1, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(loginButton2, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(loginButton3, GTK_STATE_NORMAL, &butColor);

	/* CREATE NEW ACCOUNT WINDOW GTK STUFF */
	if (gtk_builder_add_from_file(builder, "gui/createNew.glade", NULL) == 0) {
		printf("Error loading from createNew.glade\n");
		exit(0);
	} /* fi */
	CreateNewWindow = GTK_WIDGET(gtk_builder_get_object(builder, "CreateNewWindow"));
	gtk_builder_connect_signals(builder, NULL);
	newUsernameEntry = GTK_ENTRY(gtk_builder_get_object(builder, "newUsernameEntry"));
	newPwEntry1 = GTK_ENTRY(gtk_builder_get_object(builder, "newPwEntry1"));
	newPwEntry2 = GTK_ENTRY(gtk_builder_get_object(builder, "newPwEntry2"));
	newButton1 = GTK_WIDGET(gtk_builder_get_object(builder, "newButton1"));
	newButton2 = GTK_WIDGET(gtk_builder_get_object(builder, "newButton2"));
	takenLabel = GTK_WIDGET(gtk_builder_get_object(builder, "takenLabel"));
//	gtk_widget_modify_bg(CreateNewWindow, GTK_STATE_NORMAL, &backColor);
	gtk_widget_modify_bg(newButton1, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(newButton2, GTK_STATE_NORMAL, &butColor);

	/* FRIEND REQUEST POPUP WINDOW GTK STUFF */	
	if (gtk_builder_add_from_file (builder, "gui/FriendRequest.glade", NULL) == 0 ) {
		printf("Error loading from FriendRequest.glade\n");
		exit(0);
	} /* fi */
	friendReqWindow = GTK_WIDGET(gtk_builder_get_object(builder, "friendReqWindow"));
	g_signal_connect(friendReqWindow, "destroy", G_CALLBACK(on_rejectButton_clicked), NULL);
	acceptButton = GTK_WIDGET(gtk_builder_get_object(builder, "acceptButton"));
	rejectButton = GTK_WIDGET(gtk_builder_get_object(builder, "rejectButton"));
	requestLabel = GTK_WIDGET(gtk_builder_get_object(builder, "requestLabel"));
	gtk_widget_modify_bg(acceptButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(rejectButton, GTK_STATE_NORMAL, &butColor);
	g_signal_connect(acceptButton, "clicked", G_CALLBACK(on_acceptButton_clicked), user);


	/* GAME REQUEST POPUP WINDOW GTK STUFF */	
	if (gtk_builder_add_from_file (builder, "gui/GameRequest.glade", NULL) == 0 ) {
		printf("Error loading from GameRequest.glade\n");
		exit(0);
	} /* fi */
	gameReqWindow = GTK_WIDGET(gtk_builder_get_object(builder, "gameReqWindow"));
//	g_signal_connect(gameReqWindow, "destroy", G_CALLBACK(on_rejectGameButton_clicked), NULL);
	acceptGameButton = GTK_WIDGET(gtk_builder_get_object(builder, "acceptGameButton"));
	rejectGameButton = GTK_WIDGET(gtk_builder_get_object(builder, "rejectGameButton"));
	requestGamesLabel = GTK_WIDGET(gtk_builder_get_object(builder, "requestGamesLabel"));
	gtk_widget_modify_bg(acceptGameButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(rejectGameButton, GTK_STATE_NORMAL, &butColor);
//	g_signal_connect(acceptGameButton, "clicked", G_CALLBACK(on_acceptGameButton_clicked), user);


	/* MAIN MENU GTK STUFF */
	if (gtk_builder_add_from_file(builder, "gui/MainMenu.glade", NULL) == 0) {
		printf("Error loading from CreateNew.glade\n");
		exit(0);
	} /* fi */
	MainMenuWindow = GTK_WIDGET(gtk_builder_get_object(builder, "MainMenuWindow"));
	g_signal_connect(MainMenuWindow, "destroy", G_CALLBACK(EndProgram), user);
	gtk_builder_connect_signals(builder, NULL);
	fixed3 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"));
	mephistoLogo3 = GTK_WIDGET(gtk_builder_get_object(builder, "mephistoLogo3"));
	usernameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "usernameLabel"));
	onlineLabel = GTK_WIDGET(gtk_builder_get_object(builder, "onlineLabel"));
	friendsLabel = GTK_WIDGET(gtk_builder_get_object(builder, "friendsLabel"));
	mainButton3 = GTK_WIDGET(gtk_builder_get_object(builder, "mainButton3"));
	mainButton5 = GTK_WIDGET(gtk_builder_get_object(builder, "mainButton5"));
	scrolledwindow1 = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow1"));
	scrolledwindow2 = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow2"));
	scrolledwindow3 = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow3"));
	textview1 = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview1"));
	textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));
	textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));
	mainButton1 = GTK_WIDGET(gtk_builder_get_object(builder, "mainButton1"));
	mainButton2 = GTK_WIDGET(gtk_builder_get_object(builder, "mainButton2"));
	refresh = GTK_WIDGET(gtk_builder_get_object(builder, "refresh"));
	addRemoveFrButton = GTK_WIDGET(gtk_builder_get_object(builder, "addRemoveFrButton"));
	entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
	g_signal_connect(mainButton5, "clicked", G_CALLBACK(EndProgram), user);
	/* color the gtk window */
	gtk_widget_modify_bg(mainButton1, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(mainButton2, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(sendButton, GTK_STATE_NORMAL, &butColor);

	/* chess interface declarations and aesthetic modifications */
	if (gtk_builder_add_from_file(builder, "gui/chessInterface.glade", NULL) == 0) {
		printf("error loading chessInterface.glade\n");
		exit(0);
	} /* fi */

	chessWindow = GTK_WIDGET(gtk_builder_get_object(builder, "chessWindow"));

	g_signal_connect(chessWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);

	board = GTK_WIDGET(gtk_builder_get_object(builder, "board"));
	graveyard = GTK_WIDGET(gtk_builder_get_object(builder, "graveyard"));
	
	a1button = GTK_WIDGET(gtk_builder_get_object(builder, "a1button"));
	a2button = GTK_WIDGET(gtk_builder_get_object(builder, "a2button"));
	a3button = GTK_WIDGET(gtk_builder_get_object(builder, "a3button"));
	a4button = GTK_WIDGET(gtk_builder_get_object(builder, "a4button"));
	a5button = GTK_WIDGET(gtk_builder_get_object(builder, "a5button"));
	a6button = GTK_WIDGET(gtk_builder_get_object(builder, "a6button"));
	a7button = GTK_WIDGET(gtk_builder_get_object(builder, "a7button"));
	a8button = GTK_WIDGET(gtk_builder_get_object(builder, "a8button"));

	b1button = GTK_WIDGET(gtk_builder_get_object(builder, "b1button"));
	b2button = GTK_WIDGET(gtk_builder_get_object(builder, "b2button"));
	b3button = GTK_WIDGET(gtk_builder_get_object(builder, "b3button"));
	b4button = GTK_WIDGET(gtk_builder_get_object(builder, "b4button"));
	b5button = GTK_WIDGET(gtk_builder_get_object(builder, "b5button"));
	b6button = GTK_WIDGET(gtk_builder_get_object(builder, "b6button"));
	b7button = GTK_WIDGET(gtk_builder_get_object(builder, "b7button"));
	b8button = GTK_WIDGET(gtk_builder_get_object(builder, "b8button"));

	c1button = GTK_WIDGET(gtk_builder_get_object(builder, "c1button"));
	c2button = GTK_WIDGET(gtk_builder_get_object(builder, "c2button"));
	c3button = GTK_WIDGET(gtk_builder_get_object(builder, "c3button"));
	c4button = GTK_WIDGET(gtk_builder_get_object(builder, "c4button"));
	c5button = GTK_WIDGET(gtk_builder_get_object(builder, "c5button"));
	c6button = GTK_WIDGET(gtk_builder_get_object(builder, "c6button"));
	c7button = GTK_WIDGET(gtk_builder_get_object(builder, "c7button"));
	c8button = GTK_WIDGET(gtk_builder_get_object(builder, "c8button"));

	d1button = GTK_WIDGET(gtk_builder_get_object(builder, "d1button"));
	d2button = GTK_WIDGET(gtk_builder_get_object(builder, "d2button"));
	d3button = GTK_WIDGET(gtk_builder_get_object(builder, "d3button"));
	d4button = GTK_WIDGET(gtk_builder_get_object(builder, "d4button"));
	d5button = GTK_WIDGET(gtk_builder_get_object(builder, "d5button"));
	d6button = GTK_WIDGET(gtk_builder_get_object(builder, "d6button"));
	d7button = GTK_WIDGET(gtk_builder_get_object(builder, "d7button"));
	d8button = GTK_WIDGET(gtk_builder_get_object(builder, "d8button"));

	e1button = GTK_WIDGET(gtk_builder_get_object(builder, "e1button"));
	e2button = GTK_WIDGET(gtk_builder_get_object(builder, "e2button"));
	e3button = GTK_WIDGET(gtk_builder_get_object(builder, "e3button"));
	e4button = GTK_WIDGET(gtk_builder_get_object(builder, "e4button"));
	e5button = GTK_WIDGET(gtk_builder_get_object(builder, "e5button"));
	e6button = GTK_WIDGET(gtk_builder_get_object(builder, "e6button"));
	e7button = GTK_WIDGET(gtk_builder_get_object(builder, "e7button"));
	e8button = GTK_WIDGET(gtk_builder_get_object(builder, "e8button"));

	f1button = GTK_WIDGET(gtk_builder_get_object(builder, "f1button"));
	f2button = GTK_WIDGET(gtk_builder_get_object(builder, "f2button"));
	f3button = GTK_WIDGET(gtk_builder_get_object(builder, "f3button"));
	f4button = GTK_WIDGET(gtk_builder_get_object(builder, "f4button"));
	f5button = GTK_WIDGET(gtk_builder_get_object(builder, "f5button"));
	f6button = GTK_WIDGET(gtk_builder_get_object(builder, "f6button"));
	f7button = GTK_WIDGET(gtk_builder_get_object(builder, "f7button"));
	f8button = GTK_WIDGET(gtk_builder_get_object(builder, "f8button"));

	g1button = GTK_WIDGET(gtk_builder_get_object(builder, "g1button"));
	g2button = GTK_WIDGET(gtk_builder_get_object(builder, "g2button"));
	g3button = GTK_WIDGET(gtk_builder_get_object(builder, "g3button"));
	g4button = GTK_WIDGET(gtk_builder_get_object(builder, "g4button"));
	g5button = GTK_WIDGET(gtk_builder_get_object(builder, "g5button"));
	g6button = GTK_WIDGET(gtk_builder_get_object(builder, "g6button"));
	g7button = GTK_WIDGET(gtk_builder_get_object(builder, "g7button"));
	g8button = GTK_WIDGET(gtk_builder_get_object(builder, "g8button"));

	h1button = GTK_WIDGET(gtk_builder_get_object(builder, "h1button"));
	h2button = GTK_WIDGET(gtk_builder_get_object(builder, "h2button"));
	h3button = GTK_WIDGET(gtk_builder_get_object(builder, "h3button"));
	h4button = GTK_WIDGET(gtk_builder_get_object(builder, "h4button"));
	h5button = GTK_WIDGET(gtk_builder_get_object(builder, "h5button"));
	h6button = GTK_WIDGET(gtk_builder_get_object(builder, "h6button"));	
	h7button = GTK_WIDGET(gtk_builder_get_object(builder, "h7button"));
	h8button = GTK_WIDGET(gtk_builder_get_object(builder, "h8button"));
	
	moveCount = GTK_WIDGET(gtk_builder_get_object(builder, "moveCount"));
	playerColor = GTK_WIDGET(gtk_builder_get_object(builder, "playerColor"));
	gameMessage = GTK_WIDGET(gtk_builder_get_object(builder, "gameMessage"));
	capturedPieces = GTK_WIDGET(gtk_builder_get_object(builder, "capturedPieces"));
	chattingBox  = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "chattingBox"));
	friendBox = GTK_WIDGET(gtk_builder_get_object(builder, "friendBox"));
	message = GTK_ENTRY(gtk_builder_get_object(builder, "message"));
	buffer1 = gtk_text_buffer_new(NULL);

	refreshButton = GTK_WIDGET(gtk_builder_get_object(builder, "refreshButton"));
        sendButton = GTK_WIDGET(gtk_builder_get_object(builder, "sendButton"));
        QuitButton = GTK_WIDGET(gtk_builder_get_object(builder, "QuitButton"));
        HelpButton = GTK_WIDGET(gtk_builder_get_object(builder, "HelpButton"));
	AddRemoveButton = GTK_WIDGET(gtk_builder_get_object(builder, "AddRemoveButton"));
	returnToLobby = GTK_WIDGET(gtk_builder_get_object(builder, "returnToLobby"));
	g_signal_connect(refreshButton, "clicked", G_CALLBACK(on_refreshButton_clicked), user);
	
	gtk_widget_modify_bg(refreshButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(sendButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(QuitButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(HelpButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(AddRemoveButton, GTK_STATE_NORMAL, &butColor);
	gtk_widget_modify_bg(returnToLobby, GTK_STATE_NORMAL, &butColor);
	
	/* white board positions */		
	gtk_widget_modify_bg(a2button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(a4button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(a6button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(a8button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(b1button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(b3button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(b5button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(b7button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(c2button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(c4button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(c6button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(c8button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(d1button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(d3button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(d5button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(d7button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(e2button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(e4button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(e6button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(e8button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(f1button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(f3button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(f5button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(f7button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(g2button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(g4button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(g6button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(g8button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(h1button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(h3button, GTK_STATE_NORMAL, &white);
        gtk_widget_modify_bg(h5button, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_bg(h7button, GTK_STATE_NORMAL, &white);

	/*black board postions */
	gtk_widget_modify_bg(a1button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(a3button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(a5button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(a7button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(b2button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(b4button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(b6button, GTK_STATE_NORMAL, &black);
	gtk_widget_modify_bg(b8button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(c1button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(c3button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(c5button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(c7button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(d2button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(d4button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(d6button, GTK_STATE_NORMAL, &black);
	gtk_widget_modify_bg(d8button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(e1button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(e3button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(e5button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(e7button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(f2button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(f4button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(f6button, GTK_STATE_NORMAL, &black);
	gtk_widget_modify_bg(f8button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(g1button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(g3button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(g5button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(g7button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(h2button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(h4button, GTK_STATE_NORMAL, &black);
        gtk_widget_modify_bg(h6button, GTK_STATE_NORMAL, &black);
	gtk_widget_modify_bg(h8button, GTK_STATE_NORMAL, &black);

	/* REQUEST GAME declarations and modifications */
	if (gtk_builder_add_from_file(builder, "gui/NewGame.glade", NULL) == 0) {
                printf("error loading NewGame.glade\n");
                exit(0);
        } /* fi */
        playGameWindow = GTK_WIDGET(gtk_builder_get_object(builder, "playGameWindow"));
        gtk_builder_connect_signals(builder, NULL);
	playGameButton = GTK_WIDGET(gtk_builder_get_object(builder, "playGameButton"));
	returnButton1 = GTK_WIDGET(gtk_builder_get_object(builder, "returnButton1"));
	userGameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "userGameLabel"));
	errorMessage1 = GTK_WIDGET(gtk_builder_get_object(builder, "errorMessage1"));
	gameUsername = GTK_ENTRY(gtk_builder_get_object(builder, "gameUsername"));

	gtk_widget_modify_bg(playGameButton, GTK_STATE_NORMAL, &butColor);
        gtk_widget_modify_bg(returnButton1, GTK_STATE_NORMAL, &butColor);
//	g_signal_connect(removeButton, "clicked", G_CALLBACK(on_removeButton_clicked), user);
	
	/* help window declarations and modifications */
        if (gtk_builder_add_from_file(builder, "gui/help.glade", NULL) == 0) {
                printf("error loading help.glade\n");
                exit(0);
        } /* fi */


	/* Add Remove window declarations and modifications */
	if (gtk_builder_add_from_file(builder, "gui/AddRemove.glade", NULL) == 0) {
                printf("error loading AddRemove.glade\n");
                exit(0);
        } /* fi */

        addRemoveWindow = GTK_WIDGET(gtk_builder_get_object(builder, "addRemoveWindow"));

        g_signal_connect(addRemoveWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_builder_connect_signals(builder, NULL);

	addButton = GTK_WIDGET(gtk_builder_get_object(builder, "addButton"));
	removeButton = GTK_WIDGET(gtk_builder_get_object(builder, "removeButton"));
	username = GTK_WIDGET(gtk_builder_get_object(builder, "username"));
	errorMessage = GTK_WIDGET(gtk_builder_get_object(builder, "errorMessage"));

	gtk_widget_modify_bg(addButton, GTK_STATE_NORMAL, &butColor);
        gtk_widget_modify_bg(removeButton, GTK_STATE_NORMAL, &butColor);
	g_signal_connect(removeButton, "clicked", G_CALLBACK(on_removeButton_clicked), user);
	
	/* help window declarations and modifications */
        if (gtk_builder_add_from_file(builder, "gui/help.glade", NULL) == 0) {
                printf("error loading help.glade\n");
                exit(0);
        } /* fi */

        helpWindow = GTK_WIDGET(gtk_builder_get_object(builder, "helpWindow"));

        g_signal_connect(helpWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);
        returnToGameButton = GTK_WIDGET(gtk_builder_get_object(builder, "returnToGameButton"));
	helpMessage = GTK_WIDGET(gtk_builder_get_object(builder, "helpMessage"));

    	gtk_widget_modify_bg(returnToGameButton, GTK_STATE_NORMAL, &butColor);

	/* you win window */
	 if (gtk_builder_add_from_file(builder, "gui/youWin.glade", NULL) == 0) {
                printf("error loading youWin.glade\n");
                exit(0);
        } /* fi */

        youWinWindow = GTK_WIDGET(gtk_builder_get_object(builder, "youWinWindow"));
	okButton = GTK_WIDGET(gtk_builder_get_object(builder, "okButton"));
	gtk_widget_modify_bg(youWinWindow, GTK_STATE_NORMAL, &backColor);
        g_signal_connect(youWinWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_builder_connect_signals(builder, NULL);

	/* you lose window */
	 if (gtk_builder_add_from_file(builder, "gui/youLose.glade", NULL) == 0) {
                printf("error loading youLose.glade\n");
                exit(0);
        } /* fi */

        youLoseWindow = GTK_WIDGET(gtk_builder_get_object(builder, "youLoseWindow"));
	ok = GTK_WIDGET(gtk_builder_get_object(builder, "ok"));
	gtk_widget_modify_bg(youLoseWindow, GTK_STATE_NORMAL, &backColor);
        g_signal_connect(youLoseWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_builder_connect_signals(builder, NULL);	

	/* i guess you win window */	
	 if (gtk_builder_add_from_file(builder, "gui/iGuessYouWin.glade", NULL) == 0) {
                printf("error loading iGuessYouWin.glade\n");
                exit(0);
        } /* fi */

        iGuessWindow = GTK_WIDGET(gtk_builder_get_object(builder, "iGuessWindow"));
	okay = GTK_WIDGET(gtk_builder_get_object(builder, "okay"));
	gtk_widget_modify_bg(iGuessWindow, GTK_STATE_NORMAL, &backColor);
        g_signal_connect(iGuessWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_builder_connect_signals(builder, NULL);

	/* local variables */
	GtkWidget *temp;
	GtkWidget *tempButton;
	user->game = CreateGame();
	InitializeNewGame(user->game);
	char buttonName[9];
	int locMove[4];
	int valid;
	 
	
//	g_timeout_add (1000, (GSourceFunc)Timeout, NULL);


	/* GAME CONTROL STARTS HERE */
	gtk_widget_show(StartWindow);
	gtk_main();

	if ( quitLogin == 1){
		/* send a signal to server that you bouta quit */
		DeleteGame(user->game);
		return 0;
	} /* fi */
	
	do {
		gtk_widget_show(LoginWindow);
		gtk_main();
	} while (returnToLogin == 1); /* elihw */

	if (quitLogin == 1) {
		/* disconnect from server */
		/* free all structs */
		return 0;
	} /* fi */
	strcpy(user->username, uName);

	do {
		gtk_label_set_text(GTK_LABEL(usernameLabel), (const gchar*) user->username);
		gtk_widget_show(MainMenuWindow);
		gtk_main();
		 
		if (quit == 1) {
			return 0;
		} /* fi */
		else if ( chessFlag == 1) {
		    do {	
			gtk_widget_show(chessWindow);
			gtk_label_set_text(GTK_LABEL(yourUsername), (const gchar*) uName);
			gtk_label_set_text(GTK_LABEL(opponentUsername), (const gchar*) oppName);
			
			gtk_main(); 
			/* clears error message after the next signal is made */
			gtk_label_set_text(GTK_LABEL(gameMessage), (const gchar*) "");
			
			/* position array is filled */
			if (position[3] != '\0'){
				position[4] = 0;
				#ifdef DEBUG
				printf("smith string: %s\n", position);
				#endif
				SmithToArray(position, locMove);
				#ifdef DEBUG
				printf("smith: %s, arr: %d%d%d%d\n", position, locMove[0], locMove[1], locMove[2], locMove[3]);
				#endif
				/* move is valid, so move piece */
				valid = CheckValid(locMove, user->game);
				if (valid == 0){		
					/*update game board */
					MovePiece(locMove, user->game, NULL);
					user->game->count++;
					printf("making move");

					/* gets the origin button's image */
					buttonName[0] = position[0];
					buttonName[1] = position[1];
					buttonName[2] = 0;
					strcat(buttonName, "button");
					printf("line 540, buttonName = %s\n", buttonName);
					tempButton = GTK_WIDGET(gtk_builder_get_object(builder, buttonName));
					temp = gtk_button_get_image(GTK_BUTTON(tempButton));
	
					/* gets the destination button */
					buttonName[0] = position[2];
	                        	buttonName[1] = position[3];
	                       
					/* sets the destination button with the origin's button */
	                        	tempButton = GTK_WIDGET(gtk_builder_get_object(builder, buttonName));
	                        	gtk_button_set_image(GTK_BUTTON(tempButton), temp);
	                        	gtk_widget_show(temp);
				} /* fi */
				/* move is not valid, so error message is displayed */
				else {
					gtk_label_set_text(GTK_LABEL(gameMessage), (const gchar*) PrintErrorCode(valid, user->game->count));
				} /* esle */
                        	/* resets position array*/
                        	for (int i = 0; i < sizeof(position); i++){
                        	        position[i] = '\0';
                        	} /* rof */
			} /* fi esle */
		} while (lobby == 0);	
	    } /* fi esle */
	} while (quit == 0);

	
	/* send 0 to server to quit */
	if (quit == 1){
		DeleteGame(user->game);
		return 0;
	}

	return 1;
}


//////////////////////////
/* Function Definitions */
//////////////////////////

void EndProgram( USER * user ) {
	char SendBuff[4096], RecBuff[4096];
	int n;
	/* stuff to happen when start button is clicked */
	/* close StartWindow */
	gtk_widget_hide(StartWindow);
	
	/* DISCONNECT from server */
	strcpy(SendBuff, "DISCONNECT ");
	strcat(SendBuff, uName);
	n = write( FD, SendBuff, strlen(SendBuff));
	if (n < 0){   
		perror("writing to socket failed");
		exit(10);
	} /* fi */

        /* get response from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);

	close(FD);
	DeleteUser(user);
	exit(10);	
}
 
GSourceFunc Timeout (gpointer user_data ) {
	printf("Timeout: Read from socket\n");
	return 0;
}

void on_ProgStart_clicked (GtkButton *b) {

	gtk_main_quit();
	char SendBuff[4096], RecBuff[4096];
	int n;
	/* stuff to happen when start button is clicked */
	/* close StartWindow */
	gtk_widget_hide(StartWindow);
	quitLogin = 0;
	
	/* CONNECT with server */
	strcpy(SendBuff, "CONNECT");
	n = write( FD, SendBuff, strlen(SendBuff));
	if (n < 0){   
		perror("writing to socket failed");
		exit(10);
	} /* fi */

	/* get guest username from servr */
	n = read( FD, RecBuff, sizeof(RecBuff)-1);
	RecBuff[n] = 0;
	printf("Received message: %s\n", RecBuff);
	strtok(RecBuff, " ");
	char * name = strtok(NULL, " ");
	strcpy( uName, name);
	return;
} 
 
void on_ProgQuit_clicked (GtkButton *b) {
	
	gtk_widget_hide(StartWindow);
	quitLogin = 1;
	gtk_main_quit();
	return;
} 

/* create account */
void on_loginButton1_clicked( GtkButton *b) {
	
	/* open create new account menu */
	gtk_widget_hide(LoginWindow);
	gtk_widget_show(CreateNewWindow);
	return;
}

/* play as guest */
void on_loginButton2_clicked( GtkButton *b) {
	
	/* login as guest */
	/* close this window */
	gtk_widget_hide(LoginWindow);
	create = 0;
	returnToLogin = 0;
	gtk_main_quit();
	/* go to main menu */
	return;
}

/* login button in login window*/
void on_loginButton3_clicked( GtkButton *b) {

	char SendBuff[4096], RecBuff[4096];
	int n;	
	/* try to login */
	/* get username string */
	char un[21];
	char pw[21];
	sprintf(un, gtk_entry_get_text( GTK_ENTRY(usernameEntry) ));
	sprintf(pw, gtk_entry_get_text( GTK_ENTRY(passwordEntry) ));

	/* LOGIN with server */
	strcpy(SendBuff, "LOGIN ");
	strcat(SendBuff, un);
	strcat(SendBuff, " ");
	strcat(SendBuff, pw);
	n = write( FD, SendBuff, strlen(SendBuff));
	if (n < 0){   
		perror("writing to socket failed");
		exit(10);
	} /* fi */

			/* get response from servr */
			n = read( FD, RecBuff, sizeof(RecBuff)-1);
	RecBuff[n] = 0;
	printf("Received message: %s\n", RecBuff);
	char * response = strtok(RecBuff, " ");
	if ( 0 == strcmp(response, "LOGINFAILED")) {
		gtk_label_set_text(GTK_LABEL(incorrectLabel), (const gchar*) "Incorrect username or password. Please try again."); 
	} /* fi */
	else if ( 0 == strcmp(response, "LOGINSUCCESS")) {
		char * name = strtok(NULL, " ");
		strcpy( uName, name);
		guestFlag = 0;
		gtk_widget_hide(LoginWindow);
		returnToLogin = 0;
		create = 0;
	} /* fi esle */
	gtk_main_quit();
	return;
}

/* create new account */
void on_newButton1_clicked( GtkButton *b) {
	/* get username and password strings */
	char RecBuff[4096], SendBuff[4096];
	char un[21];
	char pw1[21];
	char pw2[21];
	int n;
	sprintf(un, gtk_entry_get_text( GTK_ENTRY(newUsernameEntry) ));
	sprintf(pw1, gtk_entry_get_text( GTK_ENTRY(newPwEntry1) ));
	sprintf(pw2, gtk_entry_get_text( GTK_ENTRY(newPwEntry2) ));
	#ifdef TESTING
	printf("Username: %s\nPassword1: %s\nPassword2: %s\n", un, pw1, pw2);
	#endif 

	/* passwords don't match */
	if (strcmp(pw1, pw2) != 0 ) {
		gtk_label_set_text(GTK_LABEL(takenLabel), (const gchar*) "Passwords do not match. Please try again."); 
	} /* fi */
	else {
		/* REGISTER with server */
		strcpy(SendBuff, "REGISTER ");
		strcat(SendBuff, un);
		strcat(SendBuff, " ");
		strcat(SendBuff, pw1);
		n = write( FD, SendBuff, strlen(SendBuff));
		if (n < 0){   
			perror("writing to socket failed");
			exit(10);
		} /* fi */

		/* get response from servr */
		n = read( FD, RecBuff, sizeof(RecBuff)-1);
		RecBuff[n] = 0;
		printf("Received message: %s\n", RecBuff);
		char * response = strtok(RecBuff, " ");
		if ( 0 == strcmp(response, "NOUSR")) {
			gtk_label_set_text(GTK_LABEL(takenLabel), (const gchar*) "Unable to create account. Try a different username."); 
		} /* fi */
		else if ( 0 == strcmp(response, "OKUSR")) {
			/*  else create a new account */	
			gtk_widget_hide(CreateNewWindow);
			gtk_main_quit();
			returnToLogin = 1;
		} /* fi esle */
	} /* esle */
	return;
} 


/* return to login menu */
void on_newButton2_clicked( GtkButton *b) {
	/* go to main menu without logging in */
	gtk_widget_hide(CreateNewWindow);
	gtk_main_quit();
	returnToLogin = 1;
	return;
}

/* login */
void on_mainButton1_clicked( GtkButton *b) {
	/* open login menu */	
	gtk_widget_show(LoginWindow);
	return;
}

/* logout */
void on_mainButton2_clicked( GtkButton *b) {
	/* send message to server to log out*/
	/* send message to server to login as guest */
	/* refresh main menu window */
	char SendBuff[4096], RecBuff[4096];
        int n;
        /* CONNECT with server */
        strcpy(SendBuff, "LOGOUT ");
	strcat(SendBuff, uName);
        n = write( FD, SendBuff, strlen(SendBuff));
        if (n < 0){
                perror("writing to socket failed");
                exit(10);
        } /* fi */

        /* get guest username from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);
        strtok(RecBuff, " ");
	strcpy(uName, strtok(NULL, " "));
	gtk_label_set_text(GTK_LABEL(usernameLabel), (const gchar*) uName);
	return;
}

/* new game */
void on_mainButton3_clicked( GtkButton *b) {
	/* go to main menu without logging in */
        gtk_widget_show(playGameWindow);
	return;
}

/* send message */
void on_sendIMButton_clicked( GtkButton *b) {
	char IM[161];
	char mymessagearr[161]; 
	char RecBuff[4096], SendBuff[4096];
	char sendarr[21]; 
	char *sendto; 
	char *msg; 
	int n;

	#ifdef TESTING
		printf("Instant Message: %s\n", IM);
	#endif 
	
	printf("message sent\n");
		
	gchar *mymessage2;  

	mymessage2 = gtk_entry_get_text(GTK_ENTRY(message));
		
	if (strlen(mymessage2) == 0) 
	{ 
		mymessage2 = gtk_entry_get_text(GTK_ENTRY(entry1));
	}
  
 	sprintf(mymessagearr, mymessage2); 

	sendto = strtok(mymessagearr, ":");
	msg = strtok(NULL, "\0");

	strcpy(sendarr, sendto); 
	strcpy(IM, msg);
	mymessage1 = g_strconcat(mymessage1, uName, ": ", (const gchar *) (msg) , "\n", NULL);  
	  
	gtk_text_buffer_set_text(buffer1, mymessage1, strlen(mymessage1)); 
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(chattingBox), buffer1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview3), buffer1); 
	gtk_entry_set_text( GTK_ENTRY(entry1), "\0"); 
	gtk_entry_set_text( GTK_ENTRY(message), "\0");
	
	strcpy(SendBuff, "IM ");
	strcat(SendBuff, uName);
	strcat(SendBuff, " ");
    
    // TODO: PLACEHOLDER FOR NOW, HARDCODED GUEST NAMES // 
/*
	if (0 == strcmp(uName, "guest5"))
	{
		strcat(SendBuff, "guest4 ");
	}

	else if (0 == strcmp(uName, "guest4"))
	{
		strcat(SendBuff, "guest5 ");
	} 
*/
	strcat(SendBuff, sendto); 
	strcat(SendBuff, " "); 
	strcat(SendBuff, msg); 
	strcat(SendBuff, " "); 
	n = write( FD, SendBuff, strlen(SendBuff));
	if (n < 0){   
		perror("writing to socket failed");
		exit(10);
	} /* fi */

	/* get response from servr */
    	n = read( FD, RecBuff, sizeof(RecBuff)-1);
	RecBuff[n] = 0;
	printf("Received message: %s\n", RecBuff);
	return;
}

/* opens up addRemove window when add/remove is clicked */
void on_AddRemoveButton_clicked (GtkButton *button){
        gtk_widget_show(addRemoveWindow);
	return;
}

/* request game window buttons */
void on_playGameButton_clicked (GtkButton *button){
	char SendBuff[4096], RecBuff[4096];
        int n;
        /* send game request */
        /* get username string */
        char friendUN[21];
        sprintf(friendUN, gtk_entry_get_text( GTK_ENTRY(gameUsername) ));
	#ifdef DEBUG
	printf("game request friend name: %s\n", friendUN);
	#endif

        /* send friend Req with server */
        strcpy(SendBuff, "GAMEREQ ");
        strcat(SendBuff, friendUN);
	strcat(SendBuff, " ");
	strcat(SendBuff, uName);

	/* send request to server */
        n = write( FD, SendBuff, strlen(SendBuff));
        if (n < 0){
                perror("writing to socket failed");
                exit(10);
        } /* fi */

        /* get response from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);
        char * response = strtok(RecBuff, " ");
        if ( 0 == strcmp(response, "NOGAMEREQ")) {
                gtk_label_set_text(GTK_LABEL(errorMessage), (const gchar*) "Username not found. Please try again.");
        } /* fi */
	else if ( 0 == strcmp(response, "OKGAMEREQ")) {
		gtk_widget_hide(playGameWindow);
	} /* fi esle */
        return;
}
/* addRemove window buttons */
void on_addButton_clicked (GtkButton *button){
	char SendBuff[4096], RecBuff[4096];
        int n;
        /* try to add friend */
        /* get username string */
        char friendUN[21];
        sprintf(friendUN, gtk_entry_get_text( GTK_ENTRY(username) ));

        /* send friend Req with server */
        strcpy(SendBuff, "FRREQ ");
        strcat(SendBuff, friendUN);
	strcat(SendBuff, " ");
	strcat(SendBuff, uName);

	/* send request to server */
        n = write( FD, SendBuff, strlen(SendBuff));
        if (n < 0){
                perror("writing to socket failed");
                exit(10);
        } /* fi */

        /* get response from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);
        char * response = strtok(RecBuff, " ");
        if ( 0 == strcmp(response, "NOFRUSR")) {
                gtk_label_set_text(GTK_LABEL(errorMessage), (const gchar*) "Username not found. Please try again.");
        } /* fi */
	else if ( 0 == strcmp(response, "OKFRREQ")) {
		gtk_widget_hide(addRemoveWindow);
	} /* fi esle */
        return;
}


void on_removeButton_clicked (GtkButton *button, gpointer user){
        #ifdef DEBUG
	printf("remove clicked\n");
	#endif
	char friendUN[21];
	sprintf(friendUN, gtk_entry_get_text(GTK_ENTRY(username)));

	FRIEND * select = ((USER*)user)->friendList->first;
	for(int i  = 0; i < ((USER*)user)->friendList->length; i++) {
		if ( 0 == strcmp(select->username, friendUN)) {
			RemoveFriend(select->username, user);
			gtk_widget_hide(addRemoveWindow);
			return;
		} /* fi */
		else {
			select = select->next;
		} /* esle */
	} /* rof */
	gtk_label_set_text(GTK_LABEL(errorMessage), (const gchar*) "Friend not found. Please try again.");
        return;
}

/* help window buttons */
void on_returnToGameButton_clicked (GtkButton *button){
        gtk_widget_hide(helpWindow);
	gtk_widget_show(chessWindow);
	gtk_main_quit();
}

gboolean on_refreshButton_clicked(GtkButton *button, gpointer user){
	char SendBuff[4096], RecBuff[4096]; 
	char *endstr;
	char *request; 
	char fromUN[21];
	char IM[161];  
	int n;
	strcpy(SendBuff, "REFRESH ");
	strcat(SendBuff, uName);
	n = write(FD, SendBuff, strlen(SendBuff));

	GtkTextBuffer *buffer4; 
	buffer4 = gtk_text_buffer_new(NULL);
	char viewmsg [80] = "To send a message, use the form: <friend>: <your message>";
	gtk_text_buffer_set_text(buffer4, viewmsg, strlen(viewmsg));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview3), buffer4);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(chattingBox), buffer4); 
	if (n < 0) {
		perror("writing to socket failed"); 
		exit(10);
	} /* fi */ 

	n = read(FD, RecBuff, sizeof(RecBuff) - 1);
	RecBuff[n] = 0; 
	
	#ifdef DEBUG
	printf("Received message: %s\n", RecBuff); 
	#endif 

	// get request
	request = strtok_r(RecBuff, "]", &endstr); 
	#ifdef DEBUG
	printf("request is %s\n", request); 
	#endif 

	while (request != NULL)
	{
		char *endtoken;
		char *param = strtok_r(request, " ", &endtoken); 

		if (0 == strcmp("OK_REFRESH", param))
		{
		//	#ifdef DEBUG
			printf("param: %s\n", param); 
		//	#endif

			const gchar *onlineusers = "\n"; 
			char name[20]; 

			param = strtok_r(NULL, " ", &endtoken);
	
			while(param != NULL)
			{
				strcpy(name, param);
				onlineusers = g_strconcat(onlineusers, (const gchar *) (name), "\n", NULL); 
				param = strtok_r(NULL, " ", &endtoken); 	
			}
			GtkTextBuffer *buffer2; 
			buffer2 = gtk_text_buffer_new(NULL);
			
			gtk_text_buffer_set_text(buffer2, onlineusers, strlen(onlineusers));
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview1), buffer2); 
		}

		else if (0 == strcmp("OK_REFRESH_FR", param)){
			#ifdef DEBUG
			printf("param: %s\n", param);
			#endif
			
			const gchar *onlinefriends = "\n";
			char name[20];
			
			param = strtok_r(NULL, " ", &endtoken);
			while(param != NULL) {
				strcpy(name, param);
				onlinefriends =  g_strconcat(onlinefriends, (const gchar *) (name), "\n", NULL);
				param = strtok_r(NULL, " ", &endtoken);
			} /* elihw */
	
			GtkTextBuffer *buffer3;
			buffer3 = gtk_text_buffer_new(NULL);	
			gtk_text_buffer_set_text(buffer3, onlinefriends, strlen(onlinefriends));
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview2), buffer3); 	
		} /* fi esle */

		else if (0 == strcmp("GAMEREQ", param)) {
			char * frUN = strtok_r(NULL, " ", &endtoken);
			strcpy(oppName, frUN);
			char gameReqText[161] = "Game request from: ";
			strcat(gameReqText, frUN);
			gtk_label_set_text(GTK_LABEL(requestGamesLabel), (const gchar*) gameReqText);
			gtk_widget_show(gameReqWindow);
		} /* fi esle */

		else if (0 == strcmp("GAMEACPT", param)) {
			strcpy(fromUN, strtok_r(NULL, " ", &endtoken));
			chessFlag = 1;			
		}
			
		else if (0 == strcmp("GAMEREJ", param)) {

		}

		else if (0 == strcmp("FRREQ", param)) {
			char * frUN = strtok_r(NULL, " ", &endtoken);
			#ifdef DEBUG
			printf("friend request from %s\n", frUN);
			#endif
			strcpy(oppName, frUN);
			char requestText[160] = "Friend request from: ";
			strcat(requestText, frUN);
			gtk_label_set_text(GTK_LABEL(requestLabel), (const gchar*) requestText);
			gtk_widget_show(friendReqWindow);

		}

		else if (0 == strcmp("FRACPT", param)) {
			char * newFriend = strtok_r(NULL, " ", &endstr);
			AppendFriend(newFriend, (USER*)user);
		}

		else if (0 == strcmp("FRREJ", param)) {

		}

		else if (0 == strcmp("MOVE", param)) {
			char * move = strtok_r(NULL, " ", &endstr);
			strcpy(oppMove, move);
			moveFlag = 1;
		} /* fi esle */
		else if (0 == strcmp("MSG", param))
		{
			#ifdef DEBUG
			printf("entered this loop!!");
			#endif 
			// get rid of first command 
			strcpy(fromUN, strtok_r(NULL, " ", &endtoken));
			strcpy(IM, strtok_r(NULL, "\0", &endtoken));			
			mymessage1 = g_strconcat(mymessage1, (const gchar *)(fromUN), ": ", (const gchar *)(IM), NULL);
			gtk_text_buffer_set_text(buffer1, mymessage1, strlen(mymessage1));
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(chattingBox), buffer1);
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview3), buffer1);
			 
		}

		else if (0 == strcmp("SHUTDOWN", param)) {
			quit = 1;
		}

		request = strtok_r(NULL, "]", &endstr); 	
	}
	/* flags that indicate that control should be passed back to main() */
	if (chessFlag == 1 || quit == 1 || moveFlag == 1) {
		gtk_main_quit();
	} /* fi */
	return 0;
}


/* chess interface buttons */
//void on_refreshButton_clicked (GtkButton *button){
//	char* command1, *command2;
//	char fromUN[20]; 
//	char SendBuff[4096], RecBuff[4096];
//	char * IM;  
//        int n;
//
//        
//        strcpy(SendBuff, "REFRESH ");
//	strcat(SendBuff, uName);
//        n = write( FD, SendBuff, strlen(SendBuff));
//        if (n < 0){
//                perror("writing to socket failed");
//                exit(10);
//        } /* fi */
//
//        /* from servr */
//	//fcntl(FD, F_SETFL, O_NONBLOCK);
//        n = read( FD, RecBuff, sizeof(RecBuff)-1);
//        RecBuff[n] = 0;
//        printf("Received message: %s\n", RecBuff);
//
//	n = write( FD, SendBuff, strlen(SendBuff));
//	if (n < 0){   
//		perror("writing to socket failed");
//		exit(10);
//	} /* fi */
//
//	n = read(FD, RecBuff, sizeof(RecBuff) - 1);
//	RecBuff[n] = 0;
//	printf("Received message: %s\n", RecBuff); 
//
//	// TODO: need to keep parsing here and also update the online lobby
//	// some kind of while loop to properly parse through the entire RecBuff 
//	command1 = strtok(RecBuff, " "); 
//	
//	if (0 == strcmp(command1, "OK_REFRESH"))
//	{
//		const gchar *onlineusers = "\n"; 
//		char name[20]; 
//		// create temporary buffer 
//
//		strcpy(name, strtok(NULL, " "));
//		while(0 != strcmp(name, "&*%"))
//		{
//			onlineusers = g_strconcat(onlineusers, (const gchar *)(name), "\n", NULL); 	
//			strcpy(name, strtok(NULL, " ")) ; 
//		}
//		GtkTextBuffer *buffer2;
//		buffer2 = gtk_text_buffer_new(NULL);
//	
//		gtk_text_buffer_set_text(buffer2, onlineusers, strlen(onlineusers));
//		gtk_text_view_set_buffer(GTK_TEXT_VIEW(textview1), buffer2); 
//	}
//	if (0 == strcmp(command1, "OKSENT"))  
//	{
//		// keep parsing 
//		IM = strtok(NULL, " ");
//	}
//	if (0 == strcmp(command1, "MSG"))
//	{
//		strcpy(fromUN, strtok(NULL, " ")); 	
//		strcpy(IM, strtok(NULL, "&*%")); 
//		mymessage1 = g_strconcat(mymessage1, (const gchar *)(fromUN), ": ", (const gchar *)(IM), "\n", NULL);	
//		gtk_text_buffer_set_text(buffer1, mymessage1, strlen(mymessage1));
//		gtk_text_view_set_buffer(GTK_TEXT_VIEW(chattingBox), buffer1);	
//	}
//	if(0 == strcmp(command1, "FRREQ")) {
//		char * friendName = strtok(NULL, " ");
//		char * requestText = "Friend request from ";
//		strcat(requestText, friendName);
//		gtk_label_set_text(GTK_LABEL(requestLabel), (const gchar*) requestText);
//		gtk_widget_show(friendReqWindow);
//	} /* fi */
//
//        /* get response from servr */
//        n = read( FD, RecBuff, sizeof(RecBuff)-1);
//        RecBuff[n] = 0;
//        printf("Received message: %s\n", RecBuff);
//        command2 = strtok(RecBuff, " ");
//        if ( 0 == strcmp(command2, "MSG ")) {
//		strcpy(fromUN, strtok(NULL, " "));
//		message = (RecBuff + strlen(command2) + strlen(fromUN) + 2);
//		
//        } /* fi */
//	else if ( 0 == strcmp(command2, "FRACPT")) {
//		// append friend to friends list
//		// update the friendBox and textview2
//                gtk_label_set_text(GTK_LABEL(errorMessage), (const gchar*) "Friend added");
//                gtk_widget_hide(addRemoveWindow);
//        } /* fi esle */
//
//	/*
//	strcpy(SendBuff, "REFRESH_FR");
//        n = write( FD, SendBuff, strlen(SendBuff));
//        if (n < 0){
//                perror("writing to socket failed");
//                exit(10);
//        } 
//
//        n = read( FD, RecBuff, sizeof(RecBuff)-1);
//        RecBuff[n] = 0;
//        printf("Received message: %s\n", RecBuff);
//	*/
//
//
//	return;	 
//} 

void on_QuitButton_clicked (GtkButton *button){
       
	printf("quit clicked\n");
	quit = 1;
}
void on_HelpButton_clicked (GtkButton *button){
	gtk_widget_hide(chessWindow);
	gtk_main_quit();
	help = 1;	
}

void on_returnToLobby_clicked (GtkButton *button){
	gtk_widget_hide(chessWindow);
        lobby = 1;
	gtk_main_quit();
}

void on_acceptGameButton_clicked(GtkButton *button) {
	char SendBuff[4096], RecBuff[4096];
	int n;
	gtk_widget_hide(gameReqWindow);

        /* send GAMEACPT to server */
        strcpy(SendBuff, "GAMEACPT ");
        strcat(SendBuff, uName);
	strcat(SendBuff, " ");
	strcat(SendBuff, friendName);
	#ifdef DEBUG
	printf("accepted game: %s\n", SendBuff);
	#endif

        n = write( FD, SendBuff, strlen(SendBuff));
        if (n < 0){
                perror("writing to socket failed");
                exit(10);
	} /* fi */

        /* get OK from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);
        char * response = strtok(RecBuff, " ");
        if ( 0 == strcmp(response, "OKGM")) {
		strcpy(oppName, strtok(NULL, " "));
        	gtk_main_quit();
		chessFlag = 1;
		return;
	} /* fi */
	return;

}

/* FRIENDREQ signals */
gboolean on_acceptButton_clicked (GtkButton *button, gpointer user){

	char SendBuff[4096], RecBuff[4096];
	int n;
	gtk_widget_hide(friendReqWindow);

        /* send FRACPT to server */
        strcpy(SendBuff, "FRACPT ");
        strcat(SendBuff, uName);
	strcat(SendBuff, " ");
	strcat(SendBuff, friendName);

        n = write( FD, SendBuff, strlen(SendBuff));
        if (n < 0){
                perror("writing to socket failed");
                exit(10);
	} /* fi */

        /* get OK from servr */
        n = read( FD, RecBuff, sizeof(RecBuff)-1);
        RecBuff[n] = 0;
        printf("Received message: %s\n", RecBuff);
        AppendFriend(friendName, user);
	#ifdef DEBUG
	printf("added friend %s\n", friendName);
	#endif
	return 0;
}

void on_rejectGameButton_clicked(GtkButton *button){
	gtk_widget_hide(gameReqWindow);
	return;
}

void on_rejectButton_clicked(GtkButton *button){
	gtk_widget_hide(friendReqWindow);
	return;
}

/* chess board position positions */
void on_a1button_clicked (GtkButton *button){
        strcat(position, "a1");
        gtk_main_quit();
}
void on_a2button_clicked (GtkButton *button){
        strcat(position, "a2");
        gtk_main_quit();
}
void on_a3button_clicked (GtkButton *button){
        strcat(position, "a3");
        gtk_main_quit();
}
void on_a4button_clicked (GtkButton *button){
        strcat(position, "a4");
        gtk_main_quit();
}
void on_a5button_clicked (GtkButton *button){
        strcat(position, "a5");
        gtk_main_quit();
}
void on_a6button_clicked (GtkButton *button){
        strcat(position, "a6");
        gtk_main_quit();
}
void on_a7button_clicked (GtkButton *button){
        strcat(position, "a7");
        gtk_main_quit();
}
void on_a8button_clicked (GtkButton *button){
        strcat(position, "a8");
        gtk_main_quit();
}


void on_b1button_clicked (GtkButton *button){
        strcat(position, "b1");
        gtk_main_quit();
}
void on_b2button_clicked (GtkButton *button){
        strcat(position, "b2");
        gtk_main_quit();
}
void on_b3button_clicked (GtkButton *button){
        strcat(position, "b3");
        gtk_main_quit();
}
void on_b4button_clicked (GtkButton *button){
        strcat(position, "b4");
        gtk_main_quit();
}
void on_b5button_clicked (GtkButton *button){
        strcat(position, "b5");
        gtk_main_quit();
}
void on_b6button_clicked (GtkButton *button){
        strcat(position, "b6");
        gtk_main_quit();
}
void on_b7button_clicked (GtkButton *button){
        strcat(position, "b7");
        gtk_main_quit();
}
void on_b8button_clicked (GtkButton *button){
        strcat(position, "b8");
        gtk_main_quit();
}

void on_c1button_clicked (GtkButton *button){
        strcat(position, "c1");
        gtk_main_quit();
}
void on_c2button_clicked (GtkButton *button){
        strcat(position, "c2");
        gtk_main_quit();
}
void on_c3button_clicked (GtkButton *button){
        strcat(position, "c3");
        gtk_main_quit();
}
void on_c4button_clicked (GtkButton *button){
        strcat(position, "c4");
        gtk_main_quit();
}
void on_c5button_clicked (GtkButton *button){
        strcat(position, "c5");
        gtk_main_quit();
}
void on_c6button_clicked (GtkButton *button){
        strcat(position, "c6");
        gtk_main_quit();
}
void on_c7button_clicked (GtkButton *button){
        strcat(position, "c7");
        gtk_main_quit();
}
void on_c8button_clicked (GtkButton *button){
        strcat(position, "c8");
        gtk_main_quit();
}


void on_d1button_clicked (GtkButton *button){
        strcat(position, "d1");
        gtk_main_quit();
}
void on_d2button_clicked (GtkButton *button){
        strcat(position, "d2");
        gtk_main_quit();
}
void on_d3button_clicked (GtkButton *button){
        strcat(position, "d3");
        gtk_main_quit();
}
void on_d4button_clicked (GtkButton *button){
        strcat(position, "d4");
        gtk_main_quit();
}
void on_d5button_clicked (GtkButton *button){
        strcat(position, "d5");
        gtk_main_quit();
}
void on_d6button_clicked (GtkButton *button){
        strcat(position, "d6");
        gtk_main_quit();
}
void on_d7button_clicked (GtkButton *button){
        strcat(position, "d7");
        gtk_main_quit();
}
void on_d8button_clicked (GtkButton *button){
        strcat(position, "d8");
        gtk_main_quit();
}

void on_e1button_clicked (GtkButton *button){
        strcat(position, "e1");
        gtk_main_quit();
}
void on_e2button_clicked (GtkButton *button){
        strcat(position, "e2");
        gtk_main_quit();
}
void on_e3button_clicked (GtkButton *button){
        strcat(position, "e3");
        gtk_main_quit();
}
void on_e4button_clicked (GtkButton *button){
        strcat(position, "e4");
        gtk_main_quit();
}
void on_e5button_clicked (GtkButton *button){
        strcat(position, "e5");
        gtk_main_quit();
}
void on_e6button_clicked (GtkButton *button){
        strcat(position, "e6");
        gtk_main_quit();
}
void on_e7button_clicked (GtkButton *button){
        strcat(position, "e7");
        gtk_main_quit();
}
void on_e8button_clicked (GtkButton *button){
        strcat(position, "e8");
        gtk_main_quit();
}


void on_f1button_clicked (GtkButton *button){
        strcat(position, "f1");
        gtk_main_quit();
}
void on_f2button_clicked (GtkButton *button){
        strcat(position, "f2");
        gtk_main_quit();
}
void on_f3button_clicked (GtkButton *button){
        strcat(position, "f3");
        gtk_main_quit();
}
void on_f4button_clicked (GtkButton *button){
        strcat(position, "f4");
        gtk_main_quit();
}
void on_f5button_clicked (GtkButton *button){
        strcat(position, "f5");
        gtk_main_quit();
}
void on_f6button_clicked (GtkButton *button){
        strcat(position, "f6");
        gtk_main_quit();
}
void on_f7button_clicked (GtkButton *button){
        strcat(position, "f7");
        gtk_main_quit();
}
void on_f8button_clicked (GtkButton *button){
        strcat(position, "f8");
        gtk_main_quit();
}

void on_g1button_clicked (GtkButton *button){
        strcat(position, "g1");
        gtk_main_quit();
}
void on_g2button_clicked (GtkButton *button){
        strcat(position, "g2");
        gtk_main_quit();
}
void on_g3button_clicked (GtkButton *button){
        strcat(position, "g3");
        gtk_main_quit();
}
void on_g4button_clicked (GtkButton *button){
        strcat(position, "g4");
        gtk_main_quit();
}
void on_g5button_clicked (GtkButton *button){
        strcat(position, "g5");
        gtk_main_quit();
}
void on_g6button_clicked (GtkButton *button){
        strcat(position, "g6");
        gtk_main_quit();
}
void on_g7button_clicked (GtkButton *button){
        strcat(position, "g7");
        gtk_main_quit();
}
void on_g8button_clicked (GtkButton *button){
        strcat(position, "g8");
        gtk_main_quit();
}



void on_h1button_clicked (GtkButton *button){
        strcat(position, "h1");
        gtk_main_quit();
}
void on_h2button_clicked (GtkButton *button){
        strcat(position, "h2");
        gtk_main_quit();
}
void on_h3button_clicked (GtkButton *button){
        strcat(position, "h3");
        gtk_main_quit();
}
void on_h4button_clicked (GtkButton *button){
        strcat(position, "h4");
        gtk_main_quit();
}
void on_h5button_clicked (GtkButton *button){
        strcat(position, "h5");
        gtk_main_quit();
}
void on_h6button_clicked (GtkButton *button){
        strcat(position, "h6");
        gtk_main_quit();
}
void on_h7button_clicked (GtkButton *button){
        strcat(position, "h7");
        gtk_main_quit();
}
void on_h8button_clicked (GtkButton *button){
        strcat(position, "h8");
        gtk_main_quit();
}

/* EOF */

