#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "util.h"

#include "confreg.h"

extern int DEBUG;

extern uci_list *listlogcheck;
extern filelist_st *files;

uci_logcheck *newData(){
	return ( uci_logcheck *)malloc( sizeof( uci_logcheck )) ;
}

uci_list *listNew(){
	uci_list *list = ( uci_list *)malloc( sizeof( uci_list )) ;
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	return list;
}

uci_logcheck *listAddlogcheck(uci_list* list, int enable, char* name, char* pattern, char* fields, int maxfail, char *script, pairlist_st *params ){
	uci_logcheck *d;
	uci_logcheck *p;
	d = ( uci_logcheck *)malloc( sizeof( uci_logcheck ));
	d->enable = enable;
	d->name = name;
	d->pattern = pattern;
	d->fields = fields;
	d->maxfail = maxfail;
	d->script = script;
	d->params = (struct pairlist_st *) params;
	d->next = NULL;
	if (list->last){
		p = list->last;
		p->next = d;
		d->prev = p;
	}
	if (list->first == NULL)
		list->first = d;
	list->last = d;
	list->count++;
	return d;
}

filelist_st *newFileList(){
	filelist_st *list = (filelist_st *) malloc( sizeof(filelist_st));
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	return list;
}

void addFile(filelist_st *list, const char *filename)
{
	file_st *d;
	file_st *p;

	int lname = strlen(filename);
	d = ( file_st *) malloc(sizeof( file_st ));
	d->name = strndup(filename,lname);
	d->lasteof = getsize(filename);
	d->next = NULL;
	if (list->last){
		p = (file_st *)list->last;
		p->next = (file_st *)d;
		d->prev = (file_st *)p;
	}
	if (list->first == NULL)
		list->first = (file_st *)d;
	list->last = (file_st *)d;
	list->count++;
	if (DEBUG)
		printf("Adding %s to file list\n", filename);
}

void *freeFileList(filelist_st *list)
{
	file_st *d = (file_st *)list->first;
	while(d){
		file_st * e = d;
		d = (file_st *)d->next;
		free(e->name);
		free(e);
	}
	free(list);
	return NULL;
}

