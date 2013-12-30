/*
 * auth.h
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

#ifndef _AUTH_H
#define _AUTH_H

/* Operational modes */
#define AUTH_BASTION 1
#define AUTH_PROXY   2

/* Authorization methods */
#define AUTH_TYPE_ANY   0
#define AUTH_TYPE_LOCAL 1
#define AUTH_TYPE_LDAP  2

/* Return codes */
#define AUTH_ERROR    -1
#define AUTH_FAILURE   0
#define AUTH_SUCCESS   1

int auth_user( char *user, char *host, int mode );

#endif /* _AUTH_H */

