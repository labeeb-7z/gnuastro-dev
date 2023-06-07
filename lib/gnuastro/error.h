/*********************************************************************
error - error handling throughout the Gnuastro library
This is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Jash Shah <jash28582@gmail.com>
Contributing author(s):
     Mohammad Akhlaghi <mohammad@akhlaghi.org>
     Pedram Ashofteh-Ardakani <pedramardakani@pm.me>
Copyright (C) 2022 Free Software Foundation, Inc.

Gnuastro is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

Gnuastro is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef __GAL_ERROR_H__
#define __GAL_ERROR_H__

/* Include other headers if necessary here. Note that other header files
   must be included before the C++ preparations below */
#include <stdint.h>
#include <stdarg.h>
#include <gnuastro/error.h>

/* C++ Preparations */
#undef __BEGIN_C_DECLS
#undef __END_C_DECLS
#ifdef __cplusplus
# define __BEGIN_C_DECLS extern "C" {
# define __END_C_DECLS }
#else
# define __BEGIN_C_DECLS                /* empty */
# define __END_C_DECLS                  /* empty */
#endif
/* End of C++ preparations */





/* Actual header contants (the above were for the Pre-processor). */
__BEGIN_C_DECLS  /* From C++ preparations */





/* Given the `lib_code`,`code` and the `is_warning` flag of an error,
   returns the value whose least significant 8 bits represents the
   `is_warning` flag, next 8 bits represent the `code` and next
   significant 8 bits represent the library code(`lib_code`).


                     ┌──────────────────┐
                     │                  │
                     │32 Bit Macro Value│
                     │                  │
                     └─────────┬────────┘
                               │
                               │
          ┌────────────────────┼───────────────────┐
          │                    │                   │
     Bits 16-25           Bits 8-15           Bits 0-7
          │                    │                   │
     ┌───────▼────────┐  ┌────────▼────────┐ ┌────────▼───────┐
     │   lib_code     │  │      code       │ │   is_warning   │
     │                │  │                 │ │                │
     │   0000 0000    │  │    0000 0000    │ │    0000 0000   │
     └────────────────┘  └─────────────────┘ └────────────────┘
*/
#define GAL_ERROR_BITSET(lib_code, code, is_warning) ((lib_code << 16) | (code << 8) | is_warning)







/************************************************************
 **************        Error Structure        ***************
 ************************************************************/
/* Data type for storing errors */
typedef struct gal_error_t
{
  uint8_t code;              /* Code of the problem wrt to each library.*/
  uint8_t lib_code;          /* Library which created the error.        */
  uint8_t type;              /* Type... */
  uint8_t is_warning;        /* Defines if the error is only a warning. */
  char *back_msg;            /* Detailed message of backend (library)   */
  char *front_msg;           /* Detailed message of front end (caller). */
  struct gal_error_t *next;  /* Next error message.                     */
} gal_error_t;





/************************************************************
 **************        Library Codes          ***************
 ************************************************************/
/* To re-generate this list, run the following command:

      $ cd lib/gnuastro
      $ ls *.h \
           | sed 's/\.h//' \
           | awk '{printf "GAL_ERROR_LIB_%s,\n", toupper($1)}'

   You can then simply copy-paste the output below (in the specified
   region). */
enum gal_error_library_codes{
  GAL_ERROR_LIB_INVALID,     /* ==0: accoring to the C standard. */

  /*-------------------- Output of command above --------------------*/
  GAL_ERROR_LIB_ARITHMETIC,
  GAL_ERROR_LIB_ARRAY,
  GAL_ERROR_LIB_BINARY,
  GAL_ERROR_LIB_BLANK,
  GAL_ERROR_LIB_BOX,
  GAL_ERROR_LIB_COLOR,
  GAL_ERROR_LIB_CONVOLVE,
  GAL_ERROR_LIB_COSMOLOGY,
  GAL_ERROR_LIB_DATA,
  GAL_ERROR_LIB_DIMENSION,
  GAL_ERROR_LIB_DS9,
  GAL_ERROR_LIB_EPS,
  GAL_ERROR_LIB_ERROR,
  GAL_ERROR_LIB_ERRORINPROGRAM,
  GAL_ERROR_LIB_FIT,
  GAL_ERROR_LIB_FITS,
  GAL_ERROR_LIB_GIT,
  GAL_ERROR_LIB_INTERPOLATE,
  GAL_ERROR_LIB_JPEG,
  GAL_ERROR_LIB_KDTREE,
  GAL_ERROR_LIB_LABEL,
  GAL_ERROR_LIB_LIST,
  GAL_ERROR_LIB_MATCH,
  GAL_ERROR_LIB_PDF,
  GAL_ERROR_LIB_PERMUTATION,
  GAL_ERROR_LIB_POINTER,
  GAL_ERROR_LIB_POLYGON,
  GAL_ERROR_LIB_POOL,
  GAL_ERROR_LIB_PYTHON,
  GAL_ERROR_LIB_QSORT,
  GAL_ERROR_LIB_SPECLINES,
  GAL_ERROR_LIB_STATISTICS,
  GAL_ERROR_LIB_TABLE,
  GAL_ERROR_LIB_THREADS,
  GAL_ERROR_LIB_TIFF,
  GAL_ERROR_LIB_TILE,
  GAL_ERROR_LIB_TXT,
  GAL_ERROR_LIB_TYPE,
  GAL_ERROR_LIB_UNITS,
  GAL_ERROR_LIB_WARP,
  GAL_ERROR_LIB_WCS,
  /*-----------------------------------------------------------------*/

  GAL_ERROR_LIB_NUMLIBS /* Total number of libraies */
};




/*****************************************************************
 **************        Python Error Types          ***************
 *****************************************************************/
/* Classifies each  of the libary  errors into one of the following
   Python error types. This is used to decide which Python exception
   to raise when an error occurs in the Python interface of Gnuastro. */



/* Inspired from https://docs.python.org/3/library/exceptions.html */

enum gal_error_python_types{

    /* Raised when an assert() statement fails. */
    GAL_ERROR_PYTHON_ASSERTION,

    /* Raised when accessing or modifying an attribute of an object that does not exist. */
    GAL_ERROR_PYTHON_ATTRIBUTE,

    /* Raised when an operation would block on an object set for non-blocking operation. */
    GAL_ERROR_PYTHON_BLOCKINGIO,

    /* Raised when trying to write on a pipe while the other end has been closed. */
    GAL_ERROR_BROKENPIPE,

    /* Raised when trying to create a file or directory which already exists. */
    GAL_ERROR_PYTHON_FILEEXISTS,

    /* Raised when a file or directory is requested but doesn’t exist. */
    GAL_ERROR_PYTHON_FILENOTFOUND,

    /* Raised when a system call is interrupted by an incoming signal. */
    GAL_ERROR_PYTHON_INTERRUPTED,

    /* Raised when the input() function hits an end-of-file condition (EOF) without reading any data. */
    GAL_ERROR_PYTHON_EOF,

    /* Raised when a file operation is requested on a directory. */
    GAL_ERROR_PYTHON_ISADIRECTORY,

    /* Raised when the import statement has troubles trying to load a module. */
    GAL_ERROR_PYTHON_IMPORT,

    /* Raised when a sequence subscript is out of range. */
    GAL_ERROR_PYTHON_INDEX,

    /* Raised when a key is not found in a dictionary. */
    GAL_ERROR_PYTHON_KEY,

    /* Raised when the keyboard is interrupted (for example Ctrl+C). */
    GAL_ERROR_PYTHON_KEYBOARD,

    /* Raised when an operation runs out of memory but the situation may still be rescued. */
    GAL_ERROR_PYTHON_MEMORY,

    /* Raised when a local or global name is not found. */
    GAL_ERROR_PYTHON_NAME,

    /* Raised when the result of an arithmetic operation is too large to be represented. */
    GAL_ERROR_PYTHON_OVERFLOW,

    /* Raised when trying to run an operation without the adequate access rights. */
    GAL_ERROR_PYTHON_PERMISSION,

    /* Raised when the interpreter detects that the maximum recursion depth. */
    GAL_ERROR_PYTHON_RECURSION,

    /* Raised when an error is detected that doesn’t fall in any of the other categories. */
    GAL_ERROR_PYTHON_RUNTIME,

    /* Raised when the program exits. */
    GAL_ERROR_PYTHON_SYSTEMEXIT,

    /* Raised when a system function timed out at the system level. */
    GAL_ERROR_PYTHON_TIMEOUT,

    /*  Raised when the type of a built-in operation or function is invalid for the given arguments. */
    GAL_ERROR_PYTHON_TYPE,

    /* Raised when a built-in operation or function receives an argument that has the right type but an inappropriate value. */
    GAL_ERROR_PYTHON_VALUE,

    /* Raised when a division or modulo by zero is performed for all numeric types. */
    GAL_ERROR_PYTHON_ZERODIVISION,

    /* Raised when the Library error does not correspond to any of the error types in Python.  */
    GAL_ERROR_PYTHON_UNKNOWN,


    GAL_ERROR_PYTHON_NTYPES
};





/****************************************************************
 ************************   Allocation   ************************
 ****************************************************************/
gal_error_t *
gal_error_allocate(uint8_t lib_code, uint8_t code, char *back_msg,
                   uint8_t is_warning);

void
gal_error_add_back_msg(gal_error_t **err, char *back_msg,
                       uint32_t macro_val);

void
gal_error_add_front_msg(gal_error_t **err, char *front_msg,
                        uint8_t replace);

void
gal_error(gal_error_t **err, int lib_code, int error_code,
          int is_warning, char *format, ...);

void
gal_error_reverse(gal_error_t **err);

/****************************************************************
 *************************   Checking   *************************
 ****************************************************************/
uint8_t
gal_error_check(gal_error_t **err, uint32_t macro_val);

int
gal_error_exists_leave_func(gal_error_t **err, int lib_code,
                            int error_code, int is_warning,
                            const char *func);

void
gal_error_parse_macro(uint32_t macro_val, uint8_t *lib_code, uint8_t *code,
                      uint8_t *is_warning);

uint8_t
gal_error_occurred(gal_error_t *err);


/****************************************************************
 *************************   Priting   **************************
 ****************************************************************/
char *
gal_error_to_string(gal_error_t *err, int verbose);

int
gal_error_to_stderr_all(gal_error_t *err, int verbose);

__END_C_DECLS    /* From C++ preparations */

#endif           /* __GAL_ERROR_H__ */
