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

#include "auth_local.h"
#include "log.h"
#include "util.h"

int get_auth_file( char *user, char *host, char *file, int proxy )
{
    FILE *fp;
    int  auth = AUTH_LOCAL_FAILURE;
    int  linenum = 0;
    int  can_proxy = 0;
    char line[ MAXLINELEN + 1 ];
    char *lineptr = NULL;
    char *username;
    char *hosts;
    char *hostname;

    /* return AUTH_LOCAL_FAILURE if we were sent NULL */
    if ( user == NULL || host == NULL || file == NULL ) {
        return AUTH_LOCAL_FAILURE;
    }

    /* open the file for reading */
    fp = fopen( file, "r" );
    if ( fp == NULL ) {
        error( "unable to open file '%s': %s", file, strerror(errno) );
        return( AUTH_LOCAL_FAILURE );
    }
    debug( "opened %s", file );

    /* Remove any leading or trailing whitespace from the arguments */
    trim(user);
    trim(host);

    /* Read the file line by line */
    while( auth == AUTH_LOCAL_FAILURE && fgets(line, MAXLINELEN, fp) != NULL ) {
        linenum++;
        if ( strlen( line ) > 0 ) {
            lineptr = line;
            trim(lineptr);
            /*@-unrecog@*/
            username = strsep( &lineptr, ":" );
            /*@+unrecog@*/
            if ( username != NULL ) {
                trim(username);
                if ( /*@-unrecog@*/ strncasecmp( user, username, strlen(user) ) == 0 /*@+unrecog@*/ ) {
                    /* Get the hosts list */
                    hosts = strsep( &lineptr, ":" );
                    if ( hosts != NULL ) {
                        debug( "hosts = '%s'", hosts );

                        /* traverse the hosts list */
                        hostname = strtok(hosts, ",");
                        while ( auth == AUTH_LOCAL_FAILURE && hostname != NULL ) {
                            trim(hostname);
                            if ( hostname[strlen(hostname) - 1] == '+' ) {
                                hostname[strlen(hostname) - 1] = '\0';
                                can_proxy = 1;
                            }
                            if ( strncasecmp(host, hostname, strlen(host)) == 0 ) {
                                if ( can_proxy == 0 && proxy == 1 ) {
                                    auth = AUTH_LOCAL_FAILURE;
                                } else {
                                    auth = AUTH_LOCAL_SUCCESS;
                                }
                                break;
                            }
                            hostname = strtok( NULL, "," );
                            can_proxy = 0;
                        }
                    }
                }
            }
        }
    }

    /* Close the file */
    if ( fclose( fp ) != 0 ) {
        error( "unable to close file '%s': %s", file, strerror(errno));
    } else {
        debug( "closed %s", file );
    }

    /* Return the result */
    return( auth );
}
