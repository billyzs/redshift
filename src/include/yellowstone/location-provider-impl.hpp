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

constexpr float     MIN_LAT         {-90.0};
constexpr float     MAX_LAT         {90.0};
constexpr float     MIN_LON         {-180.0};
constexpr float     MAX_LON         {180.0};

#include <iostream>
#include <iomanip>
template<typename T>
struct Location {
	T lat;
	T lon;
	bool isValid(T){
		if (!(lat >= MIN_LAT) || !(lat <= MAX_LAT))
		{
			std::cerr << std::setprecision(3) << "Latitude must be between" << MIN_LAT <<  "and " << MAX_LAT << std::endl;
			return false;
		}
		else if (!(lon >= MIN_LON) || !(lon <= MAX_LON))
		{
			std::cerr << std::setprecision(3) << "Longitude must be between" << MIN_LON <<  "and " << MAX_LON << std::endl;
			return false;
		}
		else
			return true;
	}
};

class LocationProvider {
public:
	virtual void init() ;
private:
};