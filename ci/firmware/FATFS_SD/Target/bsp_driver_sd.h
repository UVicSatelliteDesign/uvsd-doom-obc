/*
 * CI stand-in for the CubeMX-generated FATFS/Target/bsp_driver_sd.h (SDMMC1).
 * Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh when the .ioc enables
 * the SD Card drive and CubeMX output is missing. Regenerating with CubeMX
 * replaces it.
 */
#ifndef __STM32H7_SD_H
#define __STM32H7_SD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef

#define MSD_OK                   ((uint8_t)0x00)
#define MSD_ERROR                ((uint8_t)0x01)
#define MSD_ERROR_SD_NOT_PRESENT ((uint8_t)0x02)

#define SD_TRANSFER_OK           ((uint8_t)0x00)
#define SD_TRANSFER_BUSY         ((uint8_t)0x01)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

#define SD_DATATIMEOUT           ((uint32_t)100000000)

uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetCardState(void);
void    BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo);
uint8_t BSP_SD_IsDetected(void);

/* Implemented by sd_diskio.c */
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_ReadCpltCallback(void);

#ifdef __cplusplus
}
#endif
#endif /* __STM32H7_SD_H */
