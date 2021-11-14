

//////////////////////////
/* LIST UTILITIES . C */
//////////////////////////

#include "ListUtil.h"


ONLINE * CreateOnline(char username[], int FD) {
	ONLINE * online = malloc(sizeof(ONLINE));	
	if(!online) {
		perror("Could not allocate online. Out of memory.\n");
		exit(10);
	} /* fi */
	strcpy(online->name, username);
	online->FD = FD;
	online->list = NULL;
	online->next = NULL;
	online->prev = NULL;

	return online;
}

int DeleteOnline(char username[], ONLINELIST * onlinelist) {
	char nameSelect[21];
	strcpy(nameSelect, onlinelist->first->name);
	ONLINE * select = onlinelist->first;

	for(int i = 0; i < onlinelist->length; i++) {
		if(0 == strcmp(username, nameSelect)) {
			// case for if online is first in ONLINELIST
			if (i == 0) {
				select->list->first = select->list->first->next;
				select->list->first->prev = NULL;
			} /* fi esle */
			// case for if online is last in ONLINELIST
			else if (i == select->list->length - 1) {
				select->list->last = select->list->last->prev;
				select->list->last->next = NULL;
			} /* fi esle */
			// case for if online is not first or last in ONLINELIST
			else {
				select->prev->next = select->next;
				select->next->prev = select->prev;
			} /* esle */
			select->next = NULL;
			select->prev = NULL;
			free(select);
			onlinelist->length--;
			select = NULL;
			return 1;
		} /* fi */
		else {
			select = select->next;
			strcpy(nameSelect, select->name);
		} /* esle */
	} /* rof */

	return 0;
}

ONLINELIST * CreateOnlinelist(void) {
	ONLINELIST * onlinelist = malloc(sizeof(ONLINELIST));	
	assert(onlinelist);
	if(!onlinelist) {
		perror("Could not allocate onlinelist. Out of memory.\n");
		exit(10);
	} /* fi */
	onlinelist->length = 0;
	onlinelist->first = NULL;
	onlinelist->last = NULL;

	return onlinelist;
}

void DeleteOnlinelist(ONLINELIST * list) {
	assert(list);
	ONLINE * online;
	ONLINE * select;

	online = list->last;
	while(online) {
		select = online->prev;
		DeleteOnline(online->name, list);
		online = select;
	} /* elihw */

	free(list);
	online = NULL;
	select = NULL;
	return;
}

UENTRY * CreateUEntry(char username[], char password[]) {
	UENTRY * account = malloc(sizeof(UENTRY));
	CreateFList(account);
	if(!account || !account->friendlist) {
		perror("Could not allocate account. Out of memory.\n");
		exit(10);
	} /* fi */

	account->list = NULL;
	account->next = NULL;
	account->prev = NULL;
	strcpy(account->username, username);
	strcpy(account->password, password);

	// clear buffer
	for(int i=0; i<sizeof(account->buffer); i++) {
		account->buffer[i] = ' ';
	} /* rof */
	account->buffer[0] = '\0';

	return account;
}

int DeleteUEntry(char username[], ULIST * userlist) {
	char nameSelect[21];
	strcpy(nameSelect, userlist->first->username);
	UENTRY * select = userlist->first;

	for(int i = 0; i < userlist->length; i++) {
		if(0 == strcmp(username, nameSelect)) {
			if(select->friendlist) {
				DeleteFList(select);
			} /* fi */
			// case for if account is first in ULIST
			if (i == 0) {
				select->list->first = select->list->first->next;
				select->list->first->prev = NULL;
			} /* fi esle */
			// case for if account is last in ULIST
			else if (i == select->list->length - 1) {
				select->list->last = select->list->last->prev;
				select->list->last->next = NULL;
			} /* fi esle */
			// case for if account is not first or last in ULIST
			else {
				select->prev->next = select->next;
				select->next->prev = select->prev;
			} /* esle */
			select->next = NULL;
			select->prev = NULL;
			free(select);
			userlist->length--;
			select = NULL;
			return 1;
		} /* fi */
		else {
			select = select->next;
			strcpy(nameSelect, select->username);
		} /* esle */
	} /* rof */

	return 0;
}

ULIST * CreateUList(void) {
	ULIST * userlist = (ULIST * ) malloc(sizeof(ULIST));
	if (!userlist) {
		perror("Could not allocate userlist. Out of memory.\n");
		exit(10);
	} /* fi */
	userlist->first = NULL;
	userlist->last = NULL;
	userlist->length = 0;

	return userlist;
}

void DeleteUList(ULIST * accountlist) {
	assert(accountlist);
	UENTRY * account;
	UENTRY * select;

	account = accountlist->last;
	while(account) {
		select = account->prev;
		DeleteUEntry(account->username, accountlist);
		account = select;
	} /* elihw */

	free(accountlist);
	account = NULL;
	select = NULL;
	return;
}

FENTRY * CreateFEntry(char username[]) {
	FENTRY * friend = (FENTRY *) malloc(sizeof(FENTRY));
	if(!friend) {
		perror("Could not allocate friend. Out of memory.\n");
		exit(10);
	} /* fi */
	friend->list = NULL;
	friend->next = NULL;
	friend->prev = NULL;
	strcpy(friend->username, username);

	return friend;
}

int DeleteFEntry( char username[], UENTRY * user) {
	char * friendName = user->friendlist->first->username;
	FENTRY * select = user->friendlist->first;

	for (int i = 0; i < user->list->length; i++ ){
		if ( 0 == strcmp(username, friendName)) {
			if( i == 0 ) {
				user->friendlist->first = user->friendlist->first->next;
				user->friendlist->first->prev = NULL;
			} /* fi */
			else if (i == user->list->length - 1) {
				user->friendlist->last = user->friendlist->last->prev;
				user->friendlist->last->next = NULL;
			} /* fi esle */
			else {
				select->prev->next = select->next;
				select->next->prev = select->prev;
			} /* esle */
			select->next = NULL;
			select->prev = NULL;
			free(select);
			user->friendlist->length--;
			select = NULL;
			return 1;
		} /* fi */
		else {
			select = select->next;
			strcpy(friendName, select->username);	
		} /* esle */
	} /* rof */
	return 0;
}

FLIST * CreateFList(UENTRY * user) {
	user->friendlist = malloc(sizeof(FLIST));
	if(!user->friendlist) {
		perror("Could not allocate friendlist. Out of memory.\n");
		exit(10);
	} /* fi */
	user->friendlist->first = NULL;
	user->friendlist->last = NULL;
	user->friendlist->length = 0;

	return user->friendlist;
}

void DeleteFList(UENTRY * user) {
	assert(user->friendlist);
	FENTRY * friend;
	FENTRY * select;

	friend = user->friendlist->last;
	while(friend) {
		select = friend->prev;
		DeleteFEntry(friend->username, user);
		friend = select;
	} /* elihw */

	free(user->friendlist);
	friend = NULL;
	select = NULL;
	return;
}

void AppendOnline(ONLINELIST * onlinelist, char username[], int FD) {
	assert(onlinelist);
	ONLINE * online;
	
	online = CreateOnline(username, FD);
	
	if(onlinelist->last) {
		online->list = onlinelist;
		online->next = NULL;
		online->prev = onlinelist->last;
		onlinelist->last->next = online;
		onlinelist->last = online;
	} /* fi */
	else {
		online->list = onlinelist;
		online->next = NULL;
		online->prev = NULL;
		onlinelist->first = online;
		onlinelist->last = online;
	} /* esle */

	onlinelist->length++;
	return;
}

void AppendUEntry(ULIST * accountlist, char username[], char password[]) {
	assert(accountlist);
	UENTRY * account;
	FLIST * friendlist;
	
	account = CreateUEntry(username, password);
	friendlist = CreateFList(account);
	account->friendlist = friendlist;
	
	if(accountlist->last) {
		account->list = accountlist;
		account->next = NULL;
		account->prev = accountlist->last;
		accountlist->last->next = account;
		accountlist->last = account;
	} /* fi */
	else {
		account->list = accountlist;
		account->next = NULL;
		account->prev = NULL;
		accountlist->first = account;
		accountlist->last = account;
	} /* esle */

	accountlist->length++;
	return;
}

void AppendFEntry(char username[], UENTRY * user) {
	assert(username);
	assert(user);
	FENTRY * friend;
	
	friend = CreateFEntry(username);
	
	if(user->friendlist->last) {
		friend->list = user->friendlist;
		friend->next = NULL;
		friend->prev = user->friendlist->last;
		user->friendlist->last->next = friend;
		user->friendlist->last = friend;
	} /* fi */
	else {
		friend->list = user->friendlist;
		friend->next = NULL;
		friend->prev = NULL;
		user->friendlist->first = friend;
		user->friendlist->last = friend;
	} /* esle */

	user->friendlist->length++;	
	return;
}

/* EOF */
