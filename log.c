/*
 * log.c
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

#include "log.h"

int log_to_stderr = 0;
int debugging = 0;

extern char *progname;

static void do_log(const char*, const char *, va_list);

/* general notices */
void notice(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    do_log("notice", fmt, args);
    va_end(args);
}

/* general error */
void error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    do_log("error", fmt, args);
    va_end(args);
}

/* fatal error */
void fatal(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    do_log("fatal", fmt, args);
    va_end(args);

    EXIT( EXIT_FAILURE );
}

/* debug messages */
void debug(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    if (debugging == 1) {
	    do_log("debug", fmt, args);
    }
    va_end(args);
}

void do_log(const char *txt, const char *fmt, va_list args)
{
    char msgbuf[LOG_BUF_LEN];
    int log_priority = LOG_INFO;

    /* Insert the tag into the format string */
    if (txt != NULL) {
        /* Allocate a temporary buffer */
	    size_t fmtlen = strlen(txt) + strlen(fmt) + 3;
        char  *fmtbuf = (char *) malloc(fmtlen);

        /* Set the tag if we have a buffer, otherwise skip the tag */
        if ( fmtbuf != NULL ) {
	        memset(fmtbuf, 0, fmtlen);
	        (void)snprintf(fmtbuf, fmtlen, "%s: %s", txt, fmt);
	        (void)vsnprintf(msgbuf, sizeof(msgbuf), fmtbuf, args);
	        free(fmtbuf);
        } else {
	        (void)vsnprintf(msgbuf, sizeof(msgbuf), fmt, args);
        }
    } else {
	    (void)vsnprintf(msgbuf, sizeof(msgbuf), fmt, args);
    }

    /* Send the message to STDERR if requested */
    if (log_to_stderr == 1) {
	    fprintf(stderr, "%s\r\n", msgbuf);
    }

    /* Send the message to syslog */
    openlog(progname, LOG_PID, LOG_DAEMON);
    syslog(log_priority, "%s", msgbuf);
    closelog();
}
