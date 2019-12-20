#include "glue.h"

#include <_cgo_export.h>

#include <stdlib.h>

bool send_to_peer_cb(const void* buffer, size_t length, void* user_data)
{
	verifyUserData(user_data);
	return sendToPeer(length, (void*)buffer);
}

bool recv_from_peer_cb(void** buffer, size_t* length, void* user_data)
{
	verifyUserData(user_data);
	struct recvFromPeer_return r = recvFromPeer();
	*length = r.r0;
	*buffer = r.r1;
	return true;
}

void free_buffer_cb(void* buffer, size_t len, void* user_data)
{
	verifyUserData(user_data);
	free(buffer); // was obtained in recvFromPeer()'s C.CBytes()
}

struct gsswrap_context* glue_make_context()
{
	return gsswrap_make_context(
		send_to_peer_cb, recv_from_peer_cb, free_buffer_cb);
}
