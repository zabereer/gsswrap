#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

struct gsswrap_context;

/**
 * 
 */
gsswrap_context* make_gsswrap_context();
void free_gsswrap_context(gsswrap_context*);

#endif
