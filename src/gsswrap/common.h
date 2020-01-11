#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

/**
 * Functions common (mostly) to both initiator (client) and acceptor
 * (server) side.
 */

#include "callbacks.h"

struct gsswrap_credential;
struct gsswrap_context;

/**
 * Create a credential for use by either the initiator (client) or
 * acceptor (server). This credential *must* be destroyed again with
 * gsswrap_destroy_credential() when it is not needed anymore.
 */
struct gsswrap_credential* gsswrap_make_credential();

/**
 * Destroy gsswrap_credential freeing all associated resources.
 */
void gsswrap_destroy_credential(struct gsswrap_credential*);

/**
 * Create gsswrap_context for initiating (client) or accepting (server).
 * The function pointers are used by gsswrap_initiate() and gsswrap_accept()
 * when establishing a security context.
 * The send_function and recv_function is used to send and receive tokens
 * to/from the peer and free_function is used to free received tokens.
 * This context *must* be destroyed again with
 * gsswrap_destroy_context() when it is not needed anymore.
 */
struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function,
    gsswrap_free_buffer_fn free_function);

/**
 * Destroy the gsswrap_context freeing all associated resources.
 */
void gsswrap_destroy_context(struct gsswrap_context*);

/**
 * Returns last error string associated with a credential or context,
 * or NULL if none.
 * Ownership of the returned string remains with gsswrap, do not free it.
 */
const char* gsswrap_last_credential_error(struct gsswrap_credential*);
const char* gsswrap_last_context_error(struct gsswrap_context*);

/**
 * For initiator (client) side use only. These set the request flags for
 * the initiator. Its use is optional, the initiator will call
 * gsswrap_set_all_flags() if none are set.
 */
void gsswrap_set_confidentiality_available(struct gsswrap_context*);
void gsswrap_set_delegated(struct gsswrap_context*);
void gsswrap_set_integrity_available(struct gsswrap_context*);
void gsswrap_set_mutual_auth(struct gsswrap_context*);
void gsswrap_set_out_of_sequence_detection(struct gsswrap_context*);
void gsswrap_set_replay_detection(struct gsswrap_context*);
void gsswrap_set_all_flags(struct gsswrap_context*);
void gsswrap_clear_all_flags(struct gsswrap_context*);

/**
 * For both initiator (client) and acceptor (server) use.
 * These retrieve the active flags once a security context has been
 * established between them.
 */
bool gsswrap_confidentiality_available(const struct gsswrap_context*);
bool gsswrap_delegated(const struct gsswrap_context*);
bool gsswrap_integrity_available(const struct gsswrap_context*);
bool gsswrap_mutual_auth(const struct gsswrap_context*);
bool gsswrap_out_of_sequence_detection(const struct gsswrap_context*);
bool gsswrap_replay_detection(const struct gsswrap_context*);

#endif
