/*
 * stonesh.c
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

#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "config.h"
#include "hosts.h"
#include "auth.h"
#include "auth_ldap.h"
#include "log.h"
#include "readconf.h"
#include "stonesh.h"
#include "util.h"
#include "geoip.h"

char *progname = PACKAGE_NAME;  /* defined in config.h by ./configure */

int main( int argc, char **argv )
{
    int    auth = 0;
    int    c;
    int    proxy = 0;
    int    status = EXIT_SUCCESS;
    int    newargc = 0;
    size_t argv_len = 0;
    size_t newargv_len = 0;
    char   *host  = NULL;
    char   *rhost = NULL;
    char   *rport = NULL;
    char   *args = NULL;
    char   *cmdline = NULL;
    char   *newargv[( sizeof(argv) / sizeof(int) ) + 10];
    char   *newcmdline = NULL;
    char   *cval = NULL;
    char   *Cval = NULL;
    struct passwd *pwd;
    hostentry *hostent;

    /* Catch SIGINT and SIGTERM */
    (void) signal(SIGINT,  catch_signal);
    (void) signal(SIGTERM, catch_signal);

    /* Get the current username for logging */
    pwd = getpwuid( geteuid() );
    if ( pwd == NULL ) {
        fatal("unable to get the username");
        exit( EXIT_FAILURE );
    }

    /* Get the SSH remote host for logging */
    /*@-modobserver@*/
    rhost  = strtok( getenv( "SSH_CLIENT" ), " ");
    /*@+modobserver@*/
    if ( rhost != NULL ) {
        rport = strtok(NULL, " ");
        notice("(%s) connection via %s:%s", pwd->pw_name, rhost, rport);
    } else {
        notice( "(%s) local connection", pwd->pw_name );
    }

    /* Process command-line options */
    opterr = 0;
    while ( ( c = getopt( argc, argv, "c:C:lhv" ) ) != -1 ) {
        switch ( c ) {
            case 'c':
                cval = optarg;
                break;
            case 'C':
                /*@-unrecog@*/
                Cval = strdup( optarg );
                /*@+unrecog@*/
                break;
            case 'h':
                usage();
                exit( EXIT_FAILURE );
                /*@-unreachable@*/
                break;
                /*@+unreachable@*/
            case 'l':
                log_to_stderr = 1;
                break;
            case 'v':
                version();
                exit( EXIT_FAILURE );
                /*@-unreachable@*/
                break;
                /*@+unreachable@*/
            default:
                fprintf( stderr, "Illegal option '%c'\n", c );
                usage();
        }
    }

    /* Read configuration options */
    if ( Cval == NULL ) {
        /*@-unrecog@*/
        Cval = strdup( DEFAULTCONFIGFILE ); 
        /*@+unrecog@*/
    }
    if ( read_config_file( Cval, &options ) == 0 ) {
        error("unable to read configuration file");
        status = EXIT_FAILURE;
        STONESH_FREE( Cval );
        goto clean_exit;
    }
    STONESH_FREE( Cval );

    if ( log_to_stderr > 0 ) { options.log_to_stderr = 1; }
    debugging = options.debug;

    /*@-null@*/
    debug( "rhost = '%s'", rhost );
    debug( "ssh_path = '%s'", options.ssh_path );
    debug( "nc_path = '%s'", options.nc_path );
    debug( "nc_timeout = '%s'", options.nc_timeout );
    debug( "enable_proxy = '%i'", options.enable_proxy );
    debug( "proxy_keyword = '%s'", options.proxy_keyword );
    debug( "hosts_file = '%s'", options.hosts_file );
    debug( "users_file = '%s'", options.users_file );
    debug( "use_ldap = '%i'", options.use_ldap );
    debug( "ldap_uri = '%s'", options.ldap_uri );
    debug( "ldap_base = '%s", options.ldap_base );
    debug( "authorized_service_tag = '%s'", options.authorized_service_tag );
    debug( "geoip_dat = '%s'", options.geoip_dat );
    debug( "geoip6_dat = '%s'", options.geoip6_dat );
    /*@+null@*/

    /* Concatenate argv for debugging output */
    for (c = 0; c < argc; c++) {
        argv_len += strlen(argv[c]);
    }
    cmdline = (char*)malloc(argv_len + (argc * 4));
    if ( cmdline == NULL ) {
        error( "unable to allocate memory for cmdline" );
        status = EXIT_FAILURE;
        goto clean_exit;
    } else {
        memset(cmdline, 0, argv_len + (argc * 4));
        strncpy(cmdline, argv[0], strlen(argv[0]));
        for (c = 1; c < argc; c++) {
            strncat(cmdline, " \"", 2);
            strncat(cmdline, argv[c], strlen(argv[c]));
            strncat(cmdline, "\"", 1);
        }
    }
    /*@-null@*/
    debug( "cmdline = '%s'", cmdline );
    /*@+null@*/

    /* Parse the incoming command */
    if ( cval != NULL ) {
        host = strtok( cval, " " );
        /*@-null@*/
        if ((strlen(host) == strlen(options.proxy_keyword)) && (strncmp(host, options.proxy_keyword, strlen(options.proxy_keyword)) == 0)) {
        /*@+null@*/
            if ((host = strtok(NULL, " ")) == NULL) {
                error("(%s) cmdline='%s'", pwd->pw_name, cmdline);
                STONESH_FREE(cmdline);
                error("(%s) no host specified in proxy access request, exiting", pwd->pw_name);
                status = EXIT_FAILURE;
                goto clean_exit;
            }
            proxy = 1;
            // args  = "";
        } else {
            args = strtok(NULL, "");
        }
    }
    STONESH_FREE(cmdline);  /* we don't need this any more */

    /* Load the host entered on the command-line into the options */
    if ( host != NULL ) {
        if ( options.host != NULL ) { OPT_FREE( options.host ); }
        options.host = malloc(strlen(host) + 1);
        if ( options.host == NULL ) {
            error( "unable to allocate memory for host option" );
            status = EXIT_FAILURE;
            goto clean_exit;
        } else {
            memset(options.host, 0, strlen(host) + 1);
            strncpy(options.host, host, strlen(host));
        }
    }

    /* Get the host we want to connect to if not already provided */
    while ( options.host == NULL ) {
        debug("no host specified, prompting user for value");
        options.host = malloc(MAXHOSTNAMELEN + 1);
        if ( options.host == NULL ) {
            error( "unable to allocate maximum memory for host option" );
            status = EXIT_FAILURE;
            goto clean_exit;
        } else {
            memset(options.host, 0, MAXHOSTNAMELEN + 1);
            printf( "Host: " );
            (void)fgets (options.host, MAXHOSTNAMELEN, stdin );
            if ( *options.host != '\0' ) {
                options.host[strlen(options.host) - 1] = '\0';
            }
            if (*options.host == '\0') {
                STONESH_FREE(options.host);
            }
        }
    }

    /* After all that, if we still don't have a host ... quit */
    if ( options.host == NULL ) {
        error("no host was specified, exiting");
        status = EXIT_FAILURE;
        goto clean_exit;
    } 

    /* We only want the hostname */
    if ( options.allowfqdn == 0 ) {
       host = strtok( options.host, "." );
       strncpy( options.host, host, strlen(host) );
    }

    /*@-null@*/
    debug("host = %s", options.host);
    debug("cval = %s", cval);
    /*@+null@*/
   
    /* get the hostentry structure */
    /*@-null@*/
    hostent = gethostentry( options.host, options.hosts_file );
    /*@+null@*/
    if ( hostent == NULL ) {
        error( "(%s) requested invalid host '%s', exiting", pwd->pw_name, options.host );
        status = EXIT_FAILURE;
        goto clean_exit;
    } else {
        debug( "(%s) requested valid host '%s:%s' (%s)",
            pwd->pw_name, hostent->host, hostent->port, hostent->countries );
    }

    /* determine if we're authorized. */
    if ( cval == NULL ) {
        auth = auth_user( pwd->pw_name, hostent->host, AUTH_BASTION );
    } else {
        if ( strncmp( cval, options.host, strlen(cval) ) == 0 ) {
            auth = auth_user( pwd->pw_name, hostent->host, AUTH_BASTION );
        } else {
            auth = auth_user( pwd->pw_name, hostent->host, AUTH_PROXY );
        }
    }

    /* do we need to validate the client's country */
    if ( auth != 0 && hostent->countries != NULL && rhost != NULL ) {
        /*@-null@*/
        auth = is_from_valid_country( rhost, hostent->countries, "," );        
        /*@+null@*/
        if ( auth == 0 ) {
            error( "(%s) not authorized for '%s' from '%s'",
                pwd->pw_name, options.host, rhost );
        } else {
            debug( "(%s) authorized for '%s' from '%s'",
                pwd->pw_name, options.host, rhost );
        }
    }

    /* log a message stating  that we're not authorized for requested mode */
    if ( auth == 0 ) {
        if ( proxy == 0 ) {
            error("(%s) not authorized to access host '%s', exiting",
                pwd->pw_name, options.host);
        } else {
            error("(%s) not authorized for proxy access to host '%s', exiting",
                pwd->pw_name, options.host);
        }
        goto clean_exit;
    }

    /* ok, we're authorized so log a message as such if we're debugging  */
    if ( proxy == 0 ) {
        debug( "(%s) authorized for access to host '%s'", 
            pwd->pw_name, options.host );
    } else {
        debug( "(%s) authorized for proxy access to host '%s'", 
            pwd->pw_name, options.host );
    }
 
    if ( proxy == 1 ) {
        /* Build the initial elements for our execvp call */
        newargc = 0;
        /*@-onlytrans@*//*@-observertrans@*/
        newargv[newargc] = options.nc_path;    newargc++;
        newargv[newargc] = "-w";               newargc++;
        newargv[newargc] = options.nc_timeout; newargc++;
        newargv[newargc] = hostent->host;      newargc++;
        newargv[newargc] = hostent->port;      newargc++;
        newargv[newargc] = NULL;
        /*@+onlytrans@*//*@+observertrans@*/
    } else {
        /* Make sure we have a tty or have specified a command */
        if ( ( isatty(STDIN_FILENO) == 0 ) && (args == NULL)) {
            error("(%s) has no TTY and did not specify a command to be run", pwd->pw_name);
            fprintf( stderr, "You have no TTY and you have not specified a command " );
            fprintf( stderr, "to be run on the remote host.  Try again with ssh -t\n" );
            status = EXIT_FAILURE;
            goto clean_exit;
        }

        /* Build the initial elements for our execvp call */
        newargc = 0;
        /*@-onlytrans@*//*@-observertrans@*/
        newargv[newargc] = options.ssh_path;  newargc++;
        newargv[newargc] = "-p";              newargc++;
        newargv[newargc] = hostent->port;     newargc++;
        newargv[newargc] = "-q";              newargc++;
        newargv[newargc] = hostent->host;     newargc++;
        newargv[newargc] = "--";              newargc++;
        newargv[newargc] = NULL;
        /*@+onlytrans@*//*@+observertrans@*/

        /* Copy in the remaining command-line arguments */
        if (args != NULL) {
            newargv[newargc] = args; newargc++;
            newargv[newargc] = NULL;
        }
    }

    /* Concatenate newargv for logging output */
    for (c = 0; c < newargc; c++) {
        if ( newargv[c] != NULL ) {
            newargv_len += strlen(newargv[c]);
        }
    }
    newcmdline = (char*)malloc(newargv_len + (newargc * 4));
    if ( newcmdline != NULL ) {
        memset(newcmdline, 0, newargv_len + (newargc * 4));
        strncpy(newcmdline, newargv[0], strlen(newargv[0]));
        for (c = 1; c < newargc; c++) {
            if ( newargv[c] != NULL ) {
                strncat(newcmdline, " \"", 2);
                strncat(newcmdline, newargv[c], strlen(newargv[c]));
                strncat(newcmdline, "\"", 1);
            }
        }
    }

    /* Execute the command */
    notice( "(%s) executing '%s'", pwd->pw_name, newcmdline );
    /*@-null@*/
    if( execvp( newargv[0], newargv ) < 0 ) {
    /*@+null@*/
        error( "(%s) Unable to execute command: %s", pwd->pw_name, strerror(errno) );
        status = EXIT_FAILURE;
        goto clean_exit;        /* unnecessary, I know */
    }

clean_exit:
    if ( hostent != NULL ) { 
        HOSTS_FREE( hostent->host );
        HOSTS_FREE( hostent->port );
        HOSTS_FREE( hostent->countries );
    }
    free_options( &options );
    /*@-mustfreefresh@*/
    return( status );
    /*@-mustfreefresh@*/
}

void version()
{
    fprintf( stderr, "%s v%s\n", PACKAGE_NAME, PACKAGE_VERSION );
}

void usage()
{
    version();
    printf( "Usage: %s [ OPTIONS ]\n\n", progname );
    printf( "Where:\n" );
    printf( "   -h               - Print this message\n" );
    printf( "   -c <command>     - Commands to be run\n" );
    printf( "   -C <configfile>  - Alternate configuration file\n");
    printf( "   -l               - Log to stderr (for debugging)\n");
    printf( "   -v               - Print version information\n");
}

/*
 * catch_signal - signal handler
 */
void catch_signal( int sigval )
{
    char *sig;

    switch ( sigval ) {
        case SIGINT:    sig = "SIGINT";     break;
        case SIGTERM:   sig = "SIGTERM";    break;
        default:        sig = "UNKNOWN";    break;
    }

    fatal( "caught signal %s (%d), exiting", sig, sigval);
}
