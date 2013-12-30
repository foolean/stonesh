/*
 * auth_ldap.h
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

#ifndef _AUTH_LDAP_H
#define _AUTH_LDAP_H

#ifndef LDAP_AUTH_FAILURE
#define LDAP_AUTH_FAILURE   0
#endif

#ifndef LDAP_AUTH_SUCCESS
#define LDAP_AUTH_SUCCESS   1
#endif

#ifndef LDAP_HOST_FILTER
#define LDAP_HOST_FILTER "(&(|(host=\\*)(host=%s))(uid=%s))"
#endif

#ifndef LDAP_SERVICE_FILTER
#define LDAP_SERVICE_FILTER "(&(|(host=\\*)(host=%s))(|(authorizedService=%s-\\*)(authorizedService=%s-%s))(uid=%s))"
#endif

#define LDAP_FREE(x) if ( x != NULL ) { free(x); x = NULL; } else { x = NULL; }

/*@-exportlocal@*/
/*@null@*/ char *get_ldap_uri();
/*@null@*/ char *get_ldap_base();
/*@null@*/ char *get_host_filter( char *host, char *user );
/*@null@*/ char *get_service_filter( char *host, char *user, char *tag );
void ldap_free();
/*@+exportlocal@*/

int get_auth_ldap( char *user, char *host, char *tag );

#endif /* _AUTH_LDAP_H */
