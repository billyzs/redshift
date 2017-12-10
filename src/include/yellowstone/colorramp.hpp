/**
    colorramp.hpp
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
    
    Copyright (c) 2017-2017    Billy Zhou <billyzs.728@gmail.com>
    Copyright (c) 2010-2014    Jon Lund Steffensen <jonlst@gmail.com>
*/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifndef REDSHIFT_COLORRAMP_H
#define REDSHIFT_COLORRAMP_H

#include <stdint.h>

#include "redshift.h"

void colorramp_fill(uint16_t *gamma_r, uint16_t *gamma_g, uint16_t *gamma_b,
                    int size, const color_setting_t *setting);
#endif /* ! REDSHIFT_COLORRAMP_H */

#ifdef __cplusplus
};
#endif
