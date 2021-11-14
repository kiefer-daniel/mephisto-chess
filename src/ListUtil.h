

//////////////////////////
/* LIST UTILITIES . H */
//////////////////////////

#ifndef LISTUTIL_H
#define LISTUTIL_H

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ServerConst.h"


ONLINE * CreateOnline(char username[], int FD);
int DeleteOnline(char username[], ONLINELIST * onlinelist);
ONLINELIST * CreateOnlinelist(void);
void DeleteOnlinelist(ONLINELIST * list);

UENTRY * CreateUEntry(char username[], char password[]);
int DeleteUEntry(char username[], ULIST * userlist);
ULIST * CreateUList(void);
void DeleteUList(ULIST * accountlist);

FENTRY * CreateFEntry(char username[]);
int DeleteFEntry(char username[], UENTRY * user);
FLIST * CreateFList(UENTRY * user);
void DeleteFList(UENTRY * user);

void AppendOnline(ONLINELIST * onlinelist, char username[], int FD);
void AppendUEntry(ULIST * accountlist, char username[], char password[]);
void AppendFEntry(char username[], UENTRY * user);

#endif /* LISTUTIL_H */

/* EOF */
