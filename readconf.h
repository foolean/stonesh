/*
 * readconf.h
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

#ifndef _READCONF_H
#define _READCONF_H

#ifndef ALLOWEDHOSTSFILE
#define ALLOWEDHOSTSFILE "/etc/stonesh/hosts.lst"
#endif

#ifndef AUTHUSERSFILE
#define AUTHUSERSFILE "/etc/stonesh/users.lst"
#endif

#ifndef AUTHORIZEDSERVICETAG
#define AUTHORIZEDSERVICETAG "proxy"
#endif

#ifndef PROXYKEYWORD
#define PROXYKEYWORD "direct"
#endif

#ifndef NCTIMEOUT
#define NCTIMEOUT "1"
#endif

#ifndef NETCAT
#define NETCAT "/bin/nc"
#endif

#ifndef SSH
#define SSH "/usr/bin/ssh"
#endif

#ifndef GEOIP_DAT
#define GEOIP_DAT "/usr/share/GeoIP/GeoIP.dat"
#endif

#ifndef GEOIP6_DAT
#define GEOIP6_DAT "/usr/share/GeoIP/GeoIPv6.dat"
#endif

#define OPT_FREE(x) if ( x != NULL ) { free(x); x = NULL; } else { x = NULL; }

/* Enum of option codes */
typedef enum {
    oBadOption,
    oAuthorizedServiceTag,
    oAuthType,
    oAllowFQDN,
    oDebug,
    oDisableAuthorization,
    oLogToSTDERR,
    oUseLDAP,
    oProxyKeyword,
    oLDAPuri,
    oLDAPbase,
    oHostsFile,
    oUsersFile,
    oEnableProxy,
    oSSHPath,
    oNCPath,
    oNCTimeout,
    oGeoIP,
    oGeoIP6
} OpCodes;

/* Options structure */
typedef struct {
    int  debug;
    int  log_to_stderr;
    int  use_ldap;
    int  enable_proxy;
    int  auth_type;
    int  allowfqdn;
    int  disable_authorization;
    /*@null@*/ char *host;
    /*@null@*/ char *port;
    /*@null@*/ char *authorized_service_tag;
    /*@null@*/ char *proxy_keyword;
    /*@null@*/ char *ldap_uri;
    /*@null@*/ char *ldap_base;
    /*@null@*/ char *hosts_file;
    /*@null@*/ char *users_file;
    /*@null@*/ char *ssh_path;
    /*@null@*/ char *nc_path;
    /*@null@*/ char *nc_timeout;
    /*@null@*/ char *geoip_dat;
    /*@null@*/ char *geoip6_dat;
} Options;

extern Options options;

/*@-exportlocal@*/
/*@null@*/ char *strdelim( char ** );
OpCodes parse_token( char *cp,  const char*, int );
int  process_config_line( Options*, char*, const char*, int, int * );
void initialize_options( Options* );
void fill_default_options( Options* );
void free_options( Options* );
/*@+exportlocal@*/

void read_config_file( const char*, Options* );

#endif /* _READCONF_H */
