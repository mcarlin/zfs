/*
 * The libzfs_api exists as a common interface layer for ZFS actions
 * providing a safe and stable API layer. It utilizes libzfs as a means to
 * initiate these actions to the kernel
 */

#ifndef _LIBZFS_API_H
#define _LIBZFS_API_H

#include <libzfs.h>

#ifdef	__cplusplus
extern "C" {
#endif

int libzfs_api_init(void);
void libzfs_api_fini(void);

zfs_error_t lza_pool_destroy(const char *, const char *, boolean_t force);

#ifdef	__cplusplus
}
#endif

#endif /* _LIBZFS_API_H */