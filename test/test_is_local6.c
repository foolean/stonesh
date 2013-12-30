/*
 * test_is_local6.c
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

#include "geoip.h"

char *progname = "test_is_local6";

int test_is_local6( char *in, int out )
{
    int result;
    printf( "   is_local6( '%s', %i ) ", in, out );

    result = is_local6( in );
    printf( " = %i ", result );
    if ( result == out ) {
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


    /* test the trim function */
    printf( "testing geoip.c:is_local6()\n" );

    tests++; errors += test_is_local6( "::1", 1 );
    tests++; errors += test_is_local6( "0:0:0:0:0:0:0:1", 1 );
    tests++; errors += test_is_local6( "fc00::", 0 );
    tests++; errors += test_is_local6( "127.0.0.1", 0 );
    tests++; errors += test_is_local6( "10.1.20.1", 0 );
    tests++; errors += test_is_local6( "164.17.20.9", 0 );
    tests++; errors += test_is_local6( "foo", 0 );

    printf( "\ngeoip.c:is_local6(): passed %i out of %i tests\n", ( tests - errors ), tests );
    return( errors );
}
