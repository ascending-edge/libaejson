m4_define(libaejson_version_triplet, m4_esyscmd_s([cat version.txt]))
m4_define(libaejson_version_list, m4_split(libaejson_version_triplet, [[.]]))
m4_define(libaejson_version_major, m4_argn(1, libaejson_version_list))
m4_define(libaejson_version_minor, m4_argn(2, libaejson_version_list))
m4_define(libaejson_version_age, m4_argn(3, libaejson_version_list))

AC_INIT([libaejson], [libaejson_version_triplet])
AC_PREREQ(2.69)
AC_COPYRIGHT([Copyright (C) 2017 Ascending Edge, LLC.])

AC_SUBST([libaejson_MAJOR], libaejson_version_major)
AC_SUBST([libaejson_MINOR], libaejson_version_minor)
AC_SUBST([libaejson_AGE], libaejson_version_age)
AC_DEFINE([libaejson_MAJOR], libaejson_version_major, [Major Version])
AC_DEFINE([libaejson_MINOR], libaejson_version_minor, [Minor Version])
AC_DEFINE([libaejson_AGE], libaejson_version_age, [Age])

AC_USE_SYSTEM_EXTENSIONS
AM_PROG_AR
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_YACC
AC_PROG_LEX
# Checks for typedefs, structures, and compiler characteristics.
AC_CONFIG_FILES([Makefile libaejson/Makefile test/Makefile])
AM_SILENT_RULES([yes])
AC_OUTPUT


