/*
*				getsdsstiles.c
*
* Get the list of SDSS image filenames that correspond to some part of the sky.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	SelfServer
*
*	Copyright:		(C) 2011 Emmanuel Bertin -- IAP/CNRS/UPMC
*
*	Author:			Emmanuel Bertin (IAP)
*
*	License:		GNU General Public License
*
*	SelfServer is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*	SelfServer is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with SelfServer.  If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		31/10/2011
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define.h"
#include "types.h"
#include "globals.h"
#include "fits/fitscat.h"
#include "fitswcs.h"
#include "getsdsstiles.h"

static int	sdsstiles_cmp(const void *sdsstile1, const void *sdsstile2);

/****** sdsstiles_get *********************************************************
PROTO	char **sdsstiles_get(sdsstileliststruct *sdsstilelist, double *pos,
			double radius, char *prefix, char *band)
PURPOSE	Return the list of filenames of SDSS tiles that overlap a selection
	disk.
INPUT	Pointer to the SDSS tile list,
	center RA coordinate of the selection disk [deg],
	center Dec coordinate of the selection disk [deg],
	selection radius [deg],
	directory prefix,
	band.
OUTPUT	Pointer to the list of filenames.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	31/10/2011
 ***/
char	**sdsstiles_get(sdsstileliststruct *sdsstilelist, double ra, double dec,
		double radius, char *prefix, char *band)
  {
   sdsstilestruct	*sdsstilet;
   double		decmin,decmax, radius2, dra,ddec;
   char			**filenames;
   int			f,i, istart, nmf, ntile;

  ntile = sdsstilelist->ntile;
  radius2 = radius*radius;
  decmin = dec-radius;
  decmax = dec+radius;
  if (dec<-90.0)
    dec = -90.0;
  if (dec>90.0)
    dec = 90.0;
  istart = sdsstilelist->hash[(int)(dec+90.0000001)];
  sdsstilet = &sdsstilelist->sdsstile[istart];
/* Search in both directions around the position indicated in the hash table */
  for (i=istart+1; i-- && sdsstilet->dec>decmin; sdsstilet--);
  sdsstilet++;
  f = 0;
  nmf = 1;
  QCALLOC(filenames, char *, MAXFILE);
  for (i=ntile-istart; i-- && sdsstilet->dec<decmax; sdsstilet++)
    {
    dra = (sdsstilet->ra - ra)*sdsstilet->cdec;
    ddec = sdsstilet->dec - dec;
    if (dra*dra+ddec*ddec<radius2)
      {
      if ((f+1)>=nmf*MAXFILE)	/* Keep one extra slot for a NULL pointer */
        {			/* which indicates the end of the array */
        nmf++;
        QREALLOC(filenames, char *, nmf*MAXFILE);
        }
      filenames[f++] = sdsstiles_filename(sdsstilet, prefix, band);
      }
    }

  return filenames;
  }


/****** sdsstiles_filename ****************************************************
PROTO	char *sdsstiles_filename(sdsstilestruct *sdsstile, char *prefix,
			char *band)
PURPOSE	Print an SDSS file name based
INPUT	Pointer to an SDSS tile,
	directory prefix,
	band.
OUTPUT	Pointer to an allocated character string containing the SDSS filename.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	31/10/2011
 ***/
char	*sdsstiles_filename(sdsstilestruct *sdsstile, char *prefix, char *band)
  {
   char	*filename;

  QMALLOC(filename, char, MAXCHAR);
  sprintf(filename, "%s/%d/%d/frame-%s-%06d-%d-%04d.fits.bz2",
	prefix,
	sdsstile->run,
	sdsstile->col,
	band,
	sdsstile->run,
	sdsstile->col,
	sdsstile->field);

  return filename;
  }


/****** sdsstiles_end *********************************************************
PROTO	void sdsstiles_end(sdsstileliststruct *sdsstilelist)
PURPOSE	Free memory allocated for an SDSS tile list.
INPUT	Pointer to the SDSS tile list.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	31/10/2011
 ***/
void	sdsstiles_end(sdsstileliststruct *sdsstilelist)
  {
  free(sdsstilelist->sdsstile);
  free(sdsstilelist->hash);
  free(sdsstilelist);

  return;
  }


/****** sdsstiles_cmp *********************************************************
PROTO	int sdsstiles_cmp(const void *sdsstile1, const void *sdsstile2)
PURPOSE	Compare the declination of two SDSS tiles (for sorting).
INPUT	Pointer to tile #1,
	pointer to tile #2.
OUTPUT	-1 if tile #1 has lower declination than tile #2, 0 if equal, and 1
	otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	28/10/2011
 ***/
static int	sdsstiles_cmp(const void *sdsstile1, const void *sdsstile2)
  {
   double	dec1, dec2;

  dec1 = ((sdsstilestruct*)sdsstile1)->dec;
  dec2 = ((sdsstilestruct*)sdsstile2)->dec;

  return dec1<dec2? -1 : (dec1==dec2? 0 : 1);
  }


/****** sdsstiles_load *********************************************************
PROTO	sdsstileliststruct *sdsstiles_load(char *filename)
PURPOSE	Load the SDSS catalog of tiles.
INPUT	SDSS Catalog filename (e.g. "tiOutputFinal.fit").
OUTPUT	Pointer to the tile list.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	31/10/2011
 ***/
sdsstileliststruct	*sdsstiles_load(char *filename)
  {
   sdsstileliststruct	*sdsstilelist;
   sdsstilestruct	*sdsstilet;
   catstruct		*cat;
   tabstruct		*tab;
   keystruct		*key;
   double		*ra,*dec,
			delta;
   unsigned int		*field, *run,
			*col, *rerun;
   int			*hash,
			i,j, ntile;

/* Load the catalog data */
  if (!(cat = read_cat(filename)))
    error(EXIT_FAILURE, "*Error*: cannot read ", filename);

  tab = cat->tab->nexttab;

  read_keys(tab, NULL, NULL, 0, NULL);

  if (!(key = name_to_key(tab, "ra")))
    error(EXIT_FAILURE, "*Error*: ra data not found in ", cat->filename);
  ra = key->ptr;
  if (!(key = name_to_key(tab, "dec")))
    error(EXIT_FAILURE, "*Error*: dec data not found in ", cat->filename);
  dec = key->ptr;
  if (!(key = name_to_key(tab, "run")))
    error(EXIT_FAILURE, "*Error*: run data not found in ", cat->filename);
  run = key->ptr;
  if (!(key = name_to_key(tab, "rerun")))
    error(EXIT_FAILURE, "*Error*: rerun data not found in ", cat->filename);
  rerun = key->ptr;
  if (!(key = name_to_key(tab, "col")))
    error(EXIT_FAILURE, "*Error*: col data not found in ", cat->filename);
  col = key->ptr;
  if (!(key = name_to_key(tab, "field")))
    error(EXIT_FAILURE, "*Error*: field data not found in ", cat->filename);
  field = key->ptr;

/* Copy the catalog data to a compact structure array */
  QMALLOC(sdsstilelist, sdsstileliststruct, 1);  
  sdsstilelist->ntile = tab->naxisn[1];
  QMALLOC(sdsstilelist->sdsstile, sdsstilestruct, sdsstilelist->ntile);
  sdsstilet = sdsstilelist->sdsstile;
  for (i=sdsstilelist->ntile; i--; sdsstilet++)
    {
    sdsstilet->ra = *(ra++);
    sdsstilet->dec = *(dec++);
    sdsstilet->cdec = cos(sdsstilet->dec*DEG);
    sdsstilet->run = *(run++);
    sdsstilet->rerun = *(rerun++);
    sdsstilet->col = *(col++);
    sdsstilet->field = *(field++);
    }

/* We don't need the FITS catalog anymore */
  free_cat(&cat, 1);

/* Sort by increasing declination */
  qsort(sdsstilelist->sdsstile, sdsstilelist->ntile, sizeof(sdsstilestruct),
	sdsstiles_cmp);

/* Build the hash table that contains the first tile in the sorted list */
/* which may be in reach from the current declination */
  QMALLOC(sdsstilelist->hash, int, 180);
  sdsstilet = sdsstilelist->sdsstile;
  hash = sdsstilelist->hash;
  delta = -90.0;
  j = 0;
  for (i=180; i--; delta+=1.0)
    {
/*-- For safety, we keep a 1-pixel margin */
    for (;j<ntile && sdsstilet->dec<=delta; j++, sdsstilet++);
    *(hash++) = j==ntile ? ntile-1 : j;
    }

  return sdsstilelist;
  }


