/*
 * CI stand-in for the CubeMX-generated FATFS/Target/user_diskio.h.
 * Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh only when CubeMX output
 * is missing. Regenerating with CubeMX replaces it.
 */
#ifndef __USER_DISKIO_H
#define __USER_DISKIO_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ff_gen_drv.h"

extern Diskio_drvTypeDef USER_Driver;

#ifdef __cplusplus
}
#endif
#endif /* __USER_DISKIO_H */
