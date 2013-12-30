/*
 * auth_ldap.c
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

#include <ldap.h>
#include <stdlib.h>
#include <string.h>

#include "auth_ldap.h"
#include "readconf.h"
#include "log.h"

extern Options options;

/* get_ldap_uri - get LDAP uri from ldap.conf */
/*@null@*/ char *get_ldap_uri() 
{
    char *value = NULL;
    char *uri   = NULL;

    /* 
     * Return the contents of the global option
     * 'ldap_uri' if it exists.
     */
    if ( options.ldap_uri != NULL ) {
        /*@-unrecog@*/
        uri = strdup( options.ldap_uri );
        /*@+unrecog@*/
        return( uri );
    }

    /* Get the default uri value */
    /*@-nullpass@*/
    if ( ldap_get_option( NULL, LDAP_OPT_URI, &value ) != LDAP_OPT_SUCCESS ) {
    /*@+nullpass@*/
        error("ldap_get_option failed to get the uri");
        /*@-globstate@*/
        return( NULL );
        /*@+globstate@*/
    }

    /* 
     * Copy the contents and release the memory
     * allocated by ldap_get_option.
     */
    if ( value != NULL ) {
        /*@-unrecog@*/
        uri = strdup( value );
        /*@+unrecog@*/
        ldap_memfree( value );
    }

    /*@-globstate@*/
    return( uri );
    /*@+globstate@*/
}

/* get_ldap_base - get LDAP base from ldap.conf */
/*@null@*/ char *get_ldap_base() 
{
    char *value = NULL;
    char *base  = NULL;

    /* 
     * Return the contents of the global option
     * 'ldap_base' if it exists.
     */
    if ( options.ldap_base != NULL ) {
        /*@-unrecog@*/
        base = strdup( options.ldap_base );
        /*@+unrecog@*/
        return( base );
    }

    /* Get the default base dn value */
    /*@-nullpass@*/
    if ( ldap_get_option( NULL, LDAP_OPT_DEFBASE, &value ) != LDAP_OPT_SUCCESS ) {
    /*@+nullpass@*/
        error("ldap_get_option failed to get the default base dn");
        /*@-globstate@*/
        return( NULL );
        /*@+globstate@*/
    }

    /* 
     * Copy the contents and release the memory
     * allocated by ldap_get_option.
     */
    if ( value != NULL ) {
        /*@-unrecog@*/
        base = strdup( value );
        /*@+unrecog@*/
        ldap_memfree( value );
    }

    /*@-globstate@*/
    return( base );
    /*@+globstate@*/
}

/* 
 * get_host_filter - assemble the LDAP search filter for
 *      determining if a user is authorized for access
 *      to a host.
 */
/*@null@*/ char *get_host_filter( char *host, char *user )
{
    size_t fmtlen    = 0;  
    char   *format   = LDAP_HOST_FILTER;
    char   *host_fmt = NULL;

    /* We must have a host */
    if ( host == NULL || strlen( host ) == 0 ) {
        error( "must specify host for the host filter" );
        return( NULL );
    }

    /* We must have a username */
    if ( user == NULL || strlen( user ) == 0 ) {
        error( "must specify username for the host filter" );
        return( NULL );
    }

    /* Assemble the LDAP search filter for host access */
    /*
     * Note: we must include host=* to catch any users that
     *       have the "*" value for the host attribute instead
     *       of the actual hostname.
     */
    fmtlen = strlen( host ) + strlen( user ) + strlen( format );
    host_fmt = malloc( fmtlen );
    if ( host_fmt == NULL ) {
        error( "unable to allocate memory for host filter" );
        return( NULL );
    } 

    memset( host_fmt, 0, fmtlen );
    /*@-formatconst@*/
    if ( snprintf( host_fmt, fmtlen, format, host, user ) < 0 ) {
    /*@+formatconst@*/
        error( "unable to create the complete host filter" );
        free( host_fmt );
        return( NULL );
    }

    return( host_fmt );
}

/* 
 * get_service_filter - assemble the LDAP search filter
 *      for determining if a user is authorized for
 *      access to a host.
 */
/*@null@*/ char *get_service_filter( char *host, char *user, char *tag )
{
    size_t fmtlen   = 0;  
    char   *format  = LDAP_SERVICE_FILTER;
    char   *svc_fmt = NULL;
	
    /* We must have a host */
    if ( host == NULL || strlen( host ) == 0 ) {
        error( "must specify host for the service filter" );
        return( NULL );
    }

    /* We must have a username */
    if ( user == NULL || strlen( user ) == 0 ) {
        error( "must specify username for the service filter" );
        return( NULL );
    }

    /* We must have a tag */
    if ( tag == NULL || strlen( tag ) == 0 ) {
        error( "must specify tag for the service filter" );
        return( NULL );
    }

    /* Assemble the LDAP search filter for svc access */
    /*
     * Note: we must include authorizedService=%s-* to catch any users that
     *       have the "*" value for the authorizedService attribute instead
     *       of the specific authorizedService entry.  The '%s' is expanded
     *       to the tag specifying the bastion host the entry applies to.
     */
    fmtlen = ( strlen( host ) * 2 ) + strlen( user ) + (strlen( tag ) * 2) + strlen( format );
    svc_fmt = malloc( fmtlen );
    if ( svc_fmt == NULL ) {
        error( "unable to allocate memory for svc filter" );
        return( NULL );
    } 

    memset( svc_fmt, 0, fmtlen );
    /*@-formatconst@*/
    if ( snprintf( svc_fmt, fmtlen, format, host, tag, tag, host, user) < 0) {
    /*@+formatconst@*/
        error( "unable to create complete svc filter" );
        free( svc_fmt );
        return( NULL );
    }

    return( svc_fmt );
}

/*
 * get_auth_ldap - determine if a user is authorized for 
 *      a host and mode ( proxy or bastion ).
 */
int get_auth_ldap( char *user, char *host, char *tag )
{
    char *filter;
    char *uri  = NULL;
    char *base = NULL;
    int rc     = 0;
    int auth   = 0;
    int msgid  = 0;
    int scope  = LDAP_SCOPE_SUBTREE;
    LDAP *ld;
    LDAPMessage *result = NULL;

    /*@-nullassign@*/
    struct berval passwd = { 0, NULL };
    /*@+nullassign@*/

    /* return success if we're not using LDAP */
    if ( options.use_ldap == 0 ) {
        LDAP_FREE( passwd.bv_val );  /* no longer needed */
        return( LDAP_AUTH_SUCCESS );
    }

    /* get the ldap uri */
    uri = get_ldap_uri();
    if ( uri == NULL ) { 
        debug( "unable to get ldap uri" );
        auth = LDAP_AUTH_FAILURE;
        goto clean_return; 
    }
    debug( "using uri '%s'", uri );

    /* get the ldap base */
    base = get_ldap_base();
    if ( base == NULL ) { 
        debug( "unable to get ldap base" );
        auth = LDAP_AUTH_FAILURE;
        goto clean_return; 
    }
    debug( "using base dn '%s'", base );

    /* get the search filter */
    if ( tag == NULL ) {
        /* get the ldap host filter */
        filter = get_host_filter( host, user );
        if ( filter == NULL ) { 
            auth = LDAP_AUTH_FAILURE;
            error( "unable to get host filter" );
            goto clean_return; 
        }
    } else {
        /* get the ldap authorizedService filter */
        filter = get_service_filter( host, user, tag );
        if ( filter == NULL ) { 
            auth = LDAP_AUTH_FAILURE;
            error( "unable to get host and authorizeService filter" );
            goto clean_return; 
        }
    }
    debug( "using filter: '%s'", filter );

    /* Connect to the server */
    /*@-compdef@*/
    if (ldap_initialize( &ld, uri ) == LDAP_SUCCESS) {
    /*@+compdef@*/
        /*@-nullpass@*//*@-usedef@*/
        rc = ldap_sasl_bind( ld, NULL, LDAP_SASL_SIMPLE, &passwd, NULL, NULL, &msgid );
        rc = ldap_result( ld, msgid, 0, NULL, &result );
        /*@+nullpass@*//*@+usedef@*/
        LDAP_FREE( passwd.bv_val );  /* no longer needed */

        if ( rc > 0 ) {
            debug( "bound to uri: %s", uri );
            if ( result != NULL ) { rc = ldap_msgfree( result ); }

            /* Are we authorized for the requested host */
            /*@-nullpass@*/
            rc = ldap_search_ext( ld, base, scope, filter, NULL, 0, NULL, NULL, NULL, LDAP_NO_LIMIT, &msgid );
            rc = ldap_result( ld, msgid, 0, NULL, &result );
            /*@+nullpass@*/

            /* If the search returned an entry then we're authorized */
            /*@-nullpass@*/
            if ( ldap_count_entries( ld, result ) == 1 ) { 
                auth = LDAP_AUTH_SUCCESS; 
            } else {
                auth = LDAP_AUTH_FAILURE;
            }
            /*@+nullpass@*/
            debug( "setting auth to '%i'", auth );
            if ( result != NULL ) { rc = ldap_msgfree( result ); }
        } else {
            error( "unable to bind to server" );
            auth = LDAP_AUTH_FAILURE;
            if ( result != NULL ) { rc = ldap_msgfree( result ); }
            goto clean_return;
        }
    } else {
        error( "unable to initialize connection to %s", uri );
        auth = LDAP_AUTH_FAILURE;
        goto clean_return;
    }

clean_return:
    if ( ld != NULL ) {
        debug( "unbinding from uri: %s", uri );
        /*@-nullpass@*/
        if ( ldap_unbind_ext( ld, NULL, NULL ) < 0 ) {
        /*@+nullpass@*/
            error( "unable to unbind from %s", uri);
        }
    }
    LDAP_FREE( uri );
    LDAP_FREE( base );
    LDAP_FREE( filter );
    LDAP_FREE( passwd.bv_val );
    return( auth );
}

/*
 * ldap_free - release any final memory in use by libldap
 */
void ldap_free()
{
    /*@-nullpass@*/
    if ( ldap_set_option( NULL, LDAP_OPT_DEFBASE, NULL ) != LDAP_OPT_SUCCESS ) {
    /*@+nullpass@*/
        error("unable to set global defbase to null");
    }
    return;
}
