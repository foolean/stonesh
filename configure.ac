#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([version_major], [3])
m4_define([version_minor], [2])
m4_define([version_micro], [1])
m4_define([version_micro_extra], version_micro)
m4_append([version_micro_extra], [])

AC_PREREQ([2.67])
AC_INIT([stonesh], [version_major.version_minor.version_micro_extra], [bennett@foolean.org])
AM_INIT_AUTOMAKE([stonesh], [version_major.version_minor.version_micro_extra])
AC_CONFIG_SRCDIR([stonesh.c])
AC_CONFIG_HEADERS([config.h])

# Get the current UTC date and time in RFC 2822 format.
AC_SUBST( DATE, $(date -Ru) )

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([ldap.h stdlib.h string.h syslog.h unistd.h GeoIP.h], ,AC_MSG_ERROR([missing header file]))

# Checks for libraries.
AC_CHECK_LIB([ldap], [ldap_sasl_bind], ,AC_MSG_ERROR([LDAP library not found]))
AC_CHECK_LIB([GeoIP], [GeoIP_country_code_by_addr], ,AC_MSG_ERROR([GeoIP library not found]))

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memmove memset strcasecmp strchr strdup strerror strpbrk strspn])

# Debian
AC_CHECK_PROG(HAVE_DPKG_BUILDPACKAGE, dpkg-buildpackage, "yes", "no")

# RPM
AC_CHECK_PROG(HAVE_RPMBUILD, rpmbuild, "yes", "no")

AC_PATH_PROG(POD2MAN, pod2man)
if test -z "$POD2MAN"; then
    AC_MSG_ERROR([pod2man required])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT(
    debian/changelog
    packaging/rpm/SPECS/stonesh.spec
)
