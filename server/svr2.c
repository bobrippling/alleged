#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.h"

#define PORT 1982
#define MAX_BUF_SIZ 128

#define WAIT_MS 3000

int lsock;
struct timeval lastupdate;

long timevaltousec(struct timeval *tv)
{
	return tv->tv_sec * 1000000 + tv->tv_usec;
}

int needupdate()
{
	struct timeval now;
	long diff;

	gettimeofday(&now, NULL);

	diff = timevaltousec(&now) - timevaltousec(&lastupdate);

	return diff > (WAIT_MS * 1000);
}

int run()
{
#define INVALID_PACKET(s) printf("invalid packet: \"%s\"\n", s)
	struct sockaddr_in tmpaddr;
	socklen_t tmpaddrlen = sizeof tmpaddr;
	char buffer[MAX_BUF_SIZ];


	for(;;){
		int ret = recvfrom(lsock, buffer, MAX_BUF_SIZ, 0, (struct sockaddr *)&tmpaddr, &tmpaddrlen);
		struct client *client;

		/*puts("YO");*/

		/* assert tmpaddrlen == sizeof tmpaddr */
		if(ret != -1){
			client = lookupclient(&tmpaddr);
			if(client){
				/* expecting "POS %d %f{4}" */
				int id;
				double x, y, speed, heading;
				if(sscanf(buffer, "POS %d %lf %lf %lf %lf", &id, &x, &y, &speed, &heading) == 5){
					/* assert client->id == id */
					client->x = x;
					client->y = y;
					client->speed = speed;
					client->heading = heading;
				}else
					INVALID_PACKET(buffer);
			}else{
				/* expect "CONN %s" % name */
				char name[MAX_BUF_SIZ - 4];

				if(sscanf(buffer, "CONN %s", name) == 1){
					int id = addclient(&tmpaddr, name)->id;
					char sendbuf[16];
					snprintf(sendbuf, 16, "ACK %d", id);

					if(sendto(lsock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&tmpaddr, sizeof tmpaddr) == -1){
						perror("sendto()");
						goto bail;
					}

					printf("got client %d\n", id);
				}else
					INVALID_PACKET(buffer);
			}
		}else{
			if(errno != EWOULDBLOCK && errno != EAGAIN){
				perror("recvfrom()");
				goto bail;
			}
		}

		/* send updates every 0.5s */
		if(needupdate()){
			struct client *c, *d;

			for(c = firstclient(); c; c = c->next){
				char sendbuf[MAX_BUF_SIZ];
				int sendbuflen;

				snprintf(sendbuf, sizeof sendbuf, "%d POS %lf %lf %lf %lf", c->id, c->x, c->y, c->speed, c->heading);
				sendbuflen = strlen(sendbuf);

				printf("sending update for client %d \"%s\"...\n", c->id, sendbuf);

				for(d = firstclient(); d; d = d->next)
					if(d != c)
						if(sendto(lsock, sendbuf, sendbuflen, 0, (struct sockaddr *)&c->addr, sizeof c->addr)){
							perror("sendto()"); /* TODO: disco client only */
							goto bail;
						}else
							printf("sent to client %d\n", c->id);
			}

			gettimeofday(&lastupdate, NULL);
		}
		sleep(1); /* FIXME: poll the socket */
	}


bail:
	return 1;
}

int nonblock(int fd)
{
	int flags;

	if((flags = fcntl(fd, F_GETFL, 0)) == -1)
		flags = 0;
	flags |= O_NONBLOCK;

	if(fcntl(fd, F_SETFL, flags) == -1)
		return 1;
	return 0;
}

int setup()
{
	struct sockaddr_in addr;
	int flags;

	lsock = socket(PF_INET, SOCK_DGRAM, 0);
	if(lsock == -1){
		perror("socket()");
		return 1;
	}

	memset(&addr, '\0', sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(PORT);
	if(INADDR_ANY)
		/* any interface */
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(nonblock(lsock)){
		perror("nonblock()");
		close(lsock);
		return 1;
	}

	if(bind(lsock, (struct sockaddr *)&addr, sizeof addr) == -1){
		perror("bind()");
		close(lsock);
		return 1;
	}

	gettimeofday(&lastupdate, NULL);
	return 0;
}

int main(int argc, char **argv)
{
	if(setup())
		return 1;

	return run();
}
