# $Id: configure.in,v 1.1.1.1 2005/04/04 23:23:31 rdmp1c Exp $

dnl Process this file with autoconf to produce a configure script

AC_INIT(forest, 0.1)
AC_CONFIG_SRCDIR(.)
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_CC
AC_PROG_INSTALL
#AC_ARG_PROGRAM

# We need ranlib to make the library, and we need to define make
AC_PROG_RANLIB
AC_PROG_MAKE_SET

# Check for GTL
AC_CHECK_HEADERS(GTL/GTL.h)

AC_OUTPUT(Makefile graph/Makefile )

