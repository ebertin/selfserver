#
#				Makefile.am
#
# src Makefile.am. Process this file with automake to generate a Makefile
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#
#	This file part of:	SelfServer
#
#	Copyright:		(C) 2011-2018 IAP/CNRS/UPMC
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
#	Last modified:		23/03/2018
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SUBDIRS			= fits wcs
bin_PROGRAMS		= selfserver asclisttofits
asclisttofits_SOURCES	= asclisttofits.c asclisttofits.h
selfserver_SOURCES	= fitswcs.c gettiles.c main.c makeit.c prefs.c \
			  define.h fitswcs.h gettiles.h \
			  globals.h key.h prefs.h preflist.h types.h wcscelsys.h
selfserver_LDADD	= $(srcdir)/fits/libfits.a \
			  $(srcdir)/wcs/libwcs_c.a
asclisttofits_LDADD	= $(srcdir)/fits/libfits.a

DATE=`date +"%Y-%m-%d"`

