/*
 * readconf.c
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
/*
 * Portions of this code derived and borrowed from 
 * OpenSSH readconf.c written by  Tatu Ylonen <ylo@cs.hut.fi>
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "auth.h"
#include "readconf.h"
#include "log.h"
#include "util.h"

#define WHITESPACE  " \t\r\n"
#define QUOTE       "\""

/* Global options structure */
Options options;

static struct {
    /*@null@*/ const char *name;
    OpCodes opcode;
} keywords[] = {
    { "authorizedservicetag", oAuthorizedServiceTag },
    { "allowfqdn", oAllowFQDN },
    { "debug", oDebug },
    { "logtostderr", oLogToSTDERR },
    { "useldap", oUseLDAP },
    { "proxykeyword", oProxyKeyword },
    { "ldapuri", oLDAPuri },
    { "ldapbase", oLDAPbase },
    { "hostsfile", oHostsFile },
    { "usersfile", oUsersFile },
    { "enableproxy", oEnableProxy },
    { "disableauthorization", oDisableAuthorization },
    { "sshpath", oSSHPath },
    { "ncpath", oNCPath },
    { "nctimeout", oNCTimeout },
    { "geoip", oGeoIP },
    { "geoip6", oGeoIP6 },
    { NULL, oBadOption }
};

/*
 * read_config_file - main configuration parsing function.
 */
void read_config_file( const char *filename, Options *options )
{
    FILE *f;
    char line[1024];
    int  active;
    int  err = 0;
    int  linenum;
    int  bad_options = 0;

    /* open the configuration file */
    if ( (f = fopen( filename, "r" ) ) == NULL ) {
        fatal( "unable to open file '%s': %s", filename, strerror( errno ) );
        EXIT( EXIT_FAILURE );   /* should never get here */
    }

    /* initialize the options structure */
    initialize_options( options );

    active  = 1;
    linenum = 0;

    /* read the configuration file line by line */
    while ( fgets( line, (int)sizeof( line ), f ) ) {
        /* Update line number counter. */
        linenum++;
        err = process_config_line(options, line, filename, linenum, &active );
        if ( err != 0 ) { bad_options++; }
        if ( err == -1 ) { break; } 
    }

    /* close the file */
    if ( fclose( f ) != 0 ) {
        error( "unable to close '%s': %s", filename, strerror(errno));
    }

    /* throw an error and exit if we have bad configuration options */
    if ( bad_options > 0 ) {
        error( "%s: terminating, %d bad configuration options", filename, bad_options );
        free_options( options );
        EXIT( EXIT_FAILURE );   /* should never get here */
    }

    /* set any unset options to their defaults */
    fill_default_options( options );

    return;
}

int process_config_line(Options *options,
                          char *line, const char *filename, int linenum,
                          int *activep)
{
    int  opcode;
    char *s = line;
    char *keyword;
    char *arg;
    char **charptr;
    int  *intptr;
    int  value;
    int  status = 0;

    /* Strip trailing whitespace */
    trim( s );
    if ( *s == '\0' ) { return( 0 ); }
       
    debug( "%s: line %d: parsing '%s'", filename, linenum, s );

    /* Get the keyword portion of the line */
    /* 
     * NOTE: the strdelim function, by itself, causes splint
     *       to complain about keyword not being free'd at
     *       the end.  Attempts to free keyword when it's no
     *       longer needed result in memory corruption due to
     *       the way strdelim works.  Using strdup is a bit
     *       of a band-aid but strdelim should be rewritten.
     */
    /*@-unrecog@*/
    keyword = strdup( strdelim( &s ) );
    /*@+unrecog@*/
    if ( keyword == NULL ) {
        error( "%s: line %d: unable to parse keyword from '%s'", 
            filename, linenum, s );
        return( 0 );
    }

    opcode = parse_token( keyword, filename, linenum );
    OPT_FREE( keyword );

    switch ( opcode ) {
        case oBadOption:
            /* don't panic, but count bad options */
            return( 1 );
            /*@-unreachable@*/
            break;
            /*@+unreachable@*/

        case oDebug:
            intptr = &options->debug;
parse_flag:
            arg = strdelim( &s );
            if ( arg == NULL || *arg == '\0' ) {
              error( "%.200s line %d: Missing yes/no argument.", filename, linenum );
              status = -1;  /* abort when we get back */
            } else {
                value = 0;      /* To avoid compiler warning... */
                if ( strcmp( arg, "yes" ) == 0 || strcmp( arg, "true" ) == 0 )
                    value = 1;
                else if ( strcmp( arg, "no" ) == 0 || strcmp( arg, "false" ) == 0 )
                    value = 0;
                else {
                    error( "%.200s line %d: Bad yes/no argument.", filename, linenum );
                    status = -1;    /* abort when we get back */
                }
                if ( activep != NULL && *intptr == -1 ) {
                    *intptr = value;
                }
            }
            break;

        case oAuthorizedServiceTag:
            debug( "processing authorized_service_tag" );
            charptr = &options->authorized_service_tag;
parse_string:
            arg = strdelim(&s);
            if (!arg || *arg == '\0') {
                error( "%.200s line %d: Missing argument.", filename, linenum );
                status = -1;    /* abort when we get back */
            }
            if ( *activep != 0 && *charptr == NULL ) {
                /*@-unrecog@*/
                *charptr = strdup( arg );
                /*@+unrecog@*/
            }
            break;

        case oNCTimeout:
            charptr = &options->nc_timeout;
            arg = strdelim(&s);
            if (arg == NULL || *arg == '\0') {
                error( "%.200s line %d: Missing argument.", filename, linenum );
                status = -1;    /* abort when we get back */
            } else {
                if ( atoi(arg) <= 0 ) {
                    error( "%.200s line %d: Invalid timeout value '%s'", filename, linenum, arg);
                    status = -1;    /* abort when we get back */
                }
            }
            if ( *activep != 0 && *charptr == NULL ) {
              *charptr = strdup( arg );
            }
            break;

        case oProxyKeyword:
            charptr = &options->proxy_keyword;
            goto parse_string;

        case oHostsFile:
            charptr = &options->hosts_file;
            goto parse_string;

        case oUsersFile:
            charptr = &options->users_file;
            goto parse_string;

        case oLDAPuri:
            charptr = &options->ldap_uri;
            goto parse_string;

        case oLDAPbase:
            charptr = &options->ldap_base;
            goto parse_string;

        case oSSHPath:
            charptr = &options->ssh_path;
            goto parse_string;

        case oNCPath:
            charptr = &options->nc_path;
            goto parse_string;

        case oGeoIP:
            charptr = &options->geoip_dat;
            goto parse_string;

        case oGeoIP6:
            charptr = &options->geoip6_dat;
            goto parse_string;

        case oUseLDAP:
            intptr = &options->use_ldap;
            goto parse_flag;

        case oEnableProxy:
            intptr = &options->enable_proxy;
            goto parse_flag;

        case oLogToSTDERR:
            intptr = &options->log_to_stderr;
            goto parse_flag;

        case oDisableAuthorization:
            intptr = &options->disable_authorization;
            goto parse_flag;

        case oAllowFQDN:
            intptr = &options->allowfqdn;
            goto parse_flag;

        default:
            error( "process_config_line: Unimplemented opcode %d", opcode );
            status = -1;    /* abort when we get back */
    }

    /* Check that there is no garbage at end of line. */
    if ( status == 0 ) {
        if ( ( arg = strdelim( &s ) ) != NULL && *arg != '\0' ) {
            OPT_FREE( arg );
            error( "%.200s line %d: garbage at end of line; \"%.200s\".",
                filename, linenum, arg );
            status = -1;    /* abort when we get back */
        }
    }

    return( status );
}

/* return next token in configuration line */
char *strdelim( char **s )
{
    char *old;
    int wspace = 0;

    if ( *s == NULL ) {
        return( NULL );
    }

    old = *s;

    *s = strpbrk( *s, WHITESPACE QUOTE "=" );
    if ( *s == NULL )
        return( old );

    if ( *s[0] == '\"' ) {
        memmove( *s, *s + 1, strlen( *s ) ); /* move nul too */
        /* Find matching quote */
        if ( ( *s = strpbrk( *s, QUOTE ) ) == NULL ) {
            return( NULL );          /* no matching quote */
        } else {
            *s[0] = '\0';
            *s += strspn( *s + 1, WHITESPACE ) + 1;
            return( old );
        }
    }

    /* Allow only one '=' to be skipped */
    if ( *s[0] == '=' ) {
        wspace = 1;
    }
    *s[0] = '\0';

    /* Skip any extra whitespace after first token */
    *s += strspn( *s + 1, WHITESPACE ) + 1;
    if ( *s[0] == '=' && wspace == 0 ) {
        *s += strspn( *s + 1, WHITESPACE ) + 1;
    }

    return( old );
}

/*
 * Returns the option number of the token pointed to by cp or oBadOption. 
*/
OpCodes parse_token( char *cp,  const char *filename, int linenum )
{
    int i;

    for ( i = 0; keywords[i].name; i++ ) {
        /*@-unrecog@*/
        if ( strcasecmp( cp, keywords[i].name ) == 0 ) {
        /*@+unrecog@*/
            return( keywords[i].opcode );
        }
    }

    error( "%s: line %d: Bad configuration option: %s",
               filename, linenum, cp );
    return( oBadOption );
}

/*
 * Initializes options to special values that indicate that they have not yet
 * been set.  Read_config_file will only set options with this value. Options
 * are processed in the following order: command line, user config file,
 * system config file.  Last, fill_default_options is called.
 */
void initialize_options( Options * options )
{
    memset( options, 0, sizeof( *options ) );
    options->debug                  = -1;
    options->log_to_stderr          = -1;
    options->use_ldap               = -1;
    options->enable_proxy           = -1;
    options->disable_authorization  = -1;
    options->auth_type              = -1;
    options->allowfqdn              = -1;
    OPT_FREE( options->authorized_service_tag );
    OPT_FREE( options->proxy_keyword );
    OPT_FREE( options->ldap_base );
    OPT_FREE( options->ldap_uri );
    OPT_FREE( options->hosts_file );
    OPT_FREE( options->users_file );
    OPT_FREE( options->ssh_path );
    OPT_FREE( options->nc_path );
    OPT_FREE( options->nc_timeout );
    OPT_FREE( options->host );
    OPT_FREE( options->port );
    OPT_FREE( options->geoip_dat );
    OPT_FREE( options->geoip6_dat );
}

void free_options( Options *options ) {

    /* Free memory allocated to our options */
    OPT_FREE( options->ldap_uri );
    OPT_FREE( options->ldap_base );
    OPT_FREE( options->nc_path );
    OPT_FREE( options->ssh_path );
    OPT_FREE( options->proxy_keyword );
    OPT_FREE( options->hosts_file );
    OPT_FREE( options->users_file );
    OPT_FREE( options->nc_timeout );
    OPT_FREE( options->authorized_service_tag );
    OPT_FREE( options->host );
    OPT_FREE( options->port );
    OPT_FREE( options->geoip_dat );
    OPT_FREE( options->geoip6_dat );
}

/*
 * Called after processing other sources of option data, this fills those
 * options for which no value has been specified with their default values.
 */
void fill_default_options( Options *options )
{
    if ( options->debug == -1 ) {
        options->debug = 0;
    }
    if ( options->log_to_stderr == -1 ) {
        options->log_to_stderr = 0;
    }
    if ( options->use_ldap == -1 ) {
        options->use_ldap = 0;
    }
    if ( options->enable_proxy == -1 ) {
        options->enable_proxy = 1;
    }
    if ( options->disable_authorization == -1 ) {
        options->disable_authorization = 0;
    }
    if ( options->allowfqdn == -1 ) {
        options->allowfqdn = 0;
    }
    if ( options->auth_type == -1 ) {
        if ( options->disable_authorization == 1 ) {
            options->auth_type = AUTH_TYPE_ANY;
        } else {
            if ( options->use_ldap == 1 ) {
                options->auth_type = AUTH_TYPE_LDAP;
            } else {
                options->auth_type = AUTH_TYPE_LOCAL;
            } 
        }
    }

    /* Set the default authorized_service_tag */
    if ( options->authorized_service_tag == NULL ) {
        options->authorized_service_tag = (char*)malloc( strlen(AUTHORIZEDSERVICETAG) + 1 );
        if ( options->authorized_service_tag == NULL ) {
            fatal("unable to allocate memory for authorized_service_tag");
            EXIT( EXIT_FAILURE );
        }
        memset(options->authorized_service_tag, 0, strlen(AUTHORIZEDSERVICETAG) + 1);
        strncpy(options->authorized_service_tag, AUTHORIZEDSERVICETAG, strlen(AUTHORIZEDSERVICETAG));
    }

    /* Set the default proxy_keyword? */
    if ( options->proxy_keyword == NULL ) {
        options->proxy_keyword = (char*)malloc( strlen(PROXYKEYWORD) + 1 );
        if ( options->proxy_keyword == NULL ) {
            fatal("unable to allocate memory for proxy_keyword");
            EXIT( EXIT_FAILURE );
        }
        memset(options->proxy_keyword, 0, strlen(PROXYKEYWORD) + 1);
        strncpy(options->proxy_keyword, PROXYKEYWORD, strlen(PROXYKEYWORD));
    }

    /* Set the default hosts_file? */
    if ( options->hosts_file == NULL ) {
        options->hosts_file = (char*)malloc( strlen(ALLOWEDHOSTSFILE) + 1 );
        if ( options->hosts_file == NULL ) {
            fatal("unable to allocate memory for hosts_file");
            EXIT( EXIT_FAILURE );
        }
        memset(options->hosts_file, 0, strlen(ALLOWEDHOSTSFILE) + 1);
        strncpy(options->hosts_file, ALLOWEDHOSTSFILE, strlen(ALLOWEDHOSTSFILE));
    }

    /* Set the default users_file? */
    if ( options->users_file == NULL ) {
        options->users_file = (char*)malloc( strlen(AUTHUSERSFILE) + 1 );
        if ( options->users_file == NULL ) {
            fatal("unable to allocate memory for users_file");
            EXIT( EXIT_FAILURE );
        }
        memset(options->users_file, 0, strlen(AUTHUSERSFILE) + 1);
        strncpy(options->users_file, AUTHUSERSFILE, strlen(AUTHUSERSFILE));
    }

    /* Set the default ssh_path? */
    if ( options->ssh_path == NULL ) {
        options->ssh_path = (char*)malloc( strlen(SSH) + 1);
        if ( options->ssh_path == NULL ) {
            fatal("unable to allocate memory for ssh_path");
            exit( EXIT_FAILURE );
        }
        memset(options->ssh_path, 0, strlen(SSH) + 1 );
        strncpy(options->ssh_path, SSH, strlen(SSH));
    }

    /* Set the default nc_path? */
    if ( options->nc_path == NULL ) {
        options->nc_path = (char*)malloc( strlen(NETCAT) + 1);
        if ( options->nc_path == NULL ) {
            fatal("unable to allocate memory for nc_path");
            exit( EXIT_FAILURE );
        }
        memset(options->nc_path, 0, strlen(NETCAT) + 1 );
        strncpy(options->nc_path, NETCAT, strlen(NETCAT));
    }

    /* Set the default timeout? */
    if ( options->nc_timeout == NULL ) {
        options->nc_timeout = (char*)malloc( strlen(NCTIMEOUT) + 1);
        if ( options->nc_timeout == NULL ) {
            fatal("unable to allocate memory for nc_timeout");
            exit( EXIT_FAILURE );
        }
        memset(options->nc_timeout, 0, strlen(NCTIMEOUT) + 1 );
        strncpy(options->nc_timeout, NCTIMEOUT, strlen(NCTIMEOUT));
    }

    /* Set the default geoip IPv4 database? */
    if ( options->geoip_dat == NULL ) {
        options->geoip_dat = (char*)malloc( strlen(GEOIP_DAT) + 1);
        if ( options->geoip_dat == NULL ) {
            fatal("unable to allocate memory for geoip_dat");
            exit( EXIT_FAILURE );
        }
        memset(options->geoip_dat, 0, strlen(GEOIP_DAT) + 1 );
        strncpy(options->geoip_dat, GEOIP_DAT, strlen(GEOIP_DAT));
    }

    /* Set the default geoip IPv6 database? */
    if ( options->geoip6_dat == NULL ) {
        options->geoip6_dat = (char*)malloc( strlen(GEOIP6_DAT) + 1);
        if ( options->geoip6_dat == NULL ) {
            fatal("unable to allocate memory for geoip6_dat");
            exit( EXIT_FAILURE );
        }
        memset(options->geoip6_dat, 0, strlen(GEOIP6_DAT) + 1 );
        strncpy(options->geoip6_dat, GEOIP_DAT, strlen(GEOIP6_DAT));
    }
}
