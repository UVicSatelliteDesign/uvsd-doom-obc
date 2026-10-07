/*
 * CI stand-in for the CubeMX-generated FATFS/Target/user_diskio.c.
 * Every call reports "not ready" until a real SD card driver is written.
 * Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh only when CubeMX output
 * is missing. Regenerating with CubeMX replaces it.
 */
#include <string.h>
#include "ff_gen_drv.h"

static volatile DSTATUS Stat = STA_NOINIT;

static DSTATUS USER_initialize(BYTE pdrv)
{
  (void)pdrv;
  Stat = STA_NOINIT;
  return Stat;
}

static DSTATUS USER_status(BYTE pdrv)
{
  (void)pdrv;
  return Stat;
}

static DRESULT USER_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
  (void)pdrv; (void)buff; (void)sector; (void)count;
  return RES_NOTRDY;
}

#if _USE_WRITE == 1
static DRESULT USER_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
  (void)pdrv; (void)buff; (void)sector; (void)count;
  return RES_NOTRDY;
}
#endif

#if _USE_IOCTL == 1
static DRESULT USER_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
  (void)pdrv; (void)cmd; (void)buff;
  return RES_NOTRDY;
}
#endif

Diskio_drvTypeDef USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if _USE_WRITE == 1
  USER_write,
#endif
#if _USE_IOCTL == 1
  USER_ioctl,
#endif
};
