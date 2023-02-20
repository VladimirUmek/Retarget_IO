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


int32_t fs_open (const char *path, int32_t mode) {
  int openmode;
  int flag;
  int rval;

  /* Set the open mode flags */
  if ((mode & (FS_OPEN_RDONLY | FS_OPEN_WRONLY | FS_OPEN_RDWR)) == FS_OPEN_WRONLY) {
    openmode = OPEN_W;
    flag     = 0;
  } else
  if ((mode & (FS_OPEN_RDONLY | FS_OPEN_WRONLY | FS_OPEN_RDWR)) == FS_OPEN_RDWR) {
    openmode = OPEN_W;
    flag     = OPEN_PLUS;
  } else {
    /* ((mode & (FS_OPEN_RDONLY | FS_OPEN_WRONLY | FS_OPEN_RDWR)) == FS_OPEN_RDONLY) */
    openmode = OPEN_R;
    flag     = 0;
  }

  if (mode & FS_OPEN_APPEND) {
    openmode = OPEN_A;
  }

  rval = __sys_open(path, openmode| flag);

  return (rval);
}

int32_t fs_close (int32_t fd) {
  int32_t rval;

  rval = __sys_close(fd);

  if (rval < -1) {
    /* Return -1 on failure */
    rval = -1;
  }

  return (rval);
}

int32_t fs_write (int32_t fd, const void *buf, uint32_t cnt) {
  int32_t rval;
  int32_t n;

  n =__sys_write(fd, buf, cnt);

  if (n >= 0) {
    /* Return number of bytes written */
    rval = (int32_t)cnt - n;
  } else {
    /* Indicate write error */
    rval = -1;
  }

  return (rval);
}

int32_t fs_read (int32_t fd, void *buf, uint32_t cnt) {
  int32_t rval;
  int32_t n;

  n = __sys_read(fd, buf, cnt);

  if (n >= 0) {
    /* Return number of bytes read */
    rval = (int32_t)cnt - n;
  } else {
    /* Indicate read error */
    rval = -1;
  }

  return (rval);
}

int64_t fs_seek (int32_t fd, int64_t offset, int32_t whence) {
  int64_t rval;

  if (whence == FS_SEEK_SET) {
    if (offset <= UINT32_MAX) {
      /* Seek from the start of the file */
      rval = __sys_seek(fd, (uint32_t)offset);
    } else {
      /* Cannot seek, offset is to large */
      rval = -1;
    }
  }
  else {
    /* Not supported: FS_SEEK_CUR, FS_SEEK_END */
    rval = -1;
  }

  return (rval);
}

int64_t fs_size (int32_t fd) {
  int32_t rval;
  int64_t sz;

  rval = __sys_flen(fd);

  if (rval >= 0) {
    /* Size was returned */
    sz = (int64_t)rval;
  } else {
    /* Indicate error */
    sz = -1;
  }

  return (sz);
}

int32_t fs_remove (const char *path) {
  int32_t rval;
  fsStatus stat;

  stat = fdelete (path, NULL);

  if (stat == fsOK) {
    /* File or directory deleted */
    rval = 0;
  } else {
    /* Indicate error */
    rval = -1;
  }

  return (rval);
}

int32_t fs_rename (const char *oldpath, const char *newpath) {
  int32_t rval;
  fsStatus stat;

  stat = frename (oldpath, newpath);

  if (stat == fsOK) {
    /* File or directory renamed */
    rval = 0;
  } else {
    /* Indicate error */
    rval = -1;
  }

  return (rval);
}
