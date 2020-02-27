#include <libzfs_api.h>
#include <libzfs_core.h>
#include <libzfs_impl.h>
#include <libzfs.h>
#include <sys/fs/zfs.h>
#include <errno.h>

static libzfs_handle_t *g_api_zfs = NULL;

int
libzfs_api_init(void)
{
	if (g_api_zfs == NULL && (g_api_zfs = libzfs_init()) == NULL) {
		return (errno);
	}

	return (0);
}

void
libzfs_api_fini(void)
{
	if (g_api_zfs != NULL) {
		libzfs_fini(g_api_zfs);
		g_api_zfs = NULL;
	}
}

/**
 * Destroy the given pool. Will first take care of unmounting all datasets from the 
 * pool so that it's destroyable.
 *
 * If an error is encountered in the process, one of the following will be returned:
 * - EZFS_OPENFAILED: Failed to open pool directly or as dataset
 * - EZFS_UMOUNTFAILED: Failed to unmount all datasets
 * - EZFS_POOL_READONLY: One or more devices in the pool are read only
 * - EZFS_NOENT: Pool doesn't exist
 * - EZFS_EXPORT_IN_PROGRESS: Pool export currently in progress
 * - EZFS_BUSY: Pool has active references
 * - EZFS_ACTIVE_SPARE: Pool has an actively shared spare
 * - EZFS_UNKNOWN: An unknown error has occurred
 */
zfs_error_t
lza_pool_destroy(const char *pool, const char *log_history, boolean_t force)
{
	zfs_error_t ret = EZFS_SUCCESS;
	zpool_handle_t *zhp = NULL;

	zhp = zpool_open(g_api_zfs, pool);
	if (zhp == NULL)
		return (EZFS_OPENFAILED);

	if (zpool_disable_datasets(zhp, force) != 0) {
		ret = EZFS_UMOUNTFAILED;
		goto out;
	}

	ret = zpool_destroy(zhp, log_history);

out:
	if (zhp) {
		zpool_close(zhp);
	}
	return (ret);
}