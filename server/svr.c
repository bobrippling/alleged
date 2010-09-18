#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "../config.h"

#define SLEEP_MS       100

struct client
{
	struct sockaddr_in addr;
	char *name;
} *clients = NULL;


int sock = -1, nclients = 0, verbose = 0;


int  initsock(int port);
void cleanup(void);

int  svr_newclient(struct sockaddr_in *, char *name);
int  toclientf(int, const char *, ...);
void freeclient(int);

const char *addrtostr(struct sockaddr_in *);
char       *strdup2(const char *);
int         toaddr(struct sockaddr_in *, const char *);

void sigh(int);

void svr_hup( int);
void svr_err( int);


int toclientf(int idx, const char *fmt, ...)
{
	va_list l;
	char buffer[MAX_MSG_LEN];

	va_start(l, fmt);
	vsprintf(buffer, fmt, l);
	va_end(l);

	return toaddr(&clients[idx].addr, buffer);
}

int toaddr(struct sockaddr_in *addr, const char *buffer)
{
	return sendto(sock, buffer, strlen(buffer), 0,
			(struct sockaddr *)addr, sizeof *addr);
}

const char *addrtostr(struct sockaddr_in *addr)
{
	static char buffer[32];
	return inet_ntop(AF_INET, &addr->sin_addr, buffer, sizeof buffer);
}

char *strdup2(const char *s)
{
	char *d = malloc(strlen(s) + 1);
	if(!d)
		return NULL;
	strcpy(d, s);
	return d;
}

void cleanup()
{
	int i;

	for(i = 0; i < nclients; i++)
		freeclient(i);
	free(clients);

	close(sock);
}

int svr_newclient(struct sockaddr_in *addr, char *name)
{
	const int idx = nclients++;

	clients = realloc(clients, nclients * sizeof *clients);
	if(!clients){
		perror("realloc()");
		exit(1);
	}

	memcpy(&clients[idx].addr, addr, sizeof *addr);
	clients[idx].name = name;

	toclientf(idx, "%d ACK", idx);

	return idx;
}

void svr_hup(int idx)
{
	int i;

	freeclient(idx);
	for(i = idx; i < nclients-1; i++)
		clients[i] = clients[i+1];

	clients = realloc(clients, --nclients * sizeof(*clients));
	if(!clients){
		perror("realloc shrink clients");
		exit(1);
	}
}

void freeclient(int idx)
{
	free(clients[idx].name);
}

void svr_err(int idx)
{
	/* TODO: print error */
	svr_hup(idx);
}

void sigh(int sig)
{
	if(sig == SIGINT){
		int i;
		printf("\nAlleged Server, sock status - %d clients (SIGQUIT ^\\ to quit)\n", nclients);
		if(nclients){
			puts("Index Name");
			for(i = 0; i < nclients; i++)
				printf("%3d %s\n", i, clients[i].name);
		}
		fflush(stdout);
		signal(SIGINT, &sigh); /* restore */
	}else{
		const char buffer[] = "we get signal\n";
		write(STDERR_FILENO, buffer, sizeof buffer);

		cleanup();

		exit(sig);
	}
}

int main(int argc, char **argv)
{
	struct sockaddr_in svr_addr;
	int i, port = DEFAULT_PORT, ret = 0, flags;

	/* ignore sigpipe - sent when recv() called on a disconnected socket */
	if( signal(SIGPIPE, SIG_IGN) == SIG_ERR ||
			/* usual suspects... */
			signal(SIGINT,  &sigh)   == SIG_ERR ||
			signal(SIGQUIT, &sigh)   == SIG_ERR ||
			signal(SIGTERM, &sigh)   == SIG_ERR ||
			signal(SIGSEGV, &sigh)   == SIG_ERR ||
			signal(SIGHUP,  &sigh)   == SIG_ERR){
		perror("signal()");
		return 1;
	}

	for(i = 1; i < argc; i++)
		if(!strcmp(argv[i], "-p")){
			if(++i < argc)
				port = atoi(argv[i]);
			else{
				fputs("need port\n", stderr);
				return 1;
			}
		}else if(!strcmp(argv[i], "-v"))
			verbose = 1;
		else
			goto usage;

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1){
		perror("socket()");
		return 1;
	}

	flags = fcntl(F_GETFL, 0);
	if(fcntl(F_SETFL, flags | O_NONBLOCK)){
		perror("fcntl O_NONBLOCK");
		close(sock);
		return 1;
	}

	memset(&svr_addr, '\0', sizeof svr_addr);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(port);

	if(INADDR_ANY) /* usually optimised out of existence */
		svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr *)&svr_addr, sizeof svr_addr) == -1){
		perror("bind()");
		close(sock);
		return 1;
	}

	for(;;){
		/* recv vars */
		struct sockaddr_in addr;
		socklen_t len = sizeof addr;
		ssize_t ret;
		char buffer[MAX_MSG_LEN];
		/* sscanf vars */
		int id;
		double x, y, speed, heading;

		ret = recvfrom(sock, buffer, sizeof buffer, 0,
				(struct sockaddr *)&addr, &len);

		if(ret == -1)
			if(errno != EWOULDBLOCK && errno != EAGAIN){
				perror("recfrom()");
				ret = 1;
				goto finito;
			}else
				goto timeout;

		buffer[ret-1] = '\0';

		/* got ret bytes */
		if(sscanf(buffer, "%d POS %lf %lf %lf %lf",
					&id, &x, &y, &speed, &heading)){

		}else if(!strncmp(buffer, "CONN ", 5)){
			char *name = buffer + 5;
			if(*name == '\0')
				fprintf(stderr, "invalid name\n");
			else{
				int idx, ok = 1;

				for(idx = 0; idx < nclients; idx++)
					if(!strcmp(name, clients[idx].name)){
						toaddr(&addr, "NACK name taken");
						ok = 0;
						break;
					}

				if(ok){
					/* new client */
					name = strdup2(name);
					if(!name){
						fprintf(stderr, "couldn't strdup %ld bytes\n", strlen(buffer+5));
						exit(1);
					}

					idx = svr_newclient(&addr, name);
					printf("new client: #%d, %s at %s\n", idx,
							clients[idx].name, addrtostr(&clients[idx].addr));
				}
			}
		}else{
			fprintf(stderr, "unknown message: \"%s\"\n", buffer);
		}

timeout:
		/* TODO: physics */
	}
	/* UNREACHABLE */

finito:
	cleanup();
	return ret;

usage:
	fprintf(stderr, "Usage: %s [-p port] [-v]\n"
	                " -p: Port to listen on\n"
	                " -v: Verbose\n", *argv);
	return 1;
}
