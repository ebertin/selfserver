#
#				Makefile.am
#
# src Makefile.am. Process this file with automake to generate a Makefile
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#
#	This file part of:	SelfServer
#
#	Copyright:		(C) 2011 Emmanuel Bertin -- IAP/CNRS/UPMC
#
#	License:		GNU General Public License
#
#	SelfServer is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
# 	(at your option) any later version.
#	SelfServer is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	You should have received a copy of the GNU General Public License
#	along with SelfServer.  If not, see <http://www.gnu.org/licenses/>.
#
#	Last modified:		31/10/2011
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SUBDIRS			= fits wcs
bin_PROGRAMS		= selfserver
selfserver_SOURCES	= fitswcs.c getsdsstiles.c main.c makeit.c prefs.c \
			  define.h diagnostic.h fitswcs.h getsdsstiles.h \
			  globals.h key.h prefs.h preflist.h types.h wcscelsys.h
selfserver_LDADD	= $(top_builddir)/src/fits/libfits.a \
			  $(top_builddir)/src/wcs/libwcs_c.a

DATE=`date +"%Y-%m-%d"`

