/*
*				gettiles.c
*
* Get the list of  image filenames that correspond to some part of the sky.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	SelfServer
*
*	Copyright:		(C) 2011-2013 Emmanuel Bertin -- IAP/CNRS/UPMC
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
*	Last modified:		18/03/2013
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
#include "gettiles.h"

static int	tiles_cmp(const void *tile1, const void *tile2);

/****** tiles_get *********************************************************
PROTO	char **tiles_get(tileliststruct *tilelist, double *pos,
			double radius)
PURPOSE	Return the list of filenames of tiles that overlap a selection disk.
INPUT	Pointer to the tile list,
	center RA coordinate of the selection disk [deg],
	center Dec coordinate of the selection disk [deg],
	selection radius [deg].
OUTPUT	Pointer to the list of filenames.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	18/03/2013
 ***/
char	**tiles_get(tileliststruct *tilelist, double ra, double dec,
		double radius)
  {
   tilestruct	*tilet;
   double	decmin,decmax, radrad, cdec0,sdec0;
   char		format[MAXCHAR],
		**filenames,
		*filename;
   int		f,i, istart, nmf, ntile, pathlen;

  ntile = tilelist->ntile;
  pathlen = tilelist->path_len;
  cdec0 = cos(dec*DEG);
  sdec0 = sin(dec*DEG);
  radrad=radius*DEG;
  decmin = dec-radius;
  decmax = dec+radius;
  if (decmin<-90.0)
    decmin = -90.0;
  if (decmax>90.0)
    decmax = 90.0;
  istart = tilelist->hash[(int)(dec+90.0000001)];
  tilet = &tilelist->tile[istart++];
/* Search in both directions around the position indicated in the hash table */
  for (; istart-- && tilet->dec>decmin; tilet--);
  tilet++;
  istart++;
  f = 0;
  nmf = 1;
  QCALLOC(filenames, char *, MAXFILE);
  sprintf(format, "%s/%%.%ds%s", tilelist->image_prefix, pathlen,
				tilelist->image_suffix);
  for (i=ntile-istart; i-- && tilet->dec<decmax; tilet++)
    {
   if (acos(sdec0*sin(tilet->dec*DEG)
	+cdec0*cos(tilet->dec*DEG)*cos((tilet->ra - ra)*DEG))<radrad)
      {
      if ((f+1)>=nmf*MAXFILE)	/* Keep one extra slot for a NULL pointer */
        {			/* which indicates the end of the array */
        nmf++;
        QREALLOC(filenames, char *, nmf*MAXFILE);
        }
      QMALLOC(filename, char, MAXCHAR);
      sprintf(filename, format, tilet->pathptr);
      filenames[f++] = filename;
      }
    }

  return filenames;
  }

/****** tiles_end *********************************************************
PROTO	void tiles_end(tileliststruct *tilelist)
PURPOSE	Free memory allocated for a tile list.
INPUT	Pointer to the tile list.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	14/11/2011
 ***/
void	tiles_end(tileliststruct *tilelist)
  {
  free(tilelist->tile);
  free(tilelist->hash);
  free(tilelist->path);
  free(tilelist);

  return;
  }


/****** tiles_cmp *********************************************************
PROTO	int tiles_cmp(const void *tile1, const void *tile2)
PURPOSE	Compare the declination of two tiles (for sorting).
INPUT	Pointer to tile #1,
	pointer to tile #2.
OUTPUT	-1 if tile #1 has lower declination than tile #2, 0 if equal, and 1
	otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	28/10/2011
 ***/
static int	tiles_cmp(const void *tile1, const void *tile2)
  {
   double	dec1, dec2;

  dec1 = ((tilestruct*)tile1)->dec;
  dec2 = ((tilestruct*)tile2)->dec;

  return dec1<dec2? -1 : (dec1==dec2? 0 : 1);
  }


/****** tiles_load *********************************************************
PROTO	tileliststruct *tiles_load(char *filename)
PURPOSE	Load the catalog of tiles.
INPUT	Catalog filename (e.g. "sdss-r8_g.fits),
	image path prefix,
	image filename suffix.
OUTPUT	Pointer to the tile list.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	11/02/2013
 ***/
tileliststruct	*tiles_load(char *filename, char *prefix, char *suffix)
  {
   tileliststruct	*tilelist;
   tilestruct		*tilet;
   catstruct		*cat;
   tabstruct		*tab;
   keystruct		*key;
   char			*path;
   double		*ra,*dec,
			delta;
   unsigned int		*hash;
   int			i,j, ntile, pathlen;

/* Load the catalog data */
  if (!(cat = read_cat(filename)))
    error(EXIT_FAILURE, "*Error*: cannot read ", filename);

  tab = cat->tab->nexttab;

  read_keys(tab, NULL, NULL, 0, NULL);

  if (!(key = name_to_key(tab, "ALPHA_J2000")))
    error(EXIT_FAILURE, "*Error*: ra data not found in ", cat->filename);
  ra = key->ptr;
  if (!(key = name_to_key(tab, "DELTA_J2000")))
    error(EXIT_FAILURE, "*Error*: dec data not found in ", cat->filename);
  dec = key->ptr;
  if (!(key = name_to_key(tab, "IMAGE_PATH")))
    error(EXIT_FAILURE, "*Error*: run data not found in ", cat->filename);
  path = key->ptr;
  key->ptr = NULL;	/* We don't want the filename list to vanish later on */
  pathlen = key->naxisn[0];

/* Create the tile list */
  QCALLOC(tilelist, tileliststruct, 1);  
  if (prefix && *prefix)
    strcpy(tilelist->image_prefix, prefix);
  else
    fitsread(cat->tab->headbuf, "IMPREFIX", tilelist->image_prefix,
		H_STRING, T_STRING);
  if (suffix && *suffix)
    strcpy(tilelist->image_suffix, suffix);
  else
    fitsread(cat->tab->headbuf, "IMSUFFIX", tilelist->image_suffix,
		H_STRING, T_STRING);

  tilelist->path_len = pathlen;
  tilelist->path = path;

/* Copy the catalog data to a compact structure array */
  tilelist->ntile = tab->naxisn[1];
  QMALLOC(tilelist->tile, tilestruct, tilelist->ntile);
  tilet = tilelist->tile;
  for (i=tilelist->ntile; i--; tilet++)
    {
    tilet->ra = *(ra++);
    tilet->dec = *(dec++);
    tilet->pathptr = path; path += pathlen;
    }

/* We don't need the FITS catalog anymore (but we keep the list of filenames */
  free_cat(&cat, 1);

/* Sort by increasing declination */
  qsort(tilelist->tile, tilelist->ntile, sizeof(tilestruct), tiles_cmp);

/* Build the hash table that contains the first tile in the sorted list */
/* which may be in reach from the current declination */
  QMALLOC(tilelist->hash, unsigned int, 180);
  tilet = tilelist->tile;
  hash = tilelist->hash;
  delta = -90.0;
  j = 0;
  for (i=180; i--; delta+=1.0)
    {
/*-- For safety, we keep a 1-pixel margin */
    for (;j<ntile && tilet->dec<=delta; j++, tilet++);
    *(hash++) = j==ntile ? ntile-1 : j;
    }

  return tilelist;
  }


