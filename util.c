/*
 * util.c
 *
 * This file is part of stonesh.
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

#include "util.h"
#include <stdio.h>

/*
 * trim - trim comments and leading/trailing whitespace 
 */
void trim( /*@null@*/ char *s) {
    char *p;
    size_t l;
    size_t i = 0;
    int    comment = 0;

    if ( s == NULL ) { return; }
    if ( strlen( s ) == 0 ) { return; }

    p = s;
    l = strlen(p);

    while( i < l ) {
        if ( p[i] == '#' )  { comment = 1; }
        if ( comment == 1 ) { p[i] = '\0'; }
        i++;
    }
    l = strlen(p);
    while( l > 0 && isspace(p[l - 1])) { p[--l] = '\0'; }
    while( *p != '\0' && ( isspace( *p ) || *p == '#' )) { ++p; --l; }
    if ( p != s ) {
        memmove(s, p, l);
        s[l] = '\0';
    }
}

/*
 * strtoupper - convert a string to upper case
 */
char *strtoupper(char *string)
{
    char *cp;

    cp = string;
    while ( *cp != '\0' ) {
        *cp = toupper(*cp);
        cp++;
    }

    /*@-temptrans@*/
    return( string );
    /*@+temptrans@*/
}

/*
 * strtolower - convert a string to lower case
 */
char *strtolower(char *string)
{
    char *cp;
    
    cp = string;
    while ( *cp != '\0' ) {
        *cp = (char)tolower(*cp);
        cp++;
    }

    /*@-temptrans@*/
    return( string );
    /*@+temptrans@*/
}

