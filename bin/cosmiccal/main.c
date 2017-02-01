/*********************************************************************
CosmicCalculator - Calculate cosmological parameters
CosmicCalculator is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <akhlaghi@gnu.org>
Contributing author(s):
Copyright (C) 2016, Free Software Foundation, Inc.

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
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include <timing.h>    /* Includes time.h and sys/time.h */

#include "main.h"

#include "ui.h"                 /* needs main.h.                  */
#include "cosmiccal.h"          /* needs main.h.                  */

int
main (int argc, char *argv[])
{
  struct cosmiccalparams p={{0}, 0};

  /* Get the starting time. */
  time(&p.rawtime);

  /* Read the input parameters. */
  ui_read_check_inputs_setup(argc, argv, &p);

  /* Run MakeProfiles */
  cosmiccal(&p);

  /* Return successfully.*/
  return EXIT_SUCCESS;
}
