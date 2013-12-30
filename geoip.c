/*
 * geoip.c
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

#include <GeoIP.h>
#include <string.h>
#include <stdio.h>

#include "geoip.h"
#include "readconf.h"
#include "util.h"

extern Options options;

/*
 * is_from_valid_country - Determine if the GeoIP country code for a  string
 *      representation of an IPv4 or IPv6 is contained in a list of country
 *      codes separated by a specified delimiter.
 */
int is_from_valid_country( char *address, char *country_list, char *delim )
{
    char        *country;
    const char  *country_code;
    char        *delimiter;
    int         af;

    /* Ignore NULL and empty lists */
    if ( country_list == NULL )        { return( 1 ); }
    if ( strlen( country_list ) == 0 ) { return( 1 ); }

    /* set the delimiter */
    delimiter = ( delim != NULL ) ? delim : GEOIP_DELIM;

    /* Get the Country Code for the address */
    af= getafbyaddr( address );
    switch ( af ) {
        case AF_INET:
            /* IPv4 loopback is permitted */
            if ( is_local( address ) == 1 ) { return( 1 ); }
            /* IPv4 RFC-1918 addresses are permitted */
            //if ( is_private( address ) == 1 ) { return( 1 ); }
            /* get the IPv4 country code */
            /*@-nullpass@*/
            country_code = getcountrybyaddr( address, options.geoip_dat );
            /*@+nullpass@*/
            break;
        case AF_INET6:
            /* IPv6 loopback is permitted */
            if ( is_local6( address ) == 1 ) { return( 1 ); }
            /* IPv6 RFC-4193 addresses are permitted */
            //if ( is_private6( address ) == 1 ) { return( 1 ); }
            /* get the IPv6 country code */
            /*@-nullpass@*/
            country_code = getcountrybyaddr6( address, options.geoip6_dat );
            /*@+nullpass@*/
            break;
        default:
            country_code = NULL;
            break;
    }

    /* did we get a country code? */
    if ( country_code == NULL ) {
        if ( GEOIP_FAIL_ON_ERROR == 1 ) {
            return( 0 ); 
        } else {
            return( 1 );
        }
    }

    /* Check if the country of the address matches the list */
    country = strtok( country_list, delimiter );
    while ( country != NULL ) {
        trim( country );
        (void)strtoupper( country );
        if ( strlen( country_code ) == strlen( country ) ) {
            if ( strncmp ( country_code, country, strlen(country)) == 0 ) {
                //free( country_list );
                /*@-mustfreefresh@*/
                return( 1 );    /* we have a match */
                /*@+mustfreefresh@*/
            }
        }
        country = strtok( NULL, delimiter );
    }

    /*@-mustfreefresh@*/
    return( 0 );    /* no match */
    /*@+mustfreefresh@*/
}

/* 
 * getafbyaddr - Get the address family of a
 *  string representation of an IPv4 or IPv6 
 *  address.
 */
int getafbyaddr(char *addr) {
    struct addrinfo hints;
    struct addrinfo *ai = NULL;
    int             result;
    int             af = -1;

    /* Set up the hints structure */
    memset( &hints, 0, sizeof(hints) );
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags  = AI_NUMERICHOST;

    /* Get the address information */
    /*@-nullpass@*/
    result = getaddrinfo( addr, NULL, &hints, &ai );
    /*@+nullpass@*/
    free( hints.ai_addr );
    free( hints.ai_canonname );
    free( hints.ai_next );
    if ( result != 0 ) { return( -1 ); }     /* error */

    if ( ai != NULL ) {
        af = ai->ai_family;     /* save the address family         */
        freeaddrinfo( ai );     /* release memory from getaddrinfo */
    }
    return( af );               /* return the address family       */
}

/* 
 * getcountrybyaddr - Get the GeoIP country code of a
 *  string representation of an IPv4 address.
 */
/*@null@*/ 
const char *getcountrybyaddr( char *address, char *geoip_data_file )
{
    const char  *country_code = NULL;
    char        *data_file;
    GeoIP       *gi;

    /* set the data file */
    data_file = ( geoip_data_file != NULL ) ? geoip_data_file : GEOIP_DAT;

    /* open the GeoIP database */
    gi = GeoIP_open( data_file, GEOIP_STANDARD );
    if ( gi != NULL ) { 
        /* Get the Country Code for the address */
        country_code = GeoIP_country_code_by_addr( gi, address );

        /* close the GeoIP database */
        GeoIP_delete( gi );
    }
    /*@-mustfreefresh@*/
    return( country_code );
    /*@+mustfreefresh@*/
}

/* 
 * getcountrybyaddr6 - Get the GeoIP country code of a
 *  string representation of an IPv6 address.
 */
/*@null@*/ 
const char *getcountrybyaddr6( char *address, char *geoip_data_file )
{
    const char  *country_code = NULL;
    char        *data_file;
    GeoIP       *gi;

    /* set the data file */
    data_file = ( geoip_data_file != NULL ) ? geoip_data_file : GEOIP6_DAT;
    
    /* open the GeoIP database */
    gi = GeoIP_open( data_file, GEOIP_STANDARD );
    if ( gi != NULL ) { 
        /* Get the Country Code for the address */
        country_code = GeoIP_country_code_by_addr_v6( gi, address );

        /* close the GeoIP database */
        GeoIP_delete( gi );
    }
    /*@-mustfreefresh@*/
    return( country_code );
    /*@+mustfreefresh@*/
}

/*
 * is_local - determine if the string representation of the
 *  address is an IPv4 loopback address.
 */
int is_local( char *address )
{
    int result;
    struct in_addr ip;

    /* convert the address to an in_addr structure */
    ip.s_addr = 0;
    result = inet_pton( AF_INET, address, &ip );
    if ( result != 1 ) { return( result ); }

    /* is it a loopback address */
    if ( ( ip.s_addr & htonl( 0xFF000000 ) ) == htonl( 0x7F000000 ) ) {
        return( 1 );
    }

    return( 0 );
}

/*
 * is_local6 - determine if the string representation of the
 *  address is an IPv6 loopback address.
 */
int is_local6( char *address )
{
    int result;
    struct in6_addr ip;
    struct in6_addr local;

    /* convert the address to an in6_addr structure */
    result = inet_pton( AF_INET6, address, &ip );
    if ( result != 1 ) { return( result ); }

    /* convert an IPv6 loopback address to an in6_addr structure */
    result = inet_pton( AF_INET6, "::1", &local );
    if ( result != 1 ) { return( result ); }

    /* do we have a loopback address */
    if ( memcmp( &ip, &local, sizeof( struct in6_addr ) ) == 0 ) {
        return( 1 );
    }

    return( 0 );
}

/*
 * is_private - determine if the string representation of the
 *  address is an IPv4 RFC-1918 address.
 */
int is_private( char *address )
{
    int result;
    struct in_addr ip;

    ip.s_addr = 0;

    /* convert the address to an in_addr structure */
    result = inet_pton( AF_INET, address, &ip );
    if ( result == 0 ) { return( result ); }

    if ( ( ( ip.s_addr & 0x000000FF ) == htonl(0x0A000000) ) ||
         ( ( ip.s_addr & 0x0000FFFF ) == htonl(0xC0A80000) ) ||
         ( ( ip.s_addr & 0x0000F0FF ) == htonl(0xAC100000) ) ) {
        return( 1 );
    }
    return( 0 );
}

/*
 * is_private6 - determine if the string representation of the
 *  address is an IPv6 RFC-4193 address.
 */
int is_private6( char *address )
{
    int result;
    struct in6_addr ip;

    /* zero out the address structure */
    memset(ip.s6_addr,0,sizeof(ip.s6_addr));

    /* convert the address to an in6_addr structure */
    result = inet_pton( AF_INET6, address, &ip );
    if ( result != 1 ) { return( result ); }

    if ( ( ip.s6_addr[0] == 0xfc ) || ( ip.s6_addr[0] == 0xfd ) ) {
        return( 1 );
    }
    return( 0 );
}
