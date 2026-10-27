/*
 * CI stand-in for the CubeMX-generated FATFS/App/fatfs.c ("User-defined" disk).
 * Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh only when CubeMX output
 * is missing. Regenerating with CubeMX replaces it.
 */
#include "fatfs.h"

uint8_t retUSER;
char USERPath[4];
FATFS USERFatFS;
FIL USERFile;

void MX_FATFS_Init(void)
{
  /* Link the USER disk I/O driver */
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);
}

/* No RTC on the OBC yet: report a fixed timestamp. */
DWORD get_fattime(void)
{
  return 0;
}
