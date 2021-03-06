#ifndef ZMAP_SEND_PFRING_H
#define ZMAP_SEND_PFRING_H

#include "../lib/includes.h"
#include <sys/ioctl.h>

#if defined(ZMAP_SEND_BSD_H) || defined(ZMAP_SEND_LINUX_H)
#error "Don't include send-bsd.h or send-linux.h with send-pfring.h"
#endif

int send_run_init(sock_t socket)
{
	(void) socket;

	// All init for pfring happens in get_socket
	return 0;
}

int send_packet(sock_t sock, void *buf, int len, uint32_t idx)
{
	sock.pf.buffers[idx]->len = len;
	memcpy(sock.pf.buffers[idx]->data, buf, len);
	int ret;
	do {
		ret = pfring_zc_send_pkt(sock.pf.queue, &sock.pf.buffers[idx], 0);
	} while (ret < 0);
	return ret;
}

void send_finish(sock_t sock) {
	pfring_zc_sync_queue(sock.pf.queue, tx_only);
}

#endif /* ZMAP_SEND_PFRING_H */
