/*********************************************************************
Argument parsing for options that are common to all programs.
This is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <akhlaghi@gnu.org>
Contributing author(s):
Copyright (C) 2015, Free Software Foundation, Inc.

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
#ifndef __GAL_COMMONARGS_H__
#define __GAL_COMMONARGS_H__

#include <argp.h>
#include <string.h>
#include <stdlib.h>

#include <fitsio.h>

#include <gnuastro/checkset.h>
#include <gnuastro/commonparams.h>
#include <gnuastro/fixedstringmacros.h>

/* These are separate in each package. */
#include "main.h"
#include "cite.h"








/* Fixed options for all the programs:

   Free letters (-V which is used by GNU is also removed):

   a b c d e f g i j k l m n p r s t u v w x y z
   A B C E F G H I J L M O Q R T U W X Y Z

   Used numbers <= 1003

   You can use this above list to set short options for the different
   utilities.
 */
static struct argp_option gal_commonargs_options[] =
  {
    /* -1 is the code for operating modes: */
    {
      "quiet",
      'q',
      0,
      0,
      "Only report errors, remain quiet about steps.",
      -1
    },
    {
      "printparams",
      'P',
      0,
      0,
      "Print parameter values to be used and abort.",
      -1
    },
    {
      "setdirconf",
      'S',
      0,
      0,
      "Set default values for this directory and abort.",
      -1
    },
    {
      "setusrconf",
      'U',
      0,
      0,
      "Set default values for this user and abort.",
      -1
    },
    {
      "numthreads",
      'N',
      "INT",
      0,
      "Number of CPU threads to use.",
      -1
    },
    {
      "cite",
      1000,
      0,
      0,
      "BibTeX citation for "SPACK_NAME".",
      -1
    },
    {
      "onlydirconf",
      1001,
      0,
      0,
      "Only read current directory configuration file.",
      -1
    },
    {
      "onlyversion",
      1002,
      "STR",
      0,
      "Only run if the program version is this.",
      -1
    },
    {
      "nolog",
      1003,
      0,
      0,
      "No log file for programs which make one.",
      -1
    },




    /* 1 is the code for inputs. Some utilities might need their own
       HDU option reader, if that is the case, then the NOTCOMMONHDU
       is defined in their main.h.*/
#ifndef NOTCOMMONHDU
    {
      "hdu",
      'h',
      "STR",
      0,
      "Extension name or number of input data.",
      1
    },
#endif





    /* 2 is the code for output in all programs: */
    {
      "output",
      'o',
      "STR",
      0,
      "Output file or directory name.",
      2
    },
    {
      "dontdelete",
      'D',
      0,
      0,
      "Don't delete output if it exists.",
      2
    },
    {
      "keepinputdir",
      'K',
      0,
      0,
      "Do not remove input's dir info for output.",
      2
    },



    {0}
  };










static error_t
gal_checkset_commonargs_cparse_opt(int key, char *arg, struct argp_state *state)
{
  /* Save the arguments structure: */
  struct gal_commonparams *cp=state->input;

  /* In case the user incorrectly uses the equal sign (for example
     with a short format or with space in the long format, then `arg`
     start with (if the short version was called) or be (if the long
     version was called with a space) the equal sign. So, here we
     check if the first character of arg is the equal sign, then the
     user is warned and the program is stopped: */
  if(arg && arg[0]=='=')
    argp_error(state, "Incorrect use of the equal sign (`=`). For short "
	       "options, `=` should not be used and for long options, "
	       "there should be no space between the option, equal sign "
	       "and value.");

  switch(key)
    {
    /* Operating modes: */
    case 'q':
      cp->verb=0;
      break;
    case 'P':
      cp->printparams=1;
      break;
    case 'S':
      cp->setdirconf=1;
      break;
    case 'U':
      cp->setusrconf=1;
      break;
    case 'N':
      gal_checkset_sizet_l_zero(arg, &cp->numthreads, "numthreads", key,
                                cp->spack, NULL, 0);
      cp->numthreadsset=1;
      break;
    case 1000:
      PRINTCITEABORT;
      break;
    case 1001:
      cp->onlydirconf=1;
      break;
    case 1002:
      cp->onlyversion=arg;
      cp->onlyversionset=1;
      break;
    case 1003:
      cp->nolog=1;
      cp->nologset=1;
      break;

    /* Input/output: */
    case 'h':
      errno=0;
      cp->hdu=malloc(strlen(arg)+1);
      if(cp->hdu==NULL) error(EXIT_FAILURE, 0, "Space for hdu");
      strcpy(cp->hdu, arg);
      cp->hduset=1;
      break;
    case 'o':
      errno=0;
      cp->output=malloc(strlen(arg)+1);
      if(cp->output==NULL) error(EXIT_FAILURE, 0, "Space for output");
      strcpy(cp->output, arg); /* This allocation is done so cp->output */
      cp->outputset=1;	       /* Can always be freed when set, because */
      break;		       /* It usually needs modifications.       */
    case 'D':
      cp->dontdelete=1;
      break;
    case 'K':
      cp->removedirinfo=0;
      break;


    /* Once finished: */
    case ARGP_KEY_END:
      if(cp->setdirconf && cp->setusrconf)
	error(EXIT_FAILURE, 0, "Only one of `--setusrconf` or "
	      "`--setdirconf` may be set in each run. You have asked "
	      "for both.");
      break;

    /* If the argument is not known. */
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}










static struct argp commonargp = {gal_commonargs_options,
                                 gal_checkset_commonargs_cparse_opt,
                                 NULL, NULL, NULL, NULL, NULL};

#endif