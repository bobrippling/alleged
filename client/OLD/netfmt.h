#ifndef NETFMT_H
#define NETFMT_H

struct packet
{
	enum
	{
		BALL_POS
	} type;

	double x, y;
};

int net_formatpacket( struct packet &, class Ball &);
int net_recievepacket(struct packet &, int socket);

#endif
