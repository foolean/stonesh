/*
 * geoip.h
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

#ifndef _GEOIP_H
#define _GEOIP_H

#include <ctype.h>
#include <GeoIP.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define GEOIP_DELIM         ","
#ifndef GEOIP_DAT
#define GEOIP_DAT           "/usr/share/GeoIP/GeoIP.dat"
#endif
#ifndef GEOIP6_DAT
#define GEOIP6_DAT          "/usr/share/GeoIP/GeoIPv6.dat"
#endif
#define GEOIP_FAIL_ON_ERROR 1


#define is_rfc1918(x) is_private(x)
#define is_rfc4193(x) is_private6(x)

/*@-exportlocal@*/
extern char *geoip_dat;
extern char *geoip6_dat;

int is_local( char *address );
int is_local6( char *address );
int is_private( char *address );
int is_private6( char *address );
int getafbyaddr( char *address );
/*@null@*/
const char *getcountrybyaddr( char *address, char *geoip_data_file );
/*@null@*/
const char *getcountrybyaddr6( char *address, char *geoip_data_file );
/*@+exportlocal@*/

int is_from_valid_country( char *address, char *list, char *delimiter );

#endif /* _GEOIP_H */
