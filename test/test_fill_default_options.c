/*
 * test_fill_default_options.c
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

char *progname = "test_fill_default_options";

Options options;

int main()
{
    int errors = 0;
    int tests = 0;


    /* test the fill_default_options function */
    printf( "testing readconf.c:fill_default_options()\n" );

    initialize_options( &options );
    fill_default_options(&options);

    printf( "options.debug = '%i' ", options.debug );
    tests++; if ( options.debug != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.log_to_stderr = '%i' ", options.log_to_stderr );
    tests++; if ( options.log_to_stderr != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.use_ldap = '%i' ", options.use_ldap );
    tests++; if ( options.use_ldap != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.enable_proxy = '%i' ", options.enable_proxy );
    tests++; if ( options.enable_proxy != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.auth_type = '%i' ", options.auth_type );
    tests++; if ( options.auth_type != 1 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.allowfqdn = '%i' ", options.allowfqdn );
    tests++; if ( options.allowfqdn != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.disable_authorization  = '%i' ", options.disable_authorization );
    tests++; if ( options.disable_authorization != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.host = '%s' ", options.host );
    tests++; if ( options.host != NULL ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.port = '%s' ", options.port );
    tests++; if ( options.port != NULL ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.authorized_service_tag = '%s' ", options.authorized_service_tag );
    tests++; if ( strcasecmp( options.authorized_service_tag, "proxy" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.proxy_keyword = '%s' ", options.proxy_keyword );
    tests++; if ( strcasecmp( options.proxy_keyword, "direct" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.ldap_uri = '%s' ", options.ldap_uri );
    tests++; if ( options.ldap_uri != NULL ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.ldap_base = '%s' ", options.ldap_base );
    tests++; if ( options.ldap_base != NULL ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.hosts_file = '%s' ", options.hosts_file );
    tests++; if ( strcasecmp( options.hosts_file, "/etc/stonesh/hosts.lst" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.users_file = '%s' ", options.users_file );
    tests++; if ( strcasecmp( options.users_file, "/etc/stonesh/users.lst" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.ssh_path = '%s' ", options.ssh_path );
    tests++; if ( strcasecmp( options.ssh_path, "/usr/bin/ssh" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.nc_path = '%s' ", options.nc_path );
    tests++; if ( strcasecmp( options.nc_path, "/bin/nc" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.nc_timeout = '%s' ", options.nc_timeout );
    tests++; if ( strcasecmp( options.nc_timeout, "1" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.geoip_dat = '%s' ", options.geoip_dat );
    tests++; if ( strcasecmp( options.geoip_dat, "/usr/share/GeoIP/GeoIP.dat" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    printf( "options.geoip6_dat = '%s' ", options.geoip6_dat );
    tests++; if ( strcasecmp( options.geoip6_dat, "/usr/share/GeoIP/GeoIPv6.dat" ) != 0 ) { printf("failed\n"); errors++; } else { printf("passed\n"); }

    free_options(&options);

    printf( "\nreadconf.c:fill_default_options(): passed\n %i out of %i tests\n", ( tests - errors ), tests );
    return( errors );
}
