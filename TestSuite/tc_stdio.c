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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "test.h"

static int Fn_OpenWriteClose (const char *path, uint32_t cnt);

/**
  Open file, write into it and close the file.

  This function opens the file for write ('w' mode) writes cnt bytes into
  the file and then closes it.
  
  \param[in]  path  The path to the file
  \param[in]  cnt   Number of bytes to write into the file
  
  \return 0 on success, non-zero on error
*/
int Fn_OpenWriteClose (const char *path, uint32_t cnt) {
  FILE *f;
  uint32_t n;
  int rval;

  rval = 0;

  /* Open a file in 'w' mode */
  f = fopen (path, "w");

  if (f == NULL) {
    /* Cannot open (create) file */
    rval = 1;
  }
  else {
    /* Write cnt bytes to the file */
    for (n = 0; n < cnt; n++) {
      if (fputc ('E', f) != 'E') {
        /* File write failed */
        rval = 2;
      }
    }

    if (fclose(f) != 0) {
      /* File close failed */
      rval = 3;
    }
  }
  return (rval);
}

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
/**
\defgroup kernel_funcs Kernel Functions
\brief Kernel Information and Control Test Cases
\details
The Kernel Information and Control function group in CMSIS-RTOS provides means for obtain information about the system, the
underlying kernel and the CMSIS-RTOS API. The test cases check the functions return values and calls from an ISR.

@{
*/

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/

/**
\brief Test case: TC_malloc_1
\details
  - Call malloc to allocate small buffer (16 bytes)
*/
void TC_malloc_1 (void) {
#if (TC_MALLOC_1_EN)
  void *p;

  /* Call malloc to allocate small buffer (16 bytes) */
  p = malloc(16);
  ASSERT_TRUE (p != NULL);

  if (p != NULL) {
    free(p);
  }
#endif
}

/**
\brief Test case: TC_malloc_2
\details
  - Call malloc to allocate more memory than available
*/
void TC_malloc_2 (void) {
#if (TC_MALLOC_2_EN)
  void *p;

  /* Call malloc to allocate more memory than available */
  p = malloc(HEAP_SIZE_TOTAL);
  ASSERT_TRUE (p == NULL);
#endif
}

/**
\brief Test case: TC_fcvt_1
\details
  Function fcvt() converts the floating-point value to NULL-terminated ASCII
  string and returns a pointer to it. It is defined in the stdlib.h.

  char *fcvt (double value, int num, int *dec, int *sign);
  Arguments:
  - double value: the floating-point value to convert into a string.
  - int num: a number of digits to be returned by the function.
             If this is greater than the number of digits in value then the
             rest of the string is padded with zeros and if it is smaller the
             low-order digit is rounded.
  - int *dec: an integer pointer, which stores the decimal point position with respect to beginning the string.
              If it is zero or less than zero, indicates that the decimal point
              lies to the left of the digits
  - int *sign: an integer pointer, which receives the sign indicator like 0 means
               positive sign and non-zero means negative.
  Return:
  - The function returns a character string terminated with null with the same
    length specified as num that contains the digits of the double number
    passes as a parameter.

*/
extern char *	fcvt (double,int,int *,int *);

void TC_fcvt_1 (void) {
#if (TC_FCVT_1_EN)
  double fp_val = 123.4567;
  int dec, sign;
  char *buf;

  // Function Call
  buf = fcvt (fp_val, 6, &dec, &sign);
  
  ASSERT_TRUE (buf  != NULL);
  ASSERT_TRUE (dec  == 3);
  ASSERT_TRUE (sign == 0);
#endif
}

/**
\brief Test case: TC_fopen_1
\details
  - Call fopen to open a file using 'w' mode
*/
void TC_fopen_1 (void) {
#if (TC_FOPEN_1_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Call fopen to open a file using 'w' mode */
  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fopen_2
\details
  - Call fopen to open a file using 'r' mode
*/
void TC_fopen_2 (void) {
#if (TC_FOPEN_2_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Create a file */
  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close created file */
    ASSERT_TRUE (fclose(f) == 0);
  }

  /* Call fopen to open a file using 'r' mode */
  f = fopen (path, "r");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fopen_3
\details
  - Call fopen to open a file using 'a' mode
*/
void TC_fopen_3 (void) {
#if (TC_FOPEN_3_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Call fopen to open a file using 'a' mode */
  f = fopen (path, "a");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fopen_4
\details
  - Call fopen to open a file using 'r+' mode
*/
void TC_fopen_4 (void) {
#if (TC_FOPEN_4_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Create a file */
  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close created file */
    ASSERT_TRUE (fclose(f) == 0);
  }

  /* Call fopen to open a file using 'r+' mode */
  f = fopen (path, "r+");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fopen_5
\details
  - Call fopen to open a file using 'w+' mode
*/
void TC_fopen_5 (void) {
#if (TC_FOPEN_5_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Call fopen to open a file using 'w+' mode */
  f = fopen (path, "w+");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fopen_6
\details
  - Call fopen to open a file using 'a+' mode
*/
void TC_fopen_6 (void) {
#if (TC_FOPEN_6_EN)
  FILE *f;
  const char *path;

  path = "file.txt";

  /* Call fopen to open a file using 'a+' mode */
  f = fopen (path, "a+");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_write_1
\details
  - Call fwrite using argument size == 1 and nitems == 1
  - Call fwrite using argument size == 4 and nitems == 4
  - Call fwrite using argument size == 2 and nitems == 8
  - Call fwrite using argument size == 8 and nitems == 2
*/
void TC_fwrite_1 (void) {
#if (TC_FWRITE_1_EN)
  FILE *f;
  const char *path;
  char buf[16];
  uint32_t i;
  size_t size;
  size_t nitems;

  path = "file.txt";

  /* Open a file in 'w' mode */
  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Fill buffer */
    for (i = 0; i < sizeof(buf); i++) {
      buf[i] = 'E';
    }

    /* Prevent buffering by setting stream as unbuffered */
    ASSERT_TRUE (setvbuf (f, NULL, _IONBF, 0U) == 0);

    /* Call fwrite using argument size == 1 and nitems == 1 */
    size   = 1;
    nitems = 1;
    ASSERT_TRUE (fwrite (buf, size, nitems, f) == 1);

    /* Call fwrite using argument size == 4 and nitems == 4 */
    size   = 4;
    nitems = 4;
    ASSERT_TRUE (fwrite (buf, size, nitems, f) == nitems);

    /* Call fwrite using argument size == 2 and nitems == 8 */
    size   = 2;
    nitems = 8;
    ASSERT_TRUE (fwrite (buf, size, nitems, f) == nitems);

    /* Call fwrite using argument size == 8 and nitems == 2 */
    size   = 8;
    nitems = 2;
    ASSERT_TRUE (fwrite (buf, size, nitems, f) == nitems);

    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fread_1
\details
  - Call fread using argument size == 1 and nitems == 1
*/
void TC_fread_1 (void) {
#if (TC_FREAD_1_EN)
  FILE *f;
  const char *path;
  char buf[16];
  size_t size;
  size_t nitems;

  path = "file.txt";

  ASSERT_TRUE (Fn_OpenWriteClose(path, 16) == 0);

  /* Open a file in 'r' mode */
  f = fopen (path, "r");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Call fread using argument size == 1 and nitems == 1 */
    size   = 1;
    nitems = 1;
    ASSERT_TRUE (fread (buf, size, nitems, f) == 1);

    /* Call fread using argument size == 4 and nitems == 4 */
    size   = 4;
    nitems = 4;
    ASSERT_TRUE (fread (buf, size, nitems, f) == nitems);

    /* Call fread using argument size == 2 and nitems == 8 */
    size   = 2;
    nitems = 8;
    ASSERT_TRUE (fread (buf, size, nitems, f) == nitems);

    /* Call fread using argument size == 8 and nitems == 2 */
    size   = 8;
    nitems = 2;
    ASSERT_TRUE (fread (buf, size, nitems, f) == nitems);

    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fseek_1
\details
  - Call fseek with SEEK_SET and offset == 0
  - Call fseek with SEEK_SET and offset == 31
*/
void TC_fseek_1 (void) {
#if (TC_FSEEK_1_EN)
  FILE *f;
  const char *path;
  uint32_t n;

  path = "file.txt";

  /* Open a file in 'w' mode */
  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Write 32 bytes to the file */
    for (n = 0; n < 32; n++) {
      ASSERT_TRUE (fputc ('E', f) == 'E');
    }

    /* Call fseek with SEEK_SET and offset == 0 */
    ASSERT_TRUE (fseek(f, 0, SEEK_SET) == 0);

    /* Call fseek with SEEK_SET and offset == 31 */
    ASSERT_TRUE (fseek(f, 31, SEEK_SET) == 0);

    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}

/**
\brief Test case: TC_fseek_2
\details
  - Call fseek with SEEK_SET and offset == 32 (beyond EOF)
*/
void TC_fseek_2 (void) {
#if (TC_FSEEK_2_EN)
  FILE *f;
  const char *path;
  uint32_t n;

  path = "file.txt";

  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Write 32 bytes to the file */
    for (n = 0; n < 32; n++) {
      ASSERT_TRUE (fputc ('E', f) == 'E');
    }

    /* Call fseek with SEEK_SET and offset == 32 (EOF) */
    ASSERT_TRUE (fseek(f, 32, SEEK_SET) == 0);

    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}


/**
\brief Test case: TC_fgetpos_1
\details
  - Call fgetpos to retrieve current file position
*/
void TC_fgetpos_1 (void) {
#if (TC_FGETPOS_1_EN)
  FILE *f;
  const char *path;
  uint32_t n;
  fpos_t pos;

  path = "file.txt";

  f = fopen (path, "w");
  ASSERT_TRUE (f != NULL);

  if (f != NULL) {
    /* Write 32 bytes to the file */
    for (n = 0; n < 32; n++) {
      ASSERT_TRUE (fputc ('E', f) == 'E');
    }

    /* Call fgetpos to retrieve current file position */
    ASSERT_TRUE (fgetpos (f, &pos) == 0);


    /* Close opened file */
    ASSERT_TRUE (fclose(f) == 0);
  }
#endif
}


/**
\brief Test case: TC_getchar_1
\details
  - Call getchar to get a byte from a stdin stream
*/
void TC_getchar_1 (void) {
#if (TC_GETCHAR_1_EN)
  int ch;

  /* Call getchar to get a byte from a stdin stream */
  ch = getchar();
  ASSERT_TRUE (ch != EOF);
#endif
}

/**
\brief Test case: TC_scanf_1
\details
  - Call scanf with "%c" format string
*/
void TC_scanf_1 (void) {
#if (TC_SCANF_1_EN)
  int rval;
  char c;

  rval = scanf("%c", &c);
  ASSERT_TRUE (rval == 1);
#endif
}

/**
\brief Test case: TC_scanf_2
\details
  - Call scanf with "%s" format string (32 bytes max)
*/
void TC_scanf_2 (void) {
#if (TC_SCANF_2_EN)
  int rval;
  char s[32];

  /* Call scanf with "%s" format string (32 bytes max) */
  rval = scanf("%s", s);
  ASSERT_TRUE (rval == 1);
#endif
}
