/**
    location-provider-impl.hpp
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
    
    Copyright (c) 2017-2017    Billy Zhou <billyzs.728@gmail.com>
    Copyright (c) 2013-2014    Jon Lund Steffensen <jonlst@gmail.com>
*/
#pragma once

constexpr double     MIN_LAT         {-90.0};
constexpr double     MAX_LAT         {90.0};
constexpr double     MIN_LON         {-180.0};
constexpr double     MAX_LON         {180.0};

#include <string>
#include <memory>

// pedantically making these classes more complicated than they need to be
template <typename T>
class Location {
private:
	T lat;
	T lon;
	bool valid;
public:
	Location<T>(void) noexcept ;
	Location<T>(const Location<T>& other) = default;
	Location<T>(Location<T>&& other) = default;
	constexpr Location<T>& operator=(const Location<T>& other) = default;
	constexpr Location<T>& operator=(Location<T>&& other) = default;
	void setLat(const T& lat_);
	void setLon(const T& lon_);
	bool isLatValid(const T &min, const T &max, const T &input) const;
	bool isLonValid(const T &min, const T &max, const T &input) const ;
	bool isValid(const T& lat_min = MIN_LAT, const T& lat_max = MAX_LAT,
	             const T& lon_min = MIN_LON, const T& lon_max = MAX_LON) const;
	std::string&& toStr() const;
};

// TODO turn this on
using location_T = Location<double>;

// mostly to hold data for each location provider
class LocationProviderOption {
public:
	LocationProviderOption() = default;
	//TODO why can't these be const?
	LocationProviderOption(const location_T& loc_);
	location_T loc;
	mutable bool available{false};
};

// might become an issue if a custom deleter needs to be supplied
using uPtrLPO = std::unique_ptr<LocationProviderOption>;
class LocationProvider {
public:
	// Ctors are never inherited; common tasks that are shared with all base class implementations can be put here,
	// and base class ctors can call this
	LocationProvider();

	// Disable copy
	LocationProvider(const LocationProvider& other) = delete;
	LocationProvider& operator=(const LocationProvider& other) = delete;

	// move is OK
	LocationProvider& operator=(LocationProvider&& other) = default;
	LocationProvider(LocationProvider&& other) = default;

	virtual void init() const = 0;
	virtual void start() = 0;
	// This only needs to access the name, so make it static for now
	virtual void printHelp() const = 0;
	virtual ~LocationProvider() = default;

};

class LocationProviderManual : public LocationProvider {
public:
	// could be a variardic template?
	LocationProviderManual();
	LocationProviderManual(const LocationProviderOption& option_);
	void init() const override;
	void start() override;
	void printHelp() const override ;
	~LocationProviderManual() = default;
	void setLat(const double& lat) noexcept;
	void setLon(const double& lon) noexcept;
private:
	std::string name{"Manual"};
	uPtrLPO option;
};
