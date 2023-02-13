# Call chains and Notes To Myself
---------------------------------

See https://www.gnu.org/software/libc/manual/html_node/Standard-Streams.html

## Reset

Application that contains only CMSIS::Core, Device::C Startup and main function already requires:
- _exit
- _close
- _lseek
- _read
- _write
- _sbrk

Application that contains only CMSIS::Core, Device::C Startup, main function and is linked using
option "-nostartfiles" requires only _start function implementation (no syscalls).

```
/* newlib nano, no _start function provided */
Reset_Handler -> __cmsis_start -> _start (i.e. _mainCRTStartup) -> __libc_init_array -> _init -> main

/* newlib, no _start function provided */
Reset_Handler -> __cmsis_start -> _start (i.e. _mainCRTStartup) -> atexit -> __register_exitproc
                                                                          -> __retarget_lock_acquire_recursive
                                                                          -> __retarget_lock_release_recursive
                                                                -> __libc_init_array -> _init -> main

/* Application must provide _start function when newlib is linked with "-nostartfiles" option */
```

## System

```
abort -> write (2, "Abort called\n", sizeof ("Abort called\n")-1)
      -> raise(SIGABT)
      -> _exit(1)
```
```
raise -> _getpid_r
      -> _kill_r
```

## printf
```
printf -> puts -> _puts_r -> __sinit -> __sfp_lock_acquire -> __retarget_lock_acquire_recursive
                                     -> __sfp_lock_release
                                     -> global_stdio_init.part.0 -> std -> __retarget_lock_init_recursive
                                     -> __sfp_lock_release -> __retarget_lock_release_recursive

                          -> __swsetup_r -> __smakebuf_r -> __swhatbuf_r -> _fstat_r -> _fstat
```

## File Operations

```
fwrite-> _puts_r -> __sfvwrite_r -> _fflush_r -> __sflush_r -> _write_r -> _write
```
```
fopen -> _fopen_r -> __sflags
                  -> __sfp -> _malloc_r -> __malloc_lock   -> __retarget_lock_acquire_recursive
                                        -> __malloc_unlock -> __retarget_lock_release_recursive
                  -> _open_r -> _open
```

```
remove -> _remove_r -> _unlink_r -> _unlink
```

```
rename -> _rename_r -> _link_r -> _link
                    -> _unlink_r -> _unlink
```

```
getchar -> _getc_r -> __srget_r -> __srefill_r -> __sread -> _read_r -> _read
```

## Memory allocation

```
malloc-> _malloc_r -> __malloc_lock   -> __retarget_lock_acquire_recursive
                   -> _sbrk_r
                   -> __malloc_unlock -> __retarget_lock_release_recursive
```

## Floating point
```
fcvt -> fcvtbuf -> _realloc_r -> _malloc_r -> ...
                -> _dtoa_r -> __aeabi_dcmpeq -> __aeabi_cdcmple -> __cmpdf2
                           -> __d2b -> _Balloc -> _calloc_r -> _malloc-> ...
```

## Notes
  - No need to reimplement __malloc_lock/__malloc_unlock pair when __retarget_lock_release_recursive
    is implemented. It might have sense to do it in case of more efficient implementation.

  - Environment locks __env_lock/__env_unlock call __lock_acquire_recursive/__lock_release_recursive.
    No need to reimplement them unless if more efficient implementation is possible.

  - _reent structure is defined in reent.h, line 375 (_REENT_SMALL) or line 567 (full sized)

# File System
-------------

## Open function

### Open Mode Flags
-------------------
IAR:
```
      /* Open modes */
      #define _LLIO_RDONLY      0x0000
      #define _LLIO_WRONLY      0x0001
      #define _LLIO_RDWR        0x0002
      #define _LLIO_APPEND      0x0008
      #define _LLIO_CREAT       0x0100
      #define _LLIO_TRUNC       0x0200
      #define _LLIO_EXCL        0x0400
      #define _LLIO_BINARY      0x8000
      #define _LLIO_TEXT        0x4000
```

GCC:
```
      /* Access modes for open(), openat(), fcntl() */
      #define O_ACCMODE         0 // Mask for file access modes

      #define O_RDONLY          0x0000 // Open for reading only.
      #define O_WRONLY          0x0001 // Open for writing only.
      #define O_RDWR            0x0002 // Open for reading and writing.
      #define O_APPEND          0x0008 // Set append mode.

      #define O_CREAT           0x0200 // Create file if it does not exist.
      #define O_TRUNC           0x0400 // Truncate flag.
      #define O_EXCL            0x0800 // Exclusive use flag.
      #define O_SYNC            0x2000 // Write according to synchronized I/O file integrity completion.
                          //
      #define O_NONBLOCK  // Non-blocking mode.
      #define O_NOCTTY    // Do not assign controlling terminal.
                          //
      #define O_CLOEXEC   // The FD_CLOEXEC flag associated with the new descriptor shall be set to close the file descriptor upon execution of an exec family function.
      #define O_NOFOLLOW  // Do not follow symbolic links.
      #define O_DIRECTORY // Fail if file is a non-directory file.
      #define O_EXEC      // Open for execute only (non-directory files). The result is unspecified if this flag is applied to a directory.
      #define O_SEARCH    // Open directory for search only. The result is unspecified if this flag is applied to a non-directory file.
```

ARM:
```
      /* Open mode */
      #define OPEN_R                 0
      #define OPEN_PLUS              2
      #define OPEN_W                 4
      #define OPEN_A                 8
      #define OPEN_B                 1
```

### Return Value
----------------
IAR: -1 on error.

GCC: Upon successful completion, these functions shall open the file and return a non-negative
     integer representing the file descriptor. Otherwise, these functions shall return -1 and
     set errno to indicate the error. If -1 is returned, no files shall be created or modified.

ARM: The return value is -1 if an error occurs.

## Close function

### Return Value
----------------
IAR: It should return 0 on success and nonzero on failure.

GCC: Upon successful completion, 0 shall be returned;
     otherwise, -1 shall be returned and errno set to indicate the error.

ARM: The return value is 0 if successful. A nonzero value indicates an error.

## Write function

### Return Value
----------------
IAR: It should return the number of characters written, or _LLIO_ERROR on failure.

GCC: Upon successful completion, these functions shall return the number of bytes actually
     written to the file associated with fildes.
     This number shall never be greater than nbyte.
     Otherwise, -1 shall be returned and errno set to indicate the error.

ARM: The return value is either:
              - a positive number representing the number of characters not
                written (so any nonzero return value denotes a failure of
                some sort)
              - a negative number indicating an error.
## Read function

### Return Value
-----------------
IAR: It returns the number of bytes read, 0 at the end of the file, or _LLIO_ERROR
     if failure occurs.

GCC: Upon successful completion, function returns a non-negative integer indicating
     the number of bytes actually read.
     Otherwise, the functions shall return -1 and set errno to indicate the error.

ARM: The return value is one of the following:
              - The number of bytes not read (that is, len - result number of
                bytes were read).
              - An error indication.
              - An EOF indicator. The EOF indication involves the setting of
                0x80000000 in the normal result.