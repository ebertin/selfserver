/*
*				getsdsstiles.h
*
* Include file for getsdsstiles.c
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

typedef struct sdsstile
  {
  double		ra, dec;	/* RA,Dec coordinates */
  double		cdec;		/* cos(Dec) */
  unsigned short	run;		/* SDSS run */
  unsigned char		rerun;		/* SDSS processing rerun */
  unsigned char		col;		/* SDSS camcol */
  unsigned short	field;		/* SDSS field */
  }	sdsstilestruct;

typedef struct sdsstilelist
  {
  sdsstilestruct	*tile;
  int			ntile;
  int			*hash;
  } sdsstileliststruct;
