#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

struct gsswrap_context;

/**
 * 
 */
struct gsswrap_context* gsswrap_make_context();
void gsswrap_free_context(struct gsswrap_context*);

/**
 * Returns last error string, NULL if none.
 * Ownership of the string remains with gsswrap, do not free it.
 */
const char* gsswrap_last_error(struct gsswrap_context*);

#endif
