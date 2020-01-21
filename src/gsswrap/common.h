#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

/**
 * Functions common (mostly) to both initiator (client) and acceptor
 * (server) side.
 */

#include "callbacks.h"

#include <stdbool.h>
#include <stddef.h>

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

/**
 * Encrypt or decrypt a buffer using the established security context.
 * Ownership of the output_buffer remains with gsswrap, do not free it.
 * Only call this function if both gsswrap_confidentiality_available() and
 * gsswrap_integrity_available() returns true.
 * Returns true if success, false if not after which none of output_buffer
 * nor output_length is updated.
 * Use gsswrap_last_context_error() to obtain the failure reason.
 */
bool gsswrap_encrypt(struct gsswrap_context*,
                     const void* buffer,
                     const size_t length,
                     void** output_buffer,
                     size_t* output_length);
bool gsswrap_decrypt(struct gsswrap_context*,
                     const void* buffer,
                     const size_t length,
                     void** output_buffer,
                     size_t* output_length);

/**
 * Encrypt and send the buffer using the stored send_func in the context.
 * Only call this function if both gsswrap_confidentiality_available() and
 * gsswrap_integrity_available() returns true.
 * Returns true if buffer was successfully encrypted and send_func succeeded.
 * Use gsswrap_last_context_error() to obtain the failure reason.
 */
bool gsswrap_encrypt_send(struct gsswrap_context*,
                          const void* buffer,
                          size_t length,
                          void* user_data);

/**
 * Receive a buffer via recv_func stored in the context, then decrypt it and
 * update output_buffer and output_length. The buffer input buffer obtained
 * via the recv_func will be freed by free_func stored in the context.
 * Ownership of the output_buffer remains with gsswrap, do not free it.
 * Only call this function if both gsswrap_confidentiality_available() and
 * gsswrap_integrity_available() returns true.
 * Returns true if a success, false if not after which none of output_buffer
 * nor output_length is updated.
 * Use gsswrap_last_context_error() to obtain the failure reason.
 */
bool gsswrap_recv_decrypt(struct gsswrap_context*,
                          void** output_buffer,
                          size_t* output_length,
                          void* user_data);

#endif
