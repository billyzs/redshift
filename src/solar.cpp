/**
    solar.cpp -- Solar position source
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

/* Ported from javascript code by U.S. Department of Commerce,
   National Oceanic & Atmospheric Administration:
   http://www.srrb.noaa.gov/highlights/sunrise/calcdetails.html
   It is based on equations from "Astronomical Algorithms" by
   Jean Meeus. */

#include <cmath>

#include <yellowstone/solar.hpp>

constexpr double degToRad(const double& x) noexcept {return x / 180 * M_PI;}
constexpr double radToDeg(const double& x) noexcept {return x * 180 / M_PI;}

/** @param t minutes since unix epoch
 *  @return Julian day from unix epoch */
static constexpr double
jd_from_epoch(const int64_t& t) noexcept
{
	return (t / 86400.0) + 2440587.5;
}

/** @param jd: Julian Day since epoch
 *  @breturn Julian centuries since J2000.0 from Julian day */
static constexpr double
jcent_from_jd(const double& jd) noexcept
{
	return (jd - 2451545.0) / 36525.0;
}

/** @brief Julian day from Julian centuries since J2000.0
 *  @param t: Julian Century since J2000.0
 *  @return Julian day from Julian centuries since J2000.0
 * */
static constexpr double
jd_from_jcent(const double& t) noexcept
{
	return 36525.0 * t + 2451545.0;
}

/** @breif Geometric mean longitude of the sun.
    @param t: Julian centuries since J2000.0
    @return: Geometric mean logitude in radians. */
static constexpr double
sun_geom_mean_lon(const double& t) noexcept
{
	return degToRad(std::fmod(280.46646 + t*(36000.76983 + t*0.0003032), 360));
}

/** @breif Geometric mean anomaly of the sun.
    @param t: Julian centuries since J2000.0
    @return: Geometric mean anomaly in radians. */
static constexpr double
sun_geom_mean_anomaly(const double& t) noexcept
{
	return degToRad(357.52911 + t*(35999.05029 - t*0.0001537));
}

/** @brief Eccentricity of earth orbit.
    @param t: Julian centuries since J2000.0
    @return: Eccentricity (unitless). */
static constexpr double
earth_orbit_eccentricity(const double& t) noexcept
{
	return 0.016708634 - t*(0.000042037 + t*0.0000001267);
}

/** @brief Equation of center of the sun.
    @param t: Julian centuries since J2000.0
    @return: Center(?) in radians */
static constexpr double
sun_equation_of_center(const double& t) noexcept
{
	/* Use the first three terms of the equation. */
	double m{sun_geom_mean_anomaly(t)};
	double c{sin(m)*(1.914602 - t*(0.004817 + 0.000014*t)) +
		sin(2*m)*(0.019993 - 0.000101*t) +
		sin(3*m)*0.000289};
	return degToRad(c);
}

/** @brief True longitude of the sun.
    @param t: Julian centuries since J2000.0
    @return: True longitude in radians */
static constexpr double
sun_true_lon(const double&t) noexcept
{
	double l_0{sun_geom_mean_lon(t)};
	double c{sun_equation_of_center(t)};
	return l_0 + c;
}

/** @brief Apparent longitude of the sun. (Right ascension).
    @param t: Julian centuries since J2000.0
    @return: Apparent longitude in radians */
static constexpr double
sun_apparent_lon(const double& t) noexcept
{
	double o{sun_true_lon(t)};
	return degToRad(radToDeg(o) - 0.00569 - 0.00478*sin(degToRad(125.04 - 1934.136*t)));
}

/** @brief Mean obliquity of the ecliptic
    @param t: Julian centuries since J2000.0
    @return: Mean obliquity in radians */
static constexpr double
mean_ecliptic_obliquity(const double& t) noexcept
{
	double sec{21.448 - t*(46.815 + t*(0.00059 - t*0.001813))};
	return degToRad(23.0 + (26.0 + (sec/60.0))/60.0);
}

/** @brief Corrected obliquity of the ecliptic.
    @param t: Julian centuries since J2000.0
    @return: Currected obliquity in radians
*/
static constexpr double
obliquity_corr(const double& t) noexcept
{
	double e_0{mean_ecliptic_obliquity(t)};
	double omega{125.04 - t*1934.136};
	return degToRad(radToDeg(e_0) + 0.00256*cos(degToRad(omega)));
}

/** @brief Declination of the sun.
    @param t: Julian centuries since J2000.0
    @return: Declination in radians
*/
static constexpr double
solar_declination(const double& t) noexcept
{
	double e{obliquity_corr(t)};
	double lambda{sun_apparent_lon(t)};
	return asin(sin(e)*sin(lambda));
}


/** @brief Difference between true solar time and mean solar time.
 *
 * @param t: ulian centuries since J2000.0
 * @return Difference in minutes
 */
static constexpr double
equation_of_time(const double& t) noexcept
{
	double epsilon{obliquity_corr(t)};
	double l_0{sun_geom_mean_lon(t)};
	double e{earth_orbit_eccentricity(t)};
	double m{sun_geom_mean_anomaly(t)};
	double y{pow(tan(epsilon/2.0), 2.0)};

	double eq_time{y*sin(2*l_0) - 2*e*sin(m) +
		4*e*y*sin(m)*cos(2*l_0) -
		0.5*y*y*sin(4*l_0) -
		1.25*e*e*sin(2*m)};
	return 4*radToDeg(eq_time);
}
/**
 * @breif return solar elevation at hour of day based on location
 * @param lat: latitude
 * @param decl: Solar declination
 * @param ha
 * @return solar elevation in radian
 */
static constexpr double
elevation_from_hour_angle(const double& lat,
                          const double& decl,
                          const double& ha) noexcept
{
	return asin(cos(ha)*cos(degToRad(lat))*cos(decl) +
		    sin(degToRad(lat))*sin(decl));
}

/**
 * @biref solar elevation in radians
 * @param t minutes since Unix epoch
 * @param lat latitude
 * @param lon longitude
 * @return solar elevation in radians
 */
static constexpr double
solar_elevation_from_time(const double& t, const double& lat, const double& lon) noexcept
{
	/* Minutes from noon */
	double jd{jd_from_jcent(t)};
	double offset{(jd - round(jd) - 0.5)*1440.0};

	double eq_time{equation_of_time(t)};
	double ha{degToRad((720 - offset - eq_time)/4 - lon)};
	double decl{solar_declination(t)};
	return elevation_from_hour_angle(lat, decl, ha);
}

double
solar_elevation(const int64_t date, const float lat, const float lon)
{
	double jd{jd_from_epoch(date)};
	return radToDeg(solar_elevation_from_time(jcent_from_jd(jd), lat, lon));
}

