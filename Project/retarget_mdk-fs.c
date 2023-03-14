/*
 * Copyright (C) 2023 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <rt_sys.h>
#include "retarget_fs.h"
#include "rl_fs_lib.h"
#include "rl_fs.h"

#define fd_rval(fd) (((fd >> 8) & 0x0000FF00) | (fd & 0x000000FF))
#define fd_arg(fd)  (((fd & 0x0000FF00) << 8) | (fd & 0x000000FF))

/* Convert fsStatus value to retarget return code */
static int32_t fs_to_rt_rval (fsStatus fs_rval) {
  int32_t rt_rval;

  if      (fs_rval == fsUnsupported)        { rt_rval = RT_ERR_NOTSUP;   }
  else if (fs_rval == fsAccessDenied)       { rt_rval = RT_ERR_BUSY;     }
  else if (fs_rval == fsInvalidParameter)   { rt_rval = RT_ERR_INVAL;    }
  else if (fs_rval == fsInvalidDrive)       { rt_rval = RT_ERR_NOTFOUND; }
  else if (fs_rval == fsInvalidPath)        { rt_rval = RT_ERR_NOTFOUND; }
  else if (fs_rval == fsUninitializedDrive) { rt_rval = RT_ERR_IO;       }
  else if (fs_rval == fsDriverError)        { rt_rval = RT_ERR_IO;       }
  else if (fs_rval == fsMediaError)         { rt_rval = RT_ERR_IO;       }
  else if (fs_rval == fsNoMedia)            { rt_rval = RT_ERR_IO;       }
  else if (fs_rval == fsNoFreeSpace)        { rt_rval = RT_ERR_NOSPACE;  }
  else if (fs_rval == fsFileNotFound)       { rt_rval = RT_ERR_NOTFOUND; }
  else if (fs_rval == fsDirNotEmpty)        { rt_rval = RT_ERR_NOTEMPTY; }
  else if (fs_rval == fsTooManyOpenFiles)   { rt_rval = RT_ERR_MAXFILES; }
  else if (fs_rval == fsAlreadyExists)      { rt_rval = RT_ERR_EXIST;    }
  else                                      { rt_rval = RT_ERR;          }

  return (rt_rval);
}

int32_t rt_fs_open (const char *path, int32_t mode) {
  int openmode;
  int flag;
  int rval;

  /* Set the open mode flags */
  if ((mode & (RT_OPEN_RDONLY | RT_OPEN_WRONLY | RT_OPEN_RDWR)) == RT_OPEN_WRONLY) {
    openmode = OPEN_W;
    flag     = 0;
  } else
  if ((mode & (RT_OPEN_RDONLY | RT_OPEN_WRONLY | RT_OPEN_RDWR)) == RT_OPEN_RDWR) {
    openmode = OPEN_W;
    flag     = OPEN_PLUS;
  } else {
    /* ((mode & (RT_OPEN_RDONLY | RT_OPEN_WRONLY | RT_OPEN_RDWR)) == RT_OPEN_RDONLY) */
    openmode = OPEN_R;
    flag     = 0;
  }

  if (mode & RT_OPEN_APPEND) {
    openmode = OPEN_A;
  }

  rval = __sys_open(path, openmode| flag);

  if (rval > 0) {
    rval = fd_rval(rval);
  } else {
    rval = fs_to_rt_rval ((fsStatus)rval);
  }

  return (rval);
}

int32_t rt_fs_close (int32_t fd) {
  int32_t rval;

  rval = __sys_close(fd_arg(fd));

  if (rval != 0) {
    rval = fs_to_rt_rval ((fsStatus)rval);
  }

  return (rval);
}

int32_t rt_fs_write (int32_t fd, const void *buf, uint32_t cnt) {
  int32_t rval;
  int32_t n;

  n =__sys_write(fd_arg(fd), buf, cnt);

  if (n >= 0) {
    /* Return number of bytes written */
    rval = (int32_t)cnt - n;
  } else {
    /* Indicate write error */
    rval = fs_to_rt_rval ((fsStatus)-n);
  }

  return (rval);
}

int32_t rt_fs_read (int32_t fd, void *buf, uint32_t cnt) {
  int32_t rval;
  int32_t n;

  n = __sys_read(fd_arg(fd), buf, cnt);

  n &= ~0x80000000;

  if (n >= 0) {
    /* Return number of bytes read */
    rval = (int32_t)cnt - n;
  } else {
    /* Indicate read error */
    rval = fs_to_rt_rval ((fsStatus)-n);
  }

  return (rval);
}

int64_t rt_fs_seek (int32_t fd, int64_t offset, int32_t whence) {
  int64_t rval;

  if ((whence == RT_SEEK_SET) && (offset <= UINT32_MAX)) {
    /* Seek from the start of the file */
    rval = __sys_seek(fd_arg(fd), (uint32_t)offset);

    if (rval != 0) {
      rval = fs_to_rt_rval ((fsStatus)-rval);
    }

  } else {
    /* Not supported: RT_SEEK_CUR, RT_SEEK_END */
    rval = RT_ERR_NOTSUP;
  }

  return (rval);
}

int64_t rt_fs_size (int32_t fd) {
  int32_t rval;
  int64_t sz;

  rval = __sys_flen(fd_arg(fd));

  if (rval >= 0) {
    /* Size was returned */
    sz = (int64_t)rval;
  } else {
    /* Indicate error */
    sz = fs_to_rt_rval ((fsStatus)-rval);
  }

  return (sz);
}

int32_t rt_fs_stat (int32_t fd, rt_fs_stat_t *stat) {
  (void)fd;
  (void)stat;

  return RT_ERR_NOTSUP;
}

int32_t rt_fs_remove (const char *path) {
  int32_t rval;
  fsStatus stat;

  stat = fdelete (path, NULL);

  if (stat == fsOK) {
    /* File or directory deleted */
    rval = 0;
  } else {
    /* Indicate error */
    rval = fs_to_rt_rval ((fsStatus)stat);
  }

  return (rval);
}

int32_t rt_fs_rename (const char *oldpath, const char *newpath) {
  int32_t rval;
  fsStatus stat;

  stat = frename (oldpath, newpath);

  if (stat == fsOK) {
    /* File or directory renamed */
    rval = 0;
  } else {
    /* Indicate error */
    rval = fs_to_rt_rval ((fsStatus)stat);
  }

  return (rval);
}
