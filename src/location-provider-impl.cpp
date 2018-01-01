/**
    location-provider-impl.cpp
    This file is part of yellowstone
    yellowstone is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    yellowstone is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with yellowstone. If not, see <http://www.gnu.org/licenses/>.
    
    Copyright (c) 2017-2018    Billy Zhou <billyzs.728@gmail.com>
    Copyright (c) 2013-2017    Jon Lund Steffensen <jonlst@gmail.com>
*/


#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>

// #include <limits>

#include <yellowstone/location-provider-impl.hpp>

template<typename T>
Location<T>::Location() noexcept :
		lat{std::numeric_limits<T>::quiet_NaN()},
        lon{std::numeric_limits<T>::quiet_NaN()},
        valid{false} {};

template <typename T>
bool Location<T>::isLatValid(const T &min, const T &max, const T &input) const {
	if ((input < MIN_LAT) || (input > MAX_LAT)) {
		std::cerr << std::setprecision(4) << "Latitude must be between" << min <<  "and " << max << std::endl;
		return false;
	}
	return true;
}

template <typename T>
bool Location<T>::isLonValid(const T &min, const T &max, const T &input) const {
	if ((input < min) || (input > max)) {
		std::cerr << std::setprecision(4) << "Latitude must be between" << min << "and " << max << std::endl;
		return false;
	}
	return true;
}

template <typename T>
bool Location<T>::isValid(const T& lat_min, const T& lat_max, const T& lon_min, const T& lon_max) const {
	return isLatValid(lat_min, lat_max, lat) && isLonValid(lon_min, lon_max, lon);
}
template  <typename T>
std::string&& Location<T>::toStr() const {
	std::ostringstream os;
	os << std::setprecision(4) << "Location: "
	   << fabs(lat) << ((lat > 0) ? "N" : "S")
	   << ","
	   << fabs(lon) << ((lon > 0) ? "E" : "W")
	   << std::endl;
	return os.str();
}

template <typename T>
void Location<T>::setLat(const T &lat_) {
	if (isLatValid(MIN_LAT, MAX_LAT, lat_)) {
		lat = lat_;
	}
}

template <typename T>
void Location<T>::setLon(const T &lon_) {
	if (isLonValid(MIN_LON, MAX_LON, lon_)) {
		lon = lon_;
	}
}

LocationProviderOption::LocationProviderOption(const location_T& loc_) : loc(loc_) {}
LocationProvider::LocationProvider() {}; //NOP for now;

LocationProviderManual::LocationProviderManual() :
	option{std::make_unique<LocationProviderOption>()}
{}

LocationProviderManual::LocationProviderManual(const LocationProviderOption& option_) :
	option{std::make_unique<LocationProviderOption>(option_)}
{}

void LocationProviderManual::init() const {
	option->loc.isValid();
}

void LocationProviderManual::start() {
	option->available = true;
}
void LocationProviderManual::printHelp() const {
	std::cout << "Location Provider " << name << ":"<< std::endl
	          << "Specify location manually in the format -l LAT:LON." << std::endl
	          << "LAT and LON are expected to be floating point numbers." << std::endl
			  << "negative values representing west / south, respectively" << std::endl;
}

void LocationProviderManual::setLat(const double &lat) noexcept {
	option->loc.setLat(lat);
}

void LocationProviderManual::setLon(const double &lon) noexcept {
	option->loc.setLon(lon);
}
