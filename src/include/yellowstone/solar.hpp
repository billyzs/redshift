/**
    solar.hpp -- Solar position header
    This file is part of yellowstone
    yellowstone is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    REDSHIFT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with REDSHIFT. If not, see <http://www.gnu.org/licenses/>.

    Copyright (c) 2017-2018    Billy Zhou <billyzs.728@gmail.com>
    Copyright (c) 2010-2017    Jon Lund Steffensen <jonlst@gmail.com>
*/

#ifndef REDSHIFT_SOLAR_H
#define REDSHIFT_SOLAR_H

#include <stdint.h>

/* Model of atmospheric refraction near horizon (in degrees). */
#define SOLAR_ATM_REFRAC  0.833

#define SOLAR_ASTRO_TWILIGHT_ELEV   -18.0
#define SOLAR_NAUT_TWILIGHT_ELEV    -12.0
#define SOLAR_CIVIL_TWILIGHT_ELEV    -6.0
#define SOLAR_DAYTIME_ELEV           (0.0 - SOLAR_ATM_REFRAC)

//TODO option.h isn't cpp yet and it includes this file; change this to constexpr noexcept and pass by reference

/** @brief Solar angular elevation at the given location and time.
 * @param date Seconds since unix epoch
 * @param lat Latitude of location
 * @param lon: Longitude of location
 * @return Solar angular elevation in degrees
 */
double solar_elevation(const int64_t date, const float lat, const float lon);

#endif /* ! REDSHIFT_SOLAR_H */
