
/*
*				preflist.h
*
* Configuration keyword definitions.
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

#include "key.h"

#ifdef  USE_THREADS
#define	THREADS_PREFMAX	THREADS_NMAX
#else
#define	THREADS_PREFMAX	65535
#endif

/*-------------------------------- initialization ---------------------------*/
int	idummy;

pkeystruct key[] =
 {
  {"NTHREADS", P_INT, &prefs.nthreads, 0, THREADS_PREFMAX},
  {"SDSS_BAND", P_STRING, prefs.sdss_band},
  {"SDSS_PATH", P_STRING, prefs.sdss_path},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","LOG","FULL",""}},
  {""}
 };

char		keylist[sizeof(key)/sizeof(pkeystruct)][32];
const char	notokstr[] = {" \t=,;\n\r\""};

char *default_prefs[] =
 {
"# Default configuration file for " BANNER " " MYVERSION,
"# EB " DATE,
"#",
" ",
"#--------------------------------- SDSS --------------------------------------",
" ",
"SDSS_PATH       groups/boss/photoObj/frames/301  # Root path to SDSS data",
"SDSS_BAND       g                                # SDSS band",
" ",
"#----------------------------- Miscellaneous ---------------------------------",
" ",
"VERBOSE_TYPE    NORMAL          # can be QUIET,NORMAL,LOG or FULL",
"*                                # Filename for XSL style-sheet",
#ifdef USE_THREADS
"NTHREADS        0               # Number of simultaneous threads for",
"                                # the SMP version of " BANNER,
"                                # 0 = automatic",
#else
"NTHREADS        1               # 1 single thread",
#endif
" ",
""
 };

