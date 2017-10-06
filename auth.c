/*
 * auth.c
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

#include "auth.h"
#include "auth_ldap.h"
#include "auth_local.h"
#include "log.h"
#include "readconf.h"

extern Options options;

int auth_user( char *user, char *host, int mode )
{
    int auth = AUTH_FAILURE;

    /* don't allow proxy mode if it's been disabled */
    if ( options.enable_proxy == 0 && mode == AUTH_PROXY ) {
        return( AUTH_FAILURE );
    }

    switch ( options.auth_type ) {
        case AUTH_TYPE_ANY:
            return( AUTH_SUCCESS );
            /*@-unreachable@*/
            break;
            /*@+unreachable@*/
        case AUTH_TYPE_LOCAL:
            if ( mode == AUTH_PROXY && options.disable_proxy_authorization ==  0) {
                /*@-null@*/
                auth = get_auth_file( user, host, options.users_file, 1 );
                /*@+null@*/
            } else {
                /*@-null@*/
                auth = get_auth_file( user, host, options.users_file, 0 );
                /*@+null@*/
            }
            break;
        case AUTH_TYPE_LDAP:
            if ( mode == AUTH_PROXY && options.disable_proxy_authorization ==  0) {
                /*@-null@*/
                auth = get_auth_ldap( user, host, options.authorized_service_tag );
                /*@+null@*/
            } else {
                /*@-null@*/
                auth = get_auth_ldap( user, host, NULL );
                /*@+null@*/
            }
            break;
        default:
            error( "Invalid authorization type: %i", options.auth_type );
            return ( AUTH_ERROR );
            /*@-unreachable@*/
            break;
            /*@+unreachable@*/
    }

    return( auth );
}
