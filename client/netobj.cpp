#include <string.h>
#include <arpa/inet.h>

#include "netobj.h"

NetObj::NetObj(struct sockaddr_in *addr)
	: addr()
{
	if(addr)
		memcpy(&this->addr, addr, sizeof this->addr);
	else
		memset(&this->addr, '\0', sizeof this->addr);
}
