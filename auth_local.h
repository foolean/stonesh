/*
 * auth_local.h
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

#ifndef _AUTH_LOCAL_H
#define _AUTH_LOCAL_H

#ifndef AUTH_LOCAL_FAILURE
#define AUTH_LOCAL_FAILURE   0
#endif

#ifndef AUTH_LOCAL_SUCCESS
#define AUTH_LOCAL_SUCCESS   1
#endif

#ifndef MAXLINELEN
#define MAXLINELEN 1024
#endif

int get_auth_file( char *user, char *host, char *file, int proxy );

#endif /* _AUTH_LOCAL_H */
