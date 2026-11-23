/*
 * CI stand-in for the CubeMX-generated FATFS/Target/bsp_driver_sd.c: the
 * BSP-style SD API that sd_diskio.c expects, on the HAL SD driver and the
 * hsd1 handle from main.c. Copied into FATFS/ by ci/firmware/fetch_cube_deps.sh
 * when the .ioc enables the SD Card drive and CubeMX output is missing.
 * Regenerating with CubeMX replaces it.
 */
#include "bsp_driver_sd.h"

extern SD_HandleTypeDef hsd1;

uint8_t BSP_SD_Init(void)
{
  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR_SD_NOT_PRESENT;
  }
  return (HAL_SD_Init(&hsd1) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  return (HAL_SD_ReadBlocks(&hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  return (HAL_SD_WriteBlocks(&hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
  return (HAL_SD_ReadBlocks_DMA(&hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
  return (HAL_SD_WriteBlocks_DMA(&hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  return (HAL_SD_Erase(&hsd1, StartAddr, EndAddr) == HAL_OK) ? MSD_OK : MSD_ERROR;
}

uint8_t BSP_SD_GetCardState(void)
{
  return (HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY;
}

void BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo)
{
  HAL_SD_GetCardInfo(&hsd1, CardInfo);
}

/* No card-detect pin is configured in the .ioc, so assume a card is fitted. */
uint8_t BSP_SD_IsDetected(void)
{
  return SD_PRESENT;
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  (void)hsd;
  BSP_SD_WriteCpltCallback();
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  (void)hsd;
  BSP_SD_ReadCpltCallback();
}
