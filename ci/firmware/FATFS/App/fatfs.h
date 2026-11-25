/*
 * CI stand-in for the CubeMX-generated FATFS/App/fatfs.h ("User-defined" disk).
 * Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh only when CubeMX output
 * is missing. Regenerating with CubeMX replaces it.
 */
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "user_diskio.h"

extern uint8_t retUSER;    /* Return value for USER */
extern char USERPath[4];   /* USER logical drive path */
extern FATFS USERFatFS;    /* File system object for USER logical drive */
extern FIL USERFile;       /* File object for USER */

void MX_FATFS_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* __fatfs_H */
