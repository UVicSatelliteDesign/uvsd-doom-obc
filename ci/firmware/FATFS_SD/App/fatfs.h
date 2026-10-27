/*
 * CI stand-in for the CubeMX-generated FATFS/App/fatfs.h ("SD Card" on SDMMC1
 * plus "User-defined" disk). Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh
 * when the .ioc enables the SD Card drive and CubeMX output is missing.
 * Regenerating with CubeMX replaces it.
 */
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "user_diskio.h"

extern uint8_t retSD;      /* Return value for SD */
extern char SDPath[4];     /* SD logical drive path */
extern FATFS SDFatFS;      /* File system object for SD logical drive */
extern FIL SDFile;         /* File object for SD */

extern uint8_t retUSER;    /* Return value for USER */
extern char USERPath[4];   /* USER logical drive path */
extern FATFS USERFatFS;    /* File system object for USER logical drive */
extern FIL USERFile;       /* File object for USER */

void MX_FATFS_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* __fatfs_H */
