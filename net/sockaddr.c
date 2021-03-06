#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <stdlib.h>
#include "sanitise.h"
#include "compat.h"
#include "net.h"
#include "maps.h"
#include "config.h"
#include "params.h"	// do_specific_proto

void generate_sockaddr(unsigned long *addr, unsigned long *addrlen, int pf)
{
	/* If we want sockets of a specific type, we'll want sockaddrs that match. */
	if (do_specific_proto == TRUE)
		pf = specific_proto;

	/* If we got no hint passed down, pick a random proto. */
	if (pf == -1)
		pf = rand() % TRINITY_PF_MAX;

	switch (pf) {

	case PF_UNSPEC:
		//TODO
		break;

	case PF_UNIX:
		gen_unixsock(addr, addrlen);
		break;

	case PF_INET:
		gen_ipv4(addr, addrlen);
		break;

	case PF_AX25:
		gen_ax25(addr, addrlen);
		break;

	case PF_IPX:
		gen_ipx(addr, addrlen);
		break;

	case PF_APPLETALK:
		gen_appletalk(addr, addrlen);
		break;

	case PF_NETROM:
		//TODO
		break;

	case PF_BRIDGE:
		//TODO
		break;

	case PF_ATMPVC:
		gen_atmpvc(addr, addrlen);
		break;

	case PF_X25:
		gen_x25(addr, addrlen);
		break;

	case PF_INET6:
		gen_ipv6(addr, addrlen);
		break;

	case PF_ROSE:
		gen_rose(addr, addrlen);
		break;

	case PF_DECnet:
		gen_decnet(addr, addrlen);
		break;

	case PF_NETBEUI:
		gen_llc(addr, addrlen);
		break;

	case PF_SECURITY:
		//TODO
		break;

	case PF_KEY:
		break;

	case PF_NETLINK:
		gen_netlink(addr, addrlen);
		break;

	case PF_PACKET:
		gen_packet(addr, addrlen);
		break;

	case PF_ASH:
		//TODO
		break;

	case PF_ECONET:
		gen_econet(addr, addrlen);
		break;

	case PF_ATMSVC:
		gen_atmsvc(addr, addrlen);
		break;

	case PF_RDS:
		//TODO
		break;

	case PF_SNA:
		//TODO
		break;

	case PF_IRDA:
		gen_irda(addr, addrlen);
		break;

	case PF_PPPOX:
		gen_pppox(addr, addrlen);
		break;

	case PF_WANPIPE:
		//TODO
		break;

	case PF_LLC:
		gen_llc(addr, addrlen);
		break;

	case PF_CAN:
		gen_can(addr, addrlen);
		break;

	case PF_TIPC:
		gen_tipc(addr, addrlen);
		break;

	case PF_BLUETOOTH:
		//TODO
		break;

	case PF_IUCV:
		//TODO
		break;

	case PF_RXRPC:
		//TODO
		break;

	case PF_ISDN:
		//TODO
		break;

	case PF_PHONET:
		gen_phonet(addr, addrlen);
		break;

	case PF_IEEE802154:
		//TODO
		break;

#ifdef USE_CAIF
	case PF_CAIF:
		gen_caif(addr, addrlen);
		break;
#endif

#ifdef USE_IF_ALG
	case PF_ALG:
		gen_alg(addr, addrlen);
		break;
#endif

	case PF_NFC:
		gen_nfc(addr, addrlen);
		break;

	case PF_VSOCK:
		//TODO
		break;

	default:
		break;
	}
}
