#
#				Makefile.am
#
# Main Makefile. Process this file with automake to generate a Makefile
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

AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src
EXTRA_DIST		= data AUTHORS BUGS ChangeLog \
			  COPYRIGHT HISTORY INSTALL LICENSE README THANKS \
			  acx_prog_cc_optim.m4
RPM_ROOTDIR		= `rpmbuild --nobuild -E %_topdir`
RPM_SRCDIR		= $(RPM_ROOTDIR)/SOURCES
dist-hook:
	rm -rf `find $(distdir) -name .svn`

rpm:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-icc:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_ICC="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-best:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_BEST="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec


