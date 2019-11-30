#ifndef GSSWRAP_CALLBACKS_H
#define GSSWRAP_CALLBACKS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * 
 */
typedef bool (*gsswrap_send_token_fn)(
    const void* buffer, size_t len, void* user_data);
typedef bool (*gsswrap_recv_token_fn)(
    void** buffer, size_t* len, void* user_data);

#endif
