#
#				configure.ac
#
# Process this file with autoconf to generate a configure script.
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

# First, disable the annoying config.cache
define([AC_CACHE_LOAD],)
define([AC_CACHE_SAVE],)

# This is your standard Bertin source code...
AC_INIT(SelfServer, 0.4,[bertin@@iap.fr],
	selfserver, [https://github.com/ebertin/selfserver])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(autoconf)
AC_CONFIG_SRCDIR(src/makeit.c)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE
date=`date +%Y-%m-%d`
date1=`date -R`
date2=`date +"%a %b %d %Y"`
date3=`date +"%B %Y"`
AC_DEFINE_UNQUOTED(DATE, "$date", [Archive creation date])
AC_SUBST(PACKAGER, "Emmanuel Bertin")
AC_SUBST(DATE1, "$date1")
AC_SUBST(DATE2, "$date2")
AC_SUBST(DATE3, "$date3")

# Include macros
sinclude(acx_prog_cc_optim.m4)

# Provide a special option for setting the package release number
AC_ARG_WITH(release,
	[AS_HELP_STRING([--with-release=<release number>],
	[set the package release number (default = 1)])],
	[],
	[with_release="no"])
if test "$with_release" = "no"; then
  with_release=1
fi
AC_SUBST(PACKAGE_RELEASE, "$with_release")

# Display pakage and version number
AC_MSG_RESULT([******** Configuring:  $PACKAGE_NAME $PACKAGE_VERSION - $PACKAGE_RELEASE ($date) ********])

# Provide special option for choosing automatically the compilation flags.
AC_MSG_CHECKING([if compilation flags are set automatically])
AC_ARG_ENABLE(auto-flags,
	[AS_HELP_STRING([--enable-auto-flags],
	[Let the configure script choose the compilation flags (default = no)])],
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))

# Provide special option for the Linux Intel C compiler
AC_MSG_CHECKING([whether the INTEL compiler is enabled])
AC_ARG_ENABLE(icc,
	[AS_HELP_STRING([--enable-icc],
	[Use the Intel compiler (default = no)])],
        CC="icc"
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))

# Checks for programs.
AC_LANG(C)

AC_SEARCH_LIBS([strerror],[cposix])
if test "$enable_auto_flags" = "yes"; then
  CFLAGS=""
  LDFLAGS=""
  ACX_PROG_CC_OPTIM
fi
LT_INIT
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB(m, sin)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/mman.h sys/types.h unistd.h])
# Checks for INTEL math header files.
if test "$enable_icc" = "yes"; then
  AC_CHECK_HEADERS(mathimf.h)
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_LONG_LONG_INT
AC_TYPE_UNSIGNED_LONG_LONG_INT
AC_STRUCT_TM
AC_TYPE_UID_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([atexit getenv memcpy memmove memset mkdir munmap strstr \
sysconf setlinebuf])
AC_CHECK_FUNC([isnan], AC_DEFINE_UNQUOTED([HAVE_ISNAN2], 1,
                [Second isnan check]))
AC_CHECK_FUNC([isinf], AC_DEFINE_UNQUOTED([HAVE_ISINF2], 1,
                [Second isinf check]))

# Check support for large files
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

# Provide special option for profiling
AC_MSG_CHECKING([for profiler mode])
AC_ARG_ENABLE(profiling,
	[AS_HELP_STRING([--enable-profiling],
	[Enable special mode for profiling (default = no)])]
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))

# Enable linking options for making the executable as portable as possible.
AC_MSG_CHECKING([best linking option])
AC_ARG_ENABLE(best-link,
	[AS_HELP_STRING([--enable-best-link],
	[Choose the right combination of static and dynamic linking to make \
the executable as portable as possible (default = no)])],
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))


# Compile with profiling option
if test "$enable_profiling" = "yes"; then
  if test "$enable_icc" = "yes"; then
    AM_CFLAGS="$AM_CFLAGS -pq"
  else
    AM_CFLAGS="$AM_CFLAGS -pg"
  fi
  enable_best_link="no"
fi

# "Best" linking option
if test "$enable_best_link" = "yes"; then
  if test "$enable_icc" = "yes"; then
    AM_LDFLAGS="-static-intel -shared-libgcc \
	-static-libtool-libs -no-intel-extensions -avoid-version $AM_LDFLAGS"
  else
    AM_LDFLAGS="-shared-libgcc -static-libtool-libs -avoid-version $AM_LDFLAGS"
  fi
fi

AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_LDFLAGS)

# Display compiler and linker flags
AC_MSG_RESULT([***************************************************************])
AC_MSG_RESULT([Compile cmdline:  $CC $AM_CPPFLAGS $CPPFLAGS $AM_CFLAGS $CFLAGS])
AC_MSG_RESULT([Link    cmdline:  $CC $AM_LDFLAGS $LDFLAGS $LIBS])
AC_MSG_RESULT([Default XSLT URL: $xsl_url])
AC_MSG_RESULT([***************************************************************])

AC_CONFIG_FILES([
Makefile
selfserver.spec
debian/Makefile
debian/changelog
man/Makefile
man/selfserver.1
src/Makefile
src/fits/Makefile
src/wcs/Makefile
tests/Makefile
])
AC_OUTPUT
