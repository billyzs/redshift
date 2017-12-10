/* systemtime.h -- Portable system time header
   This file is part of Redshift.

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

   Copyright (c) 2010-2014  Jon Lund Steffensen <jonlst@gmail.com>
*/

#ifndef REDSHIFT_SYSTEMTIME_H
#define REDSHIFT_SYSTEMTIME_H

/**
 * @brief calculates the time in seconds since unix epoch
 * @param[out] now: the output
 * @return < 0 if errored
 */
int systemtime_get_time(int64_t &now);
/** @brief sleep for X millisecondss
 * @param msecs
 */
void systemtime_msleep(const unsigned int &msecs);

#endif /* ! REDSHIFT_SYSTEMTIME_H */
