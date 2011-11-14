/*
*				main.c
*
* Command line parsing.
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

#ifdef HAVE_CONFIG_H
#include        "config.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "define.h"
#include "types.h"
#include "globals.h"
#include "fits/fitscat.h"
#include "prefs.h"
#include "gettiles.h"

#define		SYNTAX \
EXECUTABLE " <RA,<Dec>,<Radius>\n" \
"\t\t[-c <config_file>][-<keyword> <value>]\n" \
"> to dump a default configuration file: " EXECUTABLE " -d \n" \
"> to dump a default extended configuration file: " EXECUTABLE " -dd \n"

extern const char       notokstr[];

/********************************** main ************************************/

int main(int argc, char *argv[])
  {
   char			**argkey, **argval,
			*str,*listbuf,*ptr;
   int			a, narg, nim, ntok, opt, opt2;

   tileliststruct	*tilelist;
   double		alpha,delta, radius;
   char			**filelist,
			**list;


#ifdef HAVE_SETLINEBUF
/* flush output buffer at each line */
  setlinebuf(stderr);
#endif

  if (argc<1)
    {
    fprintf(OUTPUT, "\n         %s  version %s (%s)\n", BANNER,MYVERSION,DATE);
    fprintf(OUTPUT, "\nWritten by %s\n", AUTHORS);
    fprintf(OUTPUT, "Copyright %s\n", COPYRIGHT);
    fprintf(OUTPUT, "\nvisit %s\n", WEBSITE);
    fprintf(OUTPUT, "\n%s\n", DISCLAIMER);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }

  QMALLOC(argkey, char *, argc);
  QMALLOC(argval, char *, argc);

/* Default parameters */
  prefs.command_line = argv;
  prefs.ncommand_line = argc;
  narg = 0;
  prefs.ntilelist = 0;
  listbuf = (char *)NULL;
  strcpy(prefs.prefs_name, "/etc/selfserver/selfserver.conf");

  for (a=1; a<argc; a++)
    {
    if (*(argv[a]) == '-')
      {
      opt = (int)argv[a][1];
      if (strlen(argv[a])<4 || opt == '-')
        {
        opt2 = (int)tolower((int)argv[a][2]);
        if (opt == '-')
          {
          opt = opt2;
          opt2 = (int)tolower((int)argv[a][3]);
          }
        switch(opt)
          {
          case 'c':
            if (a<(argc-1))
              strcpy(prefs.prefs_name, argv[++a]);
            break;
          case 'd':
            dumpprefs(opt2=='d' ? 1 : 0);
            exit(EXIT_SUCCESS);
            break;
          case 'v':
            printf("%s version %s (%s)\n", BANNER,MYVERSION,DATE);
            exit(EXIT_SUCCESS);
            break;
          case 'h':
            fprintf(OUTPUT, "\nSYNTAX: %s", SYNTAX);
            exit(EXIT_SUCCESS);
            break;
          default:
            error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
          }
        }
      else
        {
/*------ Config parameters */
        argkey[narg] = &argv[a][1];
        argval[narg++] = argv[++a];
        }       
      }
    else
      {
/*---- input coordinates */
      str=strtok_r(argv[a], ";,", &ptr);
      if (!str)
        error(EXIT_FAILURE, "*Error*: incorrect input coordinates/radius", "");
      alpha=atof(str);
      str=strtok_r(NULL, ";,", &ptr);
      if (!str)
        error(EXIT_FAILURE, "*Error*: incorrect input coordinates/radius", "");
      delta=atof(str);
      str=strtok_r(NULL, ";,", &ptr);
      if (!str)
        error(EXIT_FAILURE, "*Error*: incorrect input coordinates/radius", "");
      radius=atof(str);
      }
    }

  readprefs(prefs.prefs_name, argkey, argval, narg);
  useprefs();
  free(argkey);
  free(argval);

  tilelist = tiles_load(prefs.image_list,prefs.image_prefix,prefs.image_suffix);
  filelist = tiles_get(tilelist, alpha,delta, radius);
  for (list=filelist; *list; list++)
    {
    printf("%s\n", *list);
    free(*list);
    }
  free(filelist);
  tiles_end(tilelist);

//  makeit();

  free(listbuf);

  NFPRINTF(OUTPUT, "");
  NPRINTF(OUTPUT, "> All done (in %.1f s)\n", prefs.time_diff);

  exit(EXIT_SUCCESS);
  }
