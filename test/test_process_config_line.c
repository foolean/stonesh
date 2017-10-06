/*
 * test_process_config_line.c
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

char *progname = "test_process_config_line";

Options options;

int test_process_config_line_char( char *line, char **var, char *val, int out )
{
    int active;
    int error;
    char *l;

    l = (char*)malloc(strlen(line) + 1);
    memset(l, 0, strlen(line) + 1);
    strncpy(l, line, strlen(line));

    active = 1;
    initialize_options( &options );
    printf( "process_config_line(options, '%s', 'not_needed, 0, &active);", l);
    error = process_config_line(&options, l, "not_needed", 0, &active);
    if ( var != NULL ) {
        printf( " = '%s' (%i) ", *var, error );
    } else {
        printf( " = 'N/A' (%i) ", error );
    }
    free(l);
    fill_default_options(&options);

    if ( error == out ) {
        if ( var != NULL ) {
            if ( strlen(*var) == strlen(val) ) {
                if ( strcasecmp(*var, val) == 0 ) {
                    free_options(&options);
                    printf( "passed\n");
                    return(0);
                }
            } else {
                free_options(&options);
                printf( "passed\n");
                return(0);
            }
        }
    }

    free_options(&options);
    printf( "failed\n");
    return(1);
}

int test_process_config_line_int( char *line, int *var, int val, int out )
{
    int active;
    int error;
    char *l;

    l = (char*)malloc(strlen(line) + 1);
    memset(l, 0, strlen(line) + 1);
    strncpy(l, line, strlen(line));

    active = 1;
    initialize_options( &options );
    printf( "process_config_line(options, '%s', 'not_needed, 0, &active);", l);
    error = process_config_line(&options, l, "not_needed", 0, &active);
    if ( var != NULL ) {
        printf( " = '%i' (%i) ", *var, error );
    } else {
        printf( " = 'N/A' (%i) ", error );
    }
    free(l);
    fill_default_options(&options);

    if ( error == out ) {
        if ( var != NULL ) {
            if ( *var == val ) { 
                free_options(&options);
                printf( "passed\n");
                return(0);
            }
        } else {
            free_options(&options);
            printf( "passed\n");
            return(0);
        }
    }

    free_options(&options);
    printf( "failed\n");
    return(1);
}

int main()
{
    int errors = 0;
    int tests = 0;


    /* test the process_config_line function */
    printf( "testing readconf.c:process_config_line()\n" );

    tests++; errors += test_process_config_line_int( "useldap false", &options.use_ldap, 0, 0);
    tests++; errors += test_process_config_line_int( "useLDAP false", &options.use_ldap, 0, 0);
    tests++; errors += test_process_config_line_int( " useLDAP false", &options.use_ldap, 0, 0);
    tests++; errors += test_process_config_line_int( "useldap true", &options.use_ldap, 1, 0);
    tests++; errors += test_process_config_line_int( "useLDAP true", &options.use_ldap, 1, 0);
    tests++; errors += test_process_config_line_int( " useLDAP true", &options.use_ldap, 1, 0);
    tests++; errors += test_process_config_line_int( "useldap foolean", &options.use_ldap, 0, -1);
    tests++; errors += test_process_config_line_int( "useLDAP foolean", &options.use_ldap, 0, -1);
    tests++; errors += test_process_config_line_int( " useLDAP foolean", &options.use_ldap, 0, -1);
    tests++; errors += test_process_config_line_int( "debug false", &options.debug, 0, 0);
    tests++; errors += test_process_config_line_int( "Debug false", &options.debug, 0, 0);
    tests++; errors += test_process_config_line_int( " Debug false", &options.debug, 0, 0);
    tests++; errors += test_process_config_line_int( "debug true", &options.debug, 1, 0);
    tests++; errors += test_process_config_line_int( "Debug true", &options.debug, 1, 0);
    tests++; errors += test_process_config_line_int( " Debug true", &options.debug, 1, 0);
    tests++; errors += test_process_config_line_int( "debug foolean", &options.debug, 0, -1);
    tests++; errors += test_process_config_line_int( "Debug foolean", &options.debug, 0, -1);
    tests++; errors += test_process_config_line_int( " Debug foolean", &options.debug, 0, -1);
    tests++; errors += test_process_config_line_char( "authorizedservicetag proxy", &options.authorized_service_tag, "proxy", 0);
    tests++; errors += test_process_config_line_char( "authorizedServiceTag proxy", &options.authorized_service_tag, "proxy", 0);
    tests++; errors += test_process_config_line_char( " authorizedServiceTag proxy", &options.authorized_service_tag, "proxy", 0);
    tests++; errors += test_process_config_line_int( "enableproxy false", &options.enable_proxy, 0, 0);
    tests++; errors += test_process_config_line_int( "enableProxy false", &options.enable_proxy, 0, 0);
    tests++; errors += test_process_config_line_int( " enableProxy false", &options.enable_proxy, 0, 0);
    tests++; errors += test_process_config_line_int( "enableproxy true", &options.enable_proxy, 1, 0);
    tests++; errors += test_process_config_line_int( "enableProxy true", &options.enable_proxy, 1, 0);
    tests++; errors += test_process_config_line_int( " enableProxy true", &options.enable_proxy, 1, 0);
    tests++; errors += test_process_config_line_int( "enableproxy foolean", &options.enable_proxy, 0, -1);
    tests++; errors += test_process_config_line_int( "enableProxy foolean", &options.enable_proxy, 0, -1);
    tests++; errors += test_process_config_line_int( " enableProxy foolean", &options.enable_proxy, 0, -1);
    tests++; errors += test_process_config_line_char( "proxykeyword direct", &options.proxy_keyword, "direct", 0);
    tests++; errors += test_process_config_line_char( "proxyKeyword direct", &options.proxy_keyword, "direct", 0);
    tests++; errors += test_process_config_line_char( " proxyKeyword direct", &options.proxy_keyword, "direct" ,0);
    tests++; errors += test_process_config_line_char( "ldapuri ldap://localhost", &options.ldap_uri, "ldap://localhost", 0);
    tests++; errors += test_process_config_line_char( "LDAPuri ldap://localhost", &options.ldap_uri, "ldap://localhost", 0);
    tests++; errors += test_process_config_line_char( " LDAPuri ldap://localhost", &options.ldap_uri, "ldap://localhost", 0);
    tests++; errors += test_process_config_line_char( "ldapbase \"dc=padl,dc=com\"", &options.ldap_base, "\"dc=padl,dc=com\"", 0);
    tests++; errors += test_process_config_line_char( "LDAPbase \"dc=padl,dc=com\"", &options.ldap_base, "\"dc=padl,dc=com\"", 0);
    tests++; errors += test_process_config_line_char( " LDAPbase \"dc=padl,dc=com\"", &options.ldap_base, "\"dc=padl,dc=com\"", 0);
    tests++; errors += test_process_config_line_char( "hostsfile /etc/stonesh/hosts.lst", &options.hosts_file, "/etc/stonesh/hosts.lst", 0);
    tests++; errors += test_process_config_line_char( "HostsFile /etc/stonesh/hosts.lst", &options.hosts_file, "/etc/stonesh/hosts.lst", 0);
    tests++; errors += test_process_config_line_char( " HostsFile /etc/stonesh/hosts.lst", &options.hosts_file, "/etc/stonesh/hosts.lst", 0);
    tests++; errors += test_process_config_line_int( "allowfqdn false", &options.allowfqdn, 0, 0);
    tests++; errors += test_process_config_line_int( "AllowFQDN false", &options.allowfqdn, 0, 0);
    tests++; errors += test_process_config_line_int( " AllowFQDN false", &options.allowfqdn, 0, 0);
    tests++; errors += test_process_config_line_int( "allowfqdn true", &options.allowfqdn, 1, 0);
    tests++; errors += test_process_config_line_int( "AllowFQDN true", &options.allowfqdn, 1, 0);
    tests++; errors += test_process_config_line_int( " AllowFQDN true", &options.allowfqdn, 1, 0);
    tests++; errors += test_process_config_line_int( "allowfqdn foolean", &options.allowfqdn, 0, -1);
    tests++; errors += test_process_config_line_int( "AllowFQDN foolean", &options.allowfqdn, 0, -1);
    tests++; errors += test_process_config_line_int( " AllowFQDN foolean", &options.allowfqdn, 0, -1);
    tests++; errors += test_process_config_line_char( "usersfile /etc/stonesh/users.lst", &options.users_file, "/etc/stonesh/users.lst", 0);
    tests++; errors += test_process_config_line_char( "UsersFile /etc/stonesh/users.lst", &options.users_file, "/etc/stonesh/users.lst", 0);
    tests++; errors += test_process_config_line_char( " UsersFile /etc/stonesh/users.lst", &options.users_file, "/etc/stonesh/users.lst", 0);
    tests++; errors += test_process_config_line_int( "disableauthorization false", &options.disable_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "disableAuthorization false", &options.disable_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( " disableAuthorization false", &options.disable_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "disableauthorization true", &options.disable_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "disableAuthorization true", &options.disable_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( " disableAuthorization true", &options.disable_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "disableauthorization foolean", &options.disable_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( "disableAuthorization foolean", &options.disable_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( " disableAuthorization foolean", &options.disable_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( "disableproxyauthorization false", &options.disable_proxy_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "disableproxyAuthorization false", &options.disable_proxy_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "disableProxyAuthorization false", &options.disable_proxy_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( " disableProxyAuthorization false", &options.disable_proxy_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "DisableProxyAuthorization false", &options.disable_proxy_authorization, 0, 0);
    tests++; errors += test_process_config_line_int( "disableproxyauthorization true", &options.disable_proxy_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "disableproxyAuthorization true", &options.disable_proxy_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "disableProxyAuthorization true", &options.disable_proxy_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( " disableProxyAuthorization true", &options.disable_proxy_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "DisableProxyAuthorization true", &options.disable_proxy_authorization, 1, 0);
    tests++; errors += test_process_config_line_int( "disableproxyauthorization foolean", &options.disable_proxy_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( "disableproxyAuthorization foolean", &options.disable_proxy_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( "disableProxyAuthorization foolean", &options.disable_proxy_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( " disableProxyAuthorization foolean", &options.disable_proxy_authorization, 0, -1);
    tests++; errors += test_process_config_line_int( "DisableProxyAuthorization foolean", &options.disable_proxy_authorization, 0, -1);
    tests++; errors += test_process_config_line_char( "sshpath /usr/bin/ssh", &options.ssh_path, "/usr/bin/ssh", 0);
    tests++; errors += test_process_config_line_char( "sshPath /usr/bin/ssh", &options.ssh_path, "/usr/bin/ssh", 0);
    tests++; errors += test_process_config_line_char( " sshPath /usr/bin/ssh", &options.ssh_path, "/usr/bin/ssh", 0);
    tests++; errors += test_process_config_line_char( "ncpath /bin/nc", &options.nc_path, "/bin/nc", 0);
    tests++; errors += test_process_config_line_char( "ncPath /bin/nc", &options.nc_path, "/bin/nc", 0);
    tests++; errors += test_process_config_line_char( " ncPath /bin/nc", &options.nc_path, "/bin/nc", 0);
    tests++; errors += test_process_config_line_char( "nctimeout 1", &options.nc_timeout, "1", 0);
    tests++; errors += test_process_config_line_char( "ncTimeout 1", &options.nc_timeout, "1", 0);
    tests++; errors += test_process_config_line_char( " ncTimeout 1", &options.nc_timeout, "1", 0);
    tests++; errors += test_process_config_line_char( "geoip /usr/share/GeoIP/GeoIP.dat", &options.geoip_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_char( "GeoIP /usr/share/GeoIP/GeoIP.dat", &options.geoip_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_char( " GeoIP /usr/share/GeoIP/GeoIP.dat", &options.geoip_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_char( "geoip6 /usr/share/GeoIP/GeoIP.dat", &options.geoip6_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_char( "GeoIP6 /usr/share/GeoIP/GeoIP.dat", &options.geoip6_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_char( " GeoIP6 /usr/share/GeoIP/GeoIP.dat", &options.geoip6_dat, "/share/GeoIP/GeoIP.dat", 0);
    tests++; errors += test_process_config_line_int( "usesql true", NULL, 1, 1);
    tests++; errors += test_process_config_line_int( "useSQL true", NULL, 1, 1);
    tests++; errors += test_process_config_line_int( " useSQL true", NULL, 1, 1);
    tests++; errors += test_process_config_line_int( "usesql false", NULL, 0, 1);
    tests++; errors += test_process_config_line_int( "useSQL false", NULL, 0, 1);
    tests++; errors += test_process_config_line_int( " useSQL false", NULL, 0, 1);

    printf( "\nreadconf.c:process_config_line(): passed %i out of %i tests\n", ( tests - errors ), tests );
    return( errors );
}
