#ifndef CLIENT_H
#define CLIENT_H

struct client
{
	int id; /* also client ID */
	struct sockaddr_in addr;
	char *name;

	float x, y, speed, heading;

	struct client *next;
};


struct client *addclient(
		struct sockaddr_in *addr, char *name);

struct client *lookupclient(
		struct sockaddr_in *addr);

struct client *firstclient();

void removeclient(struct client *client);

#endif
