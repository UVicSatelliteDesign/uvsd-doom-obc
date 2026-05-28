/**
 * @file  fatfs.h
 * @brief Minimal FatFs mock for host-based unit tests.
 */

#ifndef __FATFS_H
#define __FATFS_H

#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint32_t UINT;

typedef enum {
    FR_OK = 0,
    FR_DISK_ERR,
    FR_INT_ERR,
    FR_NOT_READY,
    FR_NO_FILE,
    FR_NO_PATH,
    FR_INVALID_NAME,
    FR_DENIED,
    FR_EXIST,
    FR_INVALID_OBJECT,
    FR_WRITE_PROTECTED,
    FR_INVALID_DRIVE,
    FR_NOT_ENABLED,
    FR_NO_FILESYSTEM,
    FR_MKFS_ABORTED,
    FR_TIMEOUT,
    FR_LOCKED,
    FR_NOT_ENOUGH_CORE,
    FR_TOO_MANY_OPEN_FILES,
    FR_INVALID_PARAMETER
} FRESULT;

typedef struct { uint8_t dummy[44]; } FATFS;
typedef struct { uint8_t dummy[36]; } FIL;
typedef struct { uint8_t dummy[44]; } DIR;
typedef struct { uint8_t dummy[24]; } FILINFO;

static inline void     MX_FATFS_Init(void)                                     {}
static inline FRESULT  f_mount(FATFS *fs, const char *path, BYTE opt)          { (void)fs; (void)path; (void)opt; return FR_OK; }
static inline FRESULT  f_open(FIL *fp, const char *path, BYTE mode)            { (void)fp; (void)path; (void)mode; return FR_OK; }
static inline FRESULT  f_close(FIL *fp)                                         { (void)fp; return FR_OK; }
static inline FRESULT  f_read(FIL *fp, void *buf, UINT btr, UINT *br)          { (void)fp; (void)buf; (void)btr; if(br) *br=0; return FR_OK; }
static inline FRESULT  f_write(FIL *fp, const void *buf, UINT btw, UINT *bw)   { (void)fp; (void)buf; (void)btw; if(bw) *bw=0; return FR_OK; }
static inline FRESULT  f_sync(FIL *fp)                                          { (void)fp; return FR_OK; }
static inline FRESULT  f_lseek(FIL *fp, DWORD ofs)                              { (void)fp; (void)ofs; return FR_OK; }
static inline FRESULT  f_opendir(DIR *dp, const char *path)                     { (void)dp; (void)path; return FR_OK; }
static inline FRESULT  f_readdir(DIR *dp, FILINFO *fno)                         { (void)dp; (void)fno; return FR_OK; }
static inline FRESULT  f_closedir(DIR *dp)                                       { (void)dp; return FR_OK; }
static inline FRESULT  f_unlink(const char *path)                                { (void)path; return FR_OK; }
static inline FRESULT  f_mkdir(const char *path)                                 { (void)path; return FR_OK; }
static inline FRESULT  f_stat(const char *path, FILINFO *fno)                   { (void)path; (void)fno; return FR_OK; }

/* File open mode flags */
#define FA_READ   0x01
#define FA_WRITE  0x02
#define FA_OPEN_EXISTING 0x00
#define FA_CREATE_NEW    0x04
#define FA_CREATE_ALWAYS 0x08
#define FA_OPEN_ALWAYS   0x10

#endif /* __FATFS_H */