#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([stonesh], [3.1.0], [bennett@foolean.org])
AM_INIT_AUTOMAKE([stonesh], [3.1.0])
AC_CONFIG_SRCDIR([stonesh.c])
AC_CONFIG_HEADERS([config.h])

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

AC_PATH_PROG(POD2MAN, pod2man)
if test -z "$POD2MAN"; then
    AC_MSG_ERROR([pod2man required])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT(
	debian/changelog
)

