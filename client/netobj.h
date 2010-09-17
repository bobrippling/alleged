#ifndef NETOBJ_H
#define NETOBJ_H

class NetObj
{
	private:
		struct sockaddr_in addr;
		int id;

	public:
		NetObj(struct sockaddr_in *addr);
};

#endif
