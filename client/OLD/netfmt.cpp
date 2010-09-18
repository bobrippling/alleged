#include <sys/socket.h>

#include "ball.h"
#include "netfmt.h"


int net_formatpacket( struct packet & p, class Ball & b)
{
	p.type = BALL_POS;
	p.x = b.x();
	p.y = b.y();
}

int net_recievepacket(struct packet & p, int socket)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof addr;
	size_t packetlen = sizeof p;
	int ret;

	switch(ret = recvfrom(socket, p, packetlen, MSG_PEEK, &addr, &addrlen)){
		case 0:
			/* conn closed */
			break;

		case -1:
			/* error */

		default:
			/* got %d=ret bytes */
	}
}
