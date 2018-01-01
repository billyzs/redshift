/* systemtime.c -- Portable system time source
   This file is part of Yellowstone.

   Redshift is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Redshift is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Redshift.  If not, see <http://www.gnu.org/licenses/>.

   Copyright (c) 2017-2018  Billy Zhou <billyzs.728@gmail.com>
   Copyright (c) 2010-2014  Jon Lund Steffensen <jonlst@gmail.com>
*/
//TODO convert to cpp
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#ifndef _WIN32
# if _POSIX_TIMERS > 0
#  include <time.h>
# else
#  include <time.h>
#  include <sys/time.h>
# endif
#else
# include <windows.h>
#endif

#include <yellowstone/systemtime.hpp>

/* Return current time in T as the number of seconds since the epoch. */
using namespace std::chrono;
int systemtime_get_time(int64_t& t)
{
	//TODO verify on other platform
	//TODO unit test

	t = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

#if defined(_WIN32) /* Windows */
	FILETIME now;
	ULARGE_INTEGER i;
	GetSystemTimeAsFileTime(&now);
	i.LowPart = now.dwLowDateTime;
	i.HighPart = now.dwHighDateTime;

	/* FILETIME is tenths of microseconds since 1601-01-01 UTC */
	t = (i.QuadPart / 10000000) - 11644473600;
#elif _POSIX_TIMERS > 0 /* POSIX timers */

	/*struct timespec now;
	int r = clock_gettime(CLOCK_REALTIME, &now);
	if (r < 0) {
		perror("clock_gettime");
		return -1;
	}

	*t = now.tv_sec + (now.tv_nsec / 1000000000.0);*/
#else /* other platforms */
	struct timeval now;
	int r = gettimeofday(&now, NULL);
	if (r < 0) {
		perror("gettimeofday");
		return -1;
	}

	*t = now.tv_sec + (now.tv_usec / 1000000.0);
#endif

	return 0;
}

/* Sleep for a number of milliseconds. */
void systemtime_msleep(const unsigned int &msecs)
{
#ifndef _WIN32
	std::this_thread::sleep_for(milliseconds(msecs));
	/*struct timespec sleep;
	sleep.tv_sec = msecs / 1000;
	sleep.tv_nsec = (msecs % 1000)*1000000;
	nanosleep(&sleep, NULL);*/
#else
	Sleep(msecs);
#endif
}
