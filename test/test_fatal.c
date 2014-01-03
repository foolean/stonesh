/*
 * test_fatal.c
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

#include "log.h"

char *progname = "test_fatal";

int test_fatal(char *msg)
{
    printf( "   fatal( \"log.c:fatal(): test message: '%%s'\", msg );" );

#undef  EXIT_FAILURE
#define EXIT_FAILURE 0

    fatal( "log.c:fatal(): test message: '%s'", msg );
    printf( " passed\n" );
    return( 0 );

}

int main()
{
    int errors = 0;
    int tests = 0;


    /* test the fatal function */
    printf( "testing log.c:fatal()\n" );

    tests++; errors += test_fatal("foolean.org");

    printf( "\nlog.c:fatal(): passed %i out of %i tests\n", ( tests - errors ), tests );
    return( errors );
}
