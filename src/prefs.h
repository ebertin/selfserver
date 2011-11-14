/*
*				prefs.h
*
* Include file for prefs.c.
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

#ifndef _PREFS_H_
#define _PREFS_H_

/*----------------------------- Internal constants --------------------------*/

#define	MAXCHARL	16384		/* max. nb of chars in a string list */
#define	MAXLIST		64		/* max. nb of list members */
#define	MAXLISTSIZE	2000000		/* max size of list */

/* NOTES:
One must have:	MAXLIST >= 1 (preferably >= 16!)
*/
/*------------------------------- preferences -------------------------------*/
typedef struct
  {
  char		**command_line;			/* Command line */
  int		ncommand_line;			/* nb of params */
  char		prefs_name[MAXCHAR];		/* prefs filename */
  char		*(tilelist_name[MAXFILE]);	/* Filename(s) of tile list(s)*/
  int		ntilelist;			/* Number of tile lists */
  char		image_list[MAXCHAR];		/* Image list */
  char		image_prefix[MAXCHAR];		/* Image root path */
  char		image_suffix[MAXCHAR];		/* Image extension */
/* Multithreading */
  int		nthreads;			/* Number of active threads */
/* Misc */
  enum {QUIET, NORM, LOG, FULL}	verbose_type;	/* How much it displays info */
  char		sdate_start[12];		/* Start date */
  char		stime_start[12];		/* Start time */
  char		sdate_end[12];			/* End date */
  char		stime_end[12];			/* End time */
  double	time_diff;			/* Execution time */
  }	prefstruct;

  prefstruct		prefs;

/*-------------------------------- protos -----------------------------------*/
extern char	*list_to_str(char *listname);

extern int	cistrcmp(char *cs, char *ct, int mode);

extern void	dumpprefs(int state),
		readprefs(char *filename,char **argkey,char **argval,int narg),
		useprefs(void);
#endif
