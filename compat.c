/*
 * MOC - music on console
 * Copyright (C) 2005 Damian Pietras <daper@daper.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <pthread.h>
#include <log.h>
#include <string.h>
#include <errno.h>

#include "common.h"

/* Various functions that some systems lack of. */

#ifndef HAVE_STRERROR_R
static pthread_mutex_t strerror_r_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifndef HAVE_STRERROR_R
int strerror_r(int errnum, char *buf, size_t n)
{
	char *err_str;
	int ret_val = 0;
	
	LOCK (strerror_r_mutex);
	
	err_str = strerror (errnum);
	if (strlen(err_str) >= n) {
		errno = ERANGE;
		ret_val = -1;
	}
	else
		strcpy (buf, err_str);
	
	UNLOCK (strerror_r_mutex);

	return ret_val;
}
#endif

void compat_cleanup ()
{
#ifndef HAVE_STRERROR_R
	if (pthread_mutex_destroy(&strerror_r_mutex))
		logit ("Can't destroy strerror_r_mutex: %s", strerror(errno));
#endif

}
