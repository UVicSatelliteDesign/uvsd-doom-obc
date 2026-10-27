/*
 * CI stand-in for the CubeMX-generated FATFS/App/fatfs.c ("SD Card" on SDMMC1
 * plus "User-defined" disk). Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh
 * when the .ioc enables the SD Card drive and CubeMX output is missing.
 * Regenerating with CubeMX replaces it.
 */
#include "fatfs.h"

uint8_t retSD;
char SDPath[4];
FATFS SDFatFS;
FIL SDFile;

uint8_t retUSER;
char USERPath[4];
FATFS USERFatFS;
FIL USERFile;

void MX_FATFS_Init(void)
{
  /* Link the SD and USER disk I/O drivers, in the order CubeMX uses */
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);
}

/* No RTC on the OBC yet: report a fixed timestamp. */
DWORD get_fattime(void)
{
  return 0;
}
