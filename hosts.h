/*
 * hosts.h
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

#ifndef _HOSTS_H
#define _HOSTS_H

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#ifndef MAXLINELEN
#define MAXLINELEN 1024
#endif

#define HOSTS_FREE(x) if ( x != NULL ) { free(x); x = NULL; } else { x = NULL; }

typedef struct {
    /*@null@*/ char *host;
    /*@null@*/ char *countries;
    /*@null@*/ char *port;
} hostentry;

/*@null@*/ hostentry *gethostentry( char *host, char *hosts_file );

#endif /* _HOSTS_H */
