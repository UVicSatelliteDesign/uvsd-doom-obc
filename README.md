# UVSD DOOM OBC : SD functionality
## Notes:
+ This implementation does not account for D-Caching, which of the time of writing is turned off.
+ The testing of this implementation was done with a modified driver (FATFS/Target/sd_diskio.c) in order to make card detection always pass.
+ The .ioc file was changed in order to use the HSE & LSE of the board, and also changed the clock line configuration of SDMMC1 approximately 56.7MHz.
+ The FreeRTOS configuration file was changed by enabling the StackOverflowHook for detection.

## Further Implementation Details Required:
+ The naming convention for directories and files on the SD card.
+ Passing the SD buffer contents to the OBC in order to pass over the WAD file.
