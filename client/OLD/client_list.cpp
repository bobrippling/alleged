#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.h"

struct client *clients = NULL;
int nextid = 0;

struct client *addclient(struct sockaddr_in *addr, char *name)
{
	struct client *c = malloc(sizeof *c);
	if(!c){
		perror("malloc()");
		exit(1);
	}

	if(!(c->name = strdup(name))){
		perror("strdup()");
		exit(1);
	}

	c->next = clients; /* O(1) */
	clients = c;

	memset(c, '\0', sizeof *c);
	c->x = c->y = c->speed = c->heading = 0;
	memcpy(&c->addr, addr, sizeof *addr);

	c->id = nextid++;

	return c;
}

struct client *firstclient()
{
	return clients;
}

struct client *lookupclient(struct sockaddr_in *addr)
{
	struct client *c;
	for(c = clients; c; c = c->next)
		if(!memcmp(addr, &c->addr, sizeof *addr))
			return c;
	return NULL;
}

void removeclient(struct client *client)
{
	struct client *c, *prev = NULL;

	for(c = clients; c; prev = c, c = c->next)
		if(c == client){
			if(prev)
				prev->next = c->next;
			else
				clients = c->next;

			free(c->name);
			free(c);
			return;
		}

	fprintf(stderr, "client %p not found!\n",
			(void *)client);
}
