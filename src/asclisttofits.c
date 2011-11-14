/*
*				asclisttofits.c
*
* Stand-along tool that converts ASCII files to FITS-LDAC binary format.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	SelfServer
*
*	Copyright:		(C) 2011 Emmanuel Bertin -- IAP/CNRS/UPMC
*
*	License:		GNU General Public License
*
*	SelfServer is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*	SelfServer is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with SelfServer. If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		14/11/2011
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fits/fitscat.h"
#include "asclisttofits.h"

#define		SYNTAX  "asclisttofits -o output_FITS_catalog input_ASCII_catalog\n"
extern const char	notokstr[];

/********************************** main ************************************/
int	main(int argc, char *argv[])

  {
   catstruct		*cat;
   tabstruct		*tab;
   keystruct		*key;
   FILE			*ascfile;
   unsigned short	ashort=1;
   char			ascname[MAXCHAR], fitsname[MAXCHAR], str[MAXCHAR],
			prefix[MAXCHAR], suffix[MAXCHAR], fullpath[MAXCHAR],
			*path, *buf;
   int			a,t, opt,opt2, maxlen, len,preflen,suflen;

  if (argc<2)
    {
    fprintf(OUTPUT, "\n                %s  Version %s (%s)\n",
		BANNER, MYVERSION, DATE);
    fprintf(OUTPUT, "\nFor information, please contact: %s\n", COPYRIGHT);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }

/* Test if byteswapping will be needed */
  bswapflag = *((char *)&ashort);

/* Default parameters */
  strcpy(prefix, "");
  strcpy(suffix, ".fits");
  for (a=1; a<argc; a++)
    {
    if (*(argv[a]) == '-')
      {
      opt = (int)argv[a][1];
      {
      opt2 = (int)tolower((int)argv[a][2]);
      if (opt == '-')
        {
        opt = opt2;
        opt2 = (int)tolower((int)argv[a][3]);
        }
      switch(opt)
        {
        case 'o':
          strcpy(fitsname, argv[++a]);
          break;
        case 'p':
          strcpy(prefix, argv[++a]);
          break;
        case 's':
          strcpy(suffix, argv[++a]);
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
          error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
          }
        }
      }
    else
      strcpy(ascname, argv[a]);
    }

  if (!(ascfile=fopen(ascname, "r")))
    error(EXIT_FAILURE,"*Error*: cannot open for reading ", ascname);
/* Find maximum path length */
  preflen = strlen(prefix);
  suflen = strlen(suffix);
  maxlen = 0;
  while (fgets(str, MAXCHAR, ascfile))
    {
    sscanf(str,"%s", fullpath);
    len=strlen(fullpath);
    path = fullpath;
    if (strstr(fullpath, prefix)==fullpath)
      {
      path += preflen;
      len -= preflen;
      }
    if (strstr(path, suffix)==path+len-suflen)
      len -= suflen;
    if (len>maxlen)
      maxlen = len;
    }
  rewind(ascfile);

  cat = new_cat(1);
  init_cat(cat);
  tab = new_tab("IMAGE_INDICES");
  strcpy(cat->filename, fitsname);
  if (open_cat(cat, WRITE_ONLY) != RETURN_OK)
    error(EXIT_FAILURE,"*Error*: cannot open for writing ", fitsname);
  QMALLOC(objkey[2].naxisn, int, objkey[2].naxis);
  objkey[2].naxisn[0] = maxlen;
  sprintf(objkey[2].printf, "%%%d%%s", maxlen);
  for (key=objkey; *key->name; key++)
    {
    key->nbytes = t_size[key->ttype]*(key->naxis? *key->naxisn : 1);
    add_key(key, tab, 0);
    }
  tab->cat = cat;
/* Write primary header */
  addkeywordto_head(cat->tab, "IMPREFIX", "Image filename prefix");
  fitswrite(cat->tab->headbuf, "IMPREFIX", prefix, H_STRING,T_STRING);
  addkeywordto_head(cat->tab, "IMSUFFIX", "Image filename suffix");
  fitswrite(cat->tab->headbuf, "IMSUFFIX", suffix, H_STRING,T_STRING);
  save_tab(cat, cat->tab);
/* Write entries */
  init_writeobj(cat, tab, &buf);
  while (fgets(str, MAXCHAR, ascfile))
    {
    sscanf(str,"%s %lf %lf", fullpath, &alpha,&delta);
    len=strlen(fullpath);
    path = fullpath;
    if (strstr(fullpath, prefix)==fullpath)
      {
      path += preflen;
      len -= preflen;
      }
    if (strstr(path, suffix)==path+len-suflen)
      {
      len -= suflen;
      path[len] = '\0';
      }
    strcpy(imagepath, path);
    write_obj(tab, buf);
    }
  fclose(ascfile);
  end_writeobj(cat, tab, buf);

  return EXIT_SUCCESS;
  }

