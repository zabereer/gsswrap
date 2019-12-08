#include "glue.h"

bool send_to_peer_cb(const void* buffer, size_t len, void* user_data)
{
	return true;
}

bool recv_from_peer_cb(void** buffer, size_t* len, void* user_data)
{
	return true;
}

void free_buffer_cb(void* buffer, size_t len, void* user_data)
{
}

struct gsswrap_context* glue_make_context()
{
	return gsswrap_make_context(
		send_to_peer_cb, recv_from_peer_cb, free_buffer_cb);
}
