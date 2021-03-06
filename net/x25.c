#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/x25.h>
#include <stdlib.h>
#include "maps.h"	// page_rand

void gen_x25(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_x25 *x25;
	unsigned int len;

	x25 = malloc(sizeof(struct sockaddr_x25));
	if (x25 == NULL)
		return;

	x25->sx25_family = PF_X25;
	len = rand() % 15;
	memset(&page_rand[len], 0, 1);
	strncpy(x25->sx25_addr.x25_addr, page_rand, len);
	*addr = (unsigned long) x25;
	*addrlen = sizeof(struct sockaddr_x25);
}
