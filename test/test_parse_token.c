/*
 * test_parse_token.c
 *
 * This file is part of the regressions tests for stonesh.
 *
 * Copyright 2012 Bennett Samowich <bennett@foolean.org>
 *
 * Stonesh is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Stonesh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stonesh.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readconf.h"

char *progname = "test_parse_token";

int test_parse_token( char *keyword, int op )
{
    int result;

    printf( "   parse_token( '%s', 'not_needed', 0')", keyword );

    result = parse_token( keyword, "not_needed", 0 );
    printf( " = '%i' ", result );

    if ( result == op ) {
        printf( "passed\n" );
        return( 0 );
    }
    printf( "failed\n" );
    return( 1 );

}

int main()
{
    int errors = 0;
    int tests = 0;


    /* test the parse_token function */
    printf( "testing readconf.c:parse_token()\n" );

    tests++; errors += test_parse_token( "allowfqdn", oAllowFQDN );
    tests++; errors += test_parse_token( "allowFQDN", oAllowFQDN );
    tests++; errors += test_parse_token( "AllowFQDN", oAllowFQDN );
    tests++; errors += test_parse_token( "authorizedservicetag", oAuthorizedServiceTag );
    tests++; errors += test_parse_token( "AuthorizedServiceTag", oAuthorizedServiceTag );
    tests++; errors += test_parse_token( "debug", oDebug );
    tests++; errors += test_parse_token( "Debug", oDebug );
    tests++; errors += test_parse_token( "logtostderr", oLogToSTDERR );
    tests++; errors += test_parse_token( "LogToStderr", oLogToSTDERR );
    tests++; errors += test_parse_token( "useldap", oUseLDAP );
    tests++; errors += test_parse_token( "useLDAP", oUseLDAP );
    tests++; errors += test_parse_token( "UseLDAP", oUseLDAP );
    tests++; errors += test_parse_token( "proxykeyword", oProxyKeyword );
    tests++; errors += test_parse_token( "ProxyKeyword", oProxyKeyword );
    tests++; errors += test_parse_token( "ldapuri", oLDAPuri );
    tests++; errors += test_parse_token( "LdapUri", oLDAPuri );
    tests++; errors += test_parse_token( "LDAPURI", oLDAPuri );
    tests++; errors += test_parse_token( "ldapbase", oLDAPbase );
    tests++; errors += test_parse_token( "LdapBase", oLDAPbase );
    tests++; errors += test_parse_token( "LDAPBase", oLDAPbase );
    tests++; errors += test_parse_token( "hostsfile", oHostsFile );
    tests++; errors += test_parse_token( "HostsFile", oHostsFile );
    tests++; errors += test_parse_token( "usersfile", oUsersFile );
    tests++; errors += test_parse_token( "UsersFile", oUsersFile );
    tests++; errors += test_parse_token( "enableproxy", oEnableProxy );
    tests++; errors += test_parse_token( "EnableProxy", oEnableProxy );
    tests++; errors += test_parse_token( "disableauthorization", oDisableAuthorization );
    tests++; errors += test_parse_token( "disableAuthorization", oDisableAuthorization );
    tests++; errors += test_parse_token( "DisableAuthorization", oDisableAuthorization );
    tests++; errors += test_parse_token( "sshpath", oSSHPath );
    tests++; errors += test_parse_token( "SSHPath", oSSHPath );
    tests++; errors += test_parse_token( "ncpath", oNCPath );
    tests++; errors += test_parse_token( "NCPath", oNCPath );
    tests++; errors += test_parse_token( "nctimeout", oNCTimeout );
    tests++; errors += test_parse_token( "NCTimeout", oNCTimeout );
    tests++; errors += test_parse_token( "geoip", oGeoIP );
    tests++; errors += test_parse_token( "GeoIP", oGeoIP );
    tests++; errors += test_parse_token( "geoip6", oGeoIP6 );
    tests++; errors += test_parse_token( "GeoIP6", oGeoIP6 );
    tests++; errors += test_parse_token( "XsdfkhSj12", oBadOption );

    printf( "\nreadconf.c:parse_token(): passed %i out of %i tests\n", ( tests - errors ), tests );
    return( errors );
}
