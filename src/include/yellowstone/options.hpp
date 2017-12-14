/* options.h -- Program options header
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

   Copyright (c) 2017  Jon Lund Steffensen <jonlst@gmail.com>
*/

#pragma once

#include "redshift.h"
#include <yellowstone/location-provider.hpp>
#include <yellowstone/config-ini.h>
#include <yellowstone/solar.hpp>

#include <string>
#include <numeric>

/* Angular elevation of the sun at which the color temperature
   transition period starts and ends (in degress).
   Transition during twilight, and while the sun is lower than
   3.0 degrees above the horizon. */
constexpr auto   TRANSITION_LOW     = SOLAR_CIVIL_TWILIGHT_ELEV;
constexpr double TRANSITION_HIGH      {3.0};
/* default values for params */
constexpr uint16_t DEFAULT_DAY_TEMP   {6500};
constexpr uint16_t DEFAULT_NIGHT_TEMP {4500};
constexpr double DEFAULT_BRIGHTNESS   {1.0};
constexpr double DEFAULT_GAMMA        {1.0};

enum class ProgramMode : uint8_t {
	Reset = 0,
	Print,
	OneShot,
	Manual,
	Continual
};

// TODO Consider moving inside \ref Options
/* Time range.
   Fields are offsets from midnight in seconds. */
struct TimeRange {
	// Nope, can't use auto on member var unless they are const static
	int start = std::numeric_limits<int>::min();
	int end   = std::numeric_limits<int>::min();
};

/* Transition scheme.
   The solar elevations at which the transition begins/ends,
   and the association color settings. */
struct TransitionScheme {
	double high{TRANSITION_HIGH};
	double low{TRANSITION_LOW};
	bool useTime{false}; /* When enabled, ignore elevation and use time ranges. */
	TimeRange dawn{};
	TimeRange dusk{};
	color_setting_t day;
	color_setting_t night;
};

struct Options {
	char* config_filepath{nullptr};  /* Path to config file */
	TransitionScheme scheme;
	ProgramMode mode{ProgramMode::Continual};
	int tempManual = std::numeric_limits<int>::min(); /* Temperature to set in manual mode. */
	bool verbose{true};
	bool useFade{true}; 	/* Whether to fade between large skips in color temperature. */
	bool preserveGamma{true}; /* Whether to preserve gamma ramps if supported by gamma method. */
	// TODO Unique ptr
	const gamma_method_t* method{nullptr}; /* Selected gamma method. */
	// TODO use bitfields
	char* method_args{nullptr}; /* Arguments for gamma method. */
	// TODO Unique ptr
	const location_provider_t* provider{nullptr}; /* Selected location provider. */
	// TODO use bitfields
	char* provider_args{nullptr}; /* Arguments for location provider. */

	// TODO add move ctor, disable copy ctor
	void init();
	void setDefaults();
	void parseArgs();
	void parseConfigFile();
};

// TODO DOC
void options_init(Options *options);

void options_parse_args(Options *options,
                        int argc, char *argv[],
                        const gamma_method_t *gamma_methods,
                        const location_provider_t *location_providers);

void options_parse_config_file(Options *options,
                               config_ini_state_t *config_state,
                               const gamma_method_t *gamma_methods,
                               const location_provider_t *location_providers);

void options_set_defaults(Options *options);
