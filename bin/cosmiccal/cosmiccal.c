/*********************************************************************
CosmicCalculator - Calculate cosmological parameters
CosmicCalculator is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <mohammad@akhlaghi.org>
Contributing author(s):
Copyright (C) 2016-2023 Free Software Foundation, Inc.

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

#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>

#include <gnuastro/cosmology.h>
#include <gnuastro/errorinprogram.h>

#include "main.h"

#include "ui.h"
#include "cosmiccal.h"











/**************************************************************/
/************            Main function            *************/
/**************************************************************/
static void
cosmiccal_print_input(struct cosmiccalparams *p)
{
  printf("%s\n", PROGRAM_STRING);
  printf("\n Input parameters\n");
  printf(  " ----------------\n");
  if( !isnan(p->redshift) )
    printf(FLTFORMAT, "Desired redshift for calculations (z):", p->redshift);
  printf(FLTFORMAT, "Expansion rate (Hubble constant, H0), now:", p->H0);
  printf(FLTFORMAT, "Cosmological constant fractional density, now:",
         p->olambda);
  printf(FLTFORMAT, "Matter fractional density, now:", p->omatter);
  printf(EXPFORMAT, "Radiation fractional density, now:", p->oradiation);
  printf(EXPFORMAT, "Curvature fractional density (from the above):",
         1 - ( p->olambda + p->omatter + p->oradiation ));
}





static void
cosmiccal_printall(struct cosmiccalparams *p)
{
  double ad, ld, vz, pd, vel, absmagconv;
  double curage, ccritd, distmod, outage, zcritd;
  gal_error_t *err = NULL;

  /* The user wants everything, do all the calculations and print
     everything with full descriptions. */
  curage=gal_cosmology_age(0.0f, p->H0, p->olambda, p->omatter,
                           p->oradiation, &err);

  ccritd=gal_cosmology_critical_density(0.0f, p->H0, p->olambda, p->omatter,
                                        p->oradiation, &err);

  pd=gal_cosmology_proper_distance(p->redshift, p->H0, p->olambda, p->omatter,
                                   p->oradiation, &err);

  ad=gal_cosmology_angular_distance(p->redshift, p->H0, p->olambda,
                                    p->omatter, p->oradiation, &err);

  ld=gal_cosmology_luminosity_distance(p->redshift, p->H0, p->olambda,
                                       p->omatter, p->oradiation, &err);

  distmod=gal_cosmology_distance_modulus(p->redshift, p->H0, p->olambda,
                                         p->omatter, p->oradiation, &err);

  absmagconv=gal_cosmology_to_absolute_mag(p->redshift, p->H0, p->olambda,
                                           p->omatter, p->oradiation, &err);

  outage=gal_cosmology_age(p->redshift, p->H0, p->olambda, p->omatter,
                           p->oradiation, &err);

  zcritd=gal_cosmology_critical_density(p->redshift, p->H0, p->olambda,
                                        p->omatter, p->oradiation, &err);

  vel=gal_cosmology_velocity_from_z(p->redshift);

  vz=gal_cosmology_comoving_volume(p->redshift, p->H0, p->olambda, p->omatter,
                                   p->oradiation, &err);

  /* Incase an error passed through the ui sanity checks.
  if(gal_error_occurred(err))
    error(EXIT_FAILURE, 0, "%s: a bug! Please contact us at %s to "
          "fix the problem. The values provided for the "
          "cosmological constants (%f, %f, %f) don't satisfy "
          "their constraints.", __func__, PACKAGE_BUGREPORT,
          p->olambda, p->omatter, p->oradiation);
  */

  /* Print out results: */
  cosmiccal_print_input(p);


  printf("\n\n Universe now\n");
  printf(    " ------------\n");
  printf(FLTFORMAT, "Age of Universe now (Ga*):", curage);
  printf(EXPFORMAT, "Critical density now (g/cm^3):",  ccritd);
  printf(FLTFORMAT, "Velocity at z (km/s):", vel);
  printf(FLTFORMAT, "Proper distance to z (Mpc):", pd);
  printf(FLTFORMAT, "Angular diameter distance to z (Mpc):", ad);
  printf(FLTFORMAT, "Tangential distance covered by 1 arcsec at z (Kpc):",
         ad*1000*M_PI/3600/180);
  printf(FLTFORMAT, "Luminosity distance to z (Mpc):", ld);
  printf(FLTFORMAT, "Distance modulus at z (no unit):", distmod);
  printf(FLTFORMAT, "Conversion to absolute magnitude (no unit):",
         absmagconv);


  printf("\n\n Universe at desired redshift z\n");
  printf(    " ------------------------------\n");
  printf(FLTFORMAT, "Age of Universe at z (Ga*):", outage);
  printf(FLTFORMAT, "Look-back time to z (Ga*):", curage-outage);
  printf(EXPFORMAT, "Critical density at z (g/cm^3):",  zcritd);

  printf("\n\n Comoving universe (time independent)\n");
  printf(    " ------------------------------------\n");
  printf(FLTFORMAT, "Comoving volume over 4pi stradian to z (Mpc^3):", vz);

  printf("\n-------\n");
  printf("*: Ga is short for Giga Annum, or billion years (IAU standard).\n");
}





void
cosmiccal(struct cosmiccalparams *p)
{
  gal_list_i32_t *tmp;
  double curage, zage;
  gal_error_t *err = NULL;

  /* If no redshift is given, it is a bug that should be fixed by us (the
     developers). */
  if(isnan(p->redshift))
    error(EXIT_FAILURE, 0, "%s: a bug! Please contact us at '%s' to fix "
          "the problem. The value of 'redshift' should not be NaN at "
          "this point", __func__, PACKAGE_BUGREPORT);

  /* In case the user just wants one number, only print that and
     return. */
  if(p->specific)
    {
      for(tmp=p->specific;tmp!=NULL;tmp=tmp->next)
        {
          switch(tmp->v)
            {
            case UI_KEY_USEDREDSHIFT:
              printf("%g",
                     p->redshift==MAIN_REDSHIFT_ZERO ? 0.0f: p->redshift);
              break;

            case UI_KEY_AGENOW:
              printf("%f", gal_cosmology_age(0.0f, p->H0, p->olambda,
                                             p->omatter, p->oradiation,
                                             &err));
              break;

            case UI_KEY_CRITICALDENSITYNOW:
              printf("%e", gal_cosmology_critical_density(0.0f, p->H0,
                                                          p->olambda,
                                                          p->omatter,
                                                          p->oradiation,
                                                          &err));
              break;

            case UI_KEY_PROPERDISTANCE:
              printf("%f", gal_cosmology_proper_distance(p->redshift,
                                                         p->H0,
                                                         p->olambda,
                                                         p->omatter,
                                                         p->oradiation,
                                                         &err));
              break;

            case UI_KEY_ANGULARDIMDIST:
              printf("%f", gal_cosmology_angular_distance(p->redshift,
                                                          p->H0,
                                                          p->olambda,
                                                          p->omatter,
                                                          p->oradiation,
                                                          &err));
              break;

            case UI_KEY_ARCSECTANDIST:
              printf("%f", ( gal_cosmology_angular_distance(p->redshift,
                                                            p->H0,
                                                            p->olambda,
                                                            p->omatter,
                                                            p->oradiation,
                                                            &err)
                              * 1000 * M_PI / 3600 / 180 ) );
              break;

            case UI_KEY_LUMINOSITYDIST:
              printf("%f", gal_cosmology_luminosity_distance(p->redshift,
                                                             p->H0,
                                                             p->olambda,
                                                             p->omatter,
                                                             p->oradiation,
                                                             &err));
              break;

            case UI_KEY_DISTANCEMODULUS:
              printf("%f", gal_cosmology_distance_modulus(p->redshift,
                                                          p->H0,
                                                          p->olambda,
                                                          p->omatter,
                                                          p->oradiation,
                                                          &err));
              break;

            case UI_KEY_ABSMAGCONV:
              printf("%f", gal_cosmology_to_absolute_mag(p->redshift,
                                                         p->H0,
                                                         p->olambda,
                                                         p->omatter,
                                                         p->oradiation,
                                                         &err));
              break;

            case UI_KEY_AGE:
              printf("%f", gal_cosmology_age(p->redshift, p->H0,
                                             p->olambda,
                                             p->omatter,
                                             p->oradiation, &err));
              break;

            case UI_KEY_LOOKBACKTIME:
              curage=gal_cosmology_age(0.0f, p->H0, p->olambda, p->omatter,
                                       p->oradiation, &err);
              zage=gal_cosmology_age(p->redshift, p->H0, p->olambda,
                                     p->omatter, p->oradiation, &err);
              printf("%f", curage-zage);
              break;

            case UI_KEY_CRITICALDENSITY:
              printf("%e", gal_cosmology_critical_density(p->redshift,
                                                          p->H0,
                                                          p->olambda,
                                                          p->omatter,
                                                          p->oradiation,
                                                          &err));
              break;

            case UI_KEY_VOLUME:
              printf("%f", gal_cosmology_comoving_volume(p->redshift, p->H0,
                                                         p->olambda,
                                                         p->omatter,
                                                         p->oradiation,
                                                         &err));
              break;

            case UI_KEY_USEDVELOCITY:
              printf("%g", gal_cosmology_velocity_from_z(p->redshift));
              break;

            case UI_KEY_LINEATZ:
              printf("%g", ( gal_list_f64_pop(&p->specific_arg)
                             * (1+p->redshift) * p->lineunitmultip) );
              break;

            default:
              error(EXIT_FAILURE, 0, "%s: a bug! Please contact us at %s "
                    "to fix the problem. The code %d is not recognized "
                    "as a single value calculation code", __func__,
                    PACKAGE_BUGREPORT, tmp->v);
            }

          /* Add a space-character if there are more results to print. */
          if(tmp->next) printf(" ");
        }

      /* Print a new-line character to finish the output. */
      printf("\n");
    }
  else
    cosmiccal_printall(p);

  /* Incase any error occurred. */
  gal_errorinprogram(err, p->cp.verboseerrors);

  /* Print a warning if the redshift is too close for the hubble flow to be
     significant. This is done at the end because it is important and may
     be missed at the start of the program (before the outputs are
     printed). */
  if(p->redshift<MAIN_REDSHIFT_SIG_HUBBLE_FLOW && p->cp.quiet==0)
    error(EXIT_SUCCESS, 0, "[WARNING] at very low redshifts "
          "(approximately below %g), the peculiar velocity of the "
          "particular galaxy may be more significant than hubble's "
          "law (which is the basis of the measurements here). This "
          "gets worse as the redshift decreases. Therefore the "
          "results above may not be accurate on a per-object basis. "
          "You can suppress this warning with the '--quiet' option",
          MAIN_REDSHIFT_SIG_HUBBLE_FLOW);
}
