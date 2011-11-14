/*
*				gettiles.h
*
* Include file for gettiles.c
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
*	Last modified:		14/11/2011
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*--------------------------- structure definitions -------------------------*/
typedef struct tile
  {
  double		ra, dec;	/* RA,Dec coordinates */
  double		cdec;		/* cos(Dec) */
  char			*pathptr;	/* Pointer to image path */
  }	tilestruct;

typedef struct tilelist
  {
  tilestruct	*tile;
  int			ntile;
  int			*hash;
  char			*path;
  char			image_prefix[MAXCHAR];
  char			image_suffix[MAXCHAR];
  int			path_len;
  } tileliststruct;

/*---------------------------------- protos --------------------------------*/

tileliststruct		*tiles_load(char *filename, char *prefix, char *suffix);
char			**tiles_get(tileliststruct *tilelist,
				double ra, double dec, double radius);

void			tiles_end(tileliststruct *tilelist);

