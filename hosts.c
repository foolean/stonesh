/*
 * hosts.c
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

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "hosts.h"
#include "log.h"
#include "util.h"

#define HOSTS_FREE(x) if ( x != NULL ) { free(x); x = NULL; } else { x = NULL; }

static hostentry he;

/*@null@*/
hostentry *gethostentry( char *hostname, char *hosts_file )
{
    FILE *fp;
    int  linenum = 0;
    char line[ MAXLINELEN + 1 ];
    char *lineptr = NULL;
    char *host;
    char *port;
    char *countries;
    hostentry *heptr;

    /* return null if we were sent null */
    if ( hostname == NULL )   { return( NULL ); }
    if ( hosts_file == NULL ) { return( NULL ); }

    /* Initialize hostentry structure */
    heptr = &he;
    HOSTS_FREE( heptr->host );
    HOSTS_FREE( heptr->port );
    HOSTS_FREE( heptr->countries );

    /* Open the hosts file */
    fp = fopen( hosts_file, "r" );
    if ( fp == NULL ) {
        error( "unable to open hosts file '%s': %s", hosts_file, strerror(errno) );
        return( NULL );
    }
    debug( "opened %s", hosts_file );

    /* Read the hosts file line by line */
    while( fgets(line, MAXLINELEN, fp) ) {
        linenum++;
        trim( line );
        if ( strlen( line ) > 0 ) {
            /*@-unrecog@*/
            lineptr = line;
            host = strsep( &lineptr, ":" );
            /*@+unrecog@*/
            if ( host != NULL ) {
                if ( strlen( host ) == strlen( hostname ) ) {
                    if ( strncmp( host, hostname, strlen( hostname )) == 0 ) {
                        /*@-unrecog@*/
                        heptr->host = strdup(host);
                        /*@+unrecog@*/
                        debug( "host = '%s'", heptr->host );

                        /* Get the port */
                        port = strsep( &lineptr, ":" );
                        if ( port == NULL || *port == '\0' ) {
                            heptr->port = strdup( "22" );
                        } else {
                            heptr->port = strdup(port);
                        }
                        trim( heptr->port );
                        debug( "port = '%s'", heptr->port );

                        /* Get the countries list */
                        countries = strsep( &lineptr, ":" );
                        if ( countries == NULL || *countries == '\0' ) {
                            heptr->countries = strdup("");
                        } else {
                            heptr->countries = strdup(countries);
                        }
                        trim( heptr->countries );
                        debug( "countries = '%s'", heptr->countries );

                        /* close the file */
                        if ( fclose( fp ) != 0 ) {
                            error( "unable to close hosts file '%s': %s", hosts_file, strerror(errno));
                        } else {
                            debug( "closed %s", hosts_file );
                        }

                        /*@-immediatetrans@*/
                        return( heptr );
                        /*@+immediatetrans@*/
                    }
                }
            }
        }
    }

    /* Close the hosts file */
    if ( fclose( fp ) != 0 ) {
        error( "unable to close hosts file '%s': %s", hosts_file, strerror(errno));
    } else {
        debug( "closed %s", hosts_file );
    }
    
    // HOSTS_FREE( line );
    return( NULL );
}
