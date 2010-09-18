#include <sys/socket.h>
#include <string.h>

static int sock_listen = -1;

int net_init()
{
	struct sockaddr_in addr;

	sock_listen = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock_listen == -1){
		perror("socket()");
		return 1;
	}

	memset(addr, '\0', sizeof addr);

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(LISTEN_PORT);

	if(INADDR_ANY)
		/* any interface */
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock_listen, (struct sockaddr *)&addr, sizeof addr) == -1){
		perror("bind()");
		return 1;
	}

	return 0;
}

void net_term()
{
	shutdown(sock_listen, SHUT_RDWR);
	close(sock_listen);
}
