/*
 * stonesh.h
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

#ifndef _STONESH_H
#define _STONESH_H

#include <ctype.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#ifndef DEFAULTCONFIGFILE
#define DEFAULTCONFIGFILE "/etc/stonesh/stonesh.conf"
#endif

#define STONESH_FREE(x) if ( x != NULL ) { free(x); x = NULL; } else { x = NULL; }

/*@-exportlocal@*/
void usage();
void version();
void catch_signal( int );
/*@+exportlocal@*/

#endif /* _STONESH_H */

