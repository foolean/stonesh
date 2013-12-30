/*
 * log.h
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

#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>		
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>	

#ifndef LOG_BUF_LEN
#define LOG_BUF_LEN 1024
#endif

#ifndef EXIT
#define EXIT(x)    exit(x)
#endif

/*@-exportlocal@*/
extern int log_to_stderr;
extern int debugging;
/*@+exportlocal@*/

void error(const char *, ...);
void fatal(const char *, ...);
void notice(const char *, ...);
void debug(const char *, ...);

#endif /* _LOG_H */
