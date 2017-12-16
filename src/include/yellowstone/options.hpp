/**
 	options.hpp -- Program options header

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
	Copyright (c) 2017  	   Jon Lund Steffensen <jonlst@gmail.com>
*/

#pragma once

#include "redshift.h"
#include <yellowstone/location-provider.hpp>
#include <yellowstone/config-ini.h>
#include <yellowstone/solar.hpp>

#include <string>
#include <numeric>
#include <unordered_map>

/* Angular elevation of the sun at which the color temperature
   transition period starts and ends (in degress).
   Transition during twilight, and while the sun is lower than
   3.0 degrees above the horizon. */
constexpr auto     TRANSITION_LOW_DEG     = SOLAR_CIVIL_TWILIGHT_ELEV;
constexpr double   TRANSITION_HIGH_DEG      {3.0};

/* default values for some params */
constexpr uint16_t DEFAULT_DAY_TEMP_K     {6500};
constexpr uint16_t DEFAULT_NIGHT_TEMP_K   {3500};
constexpr double   DEFAULT_BRIGHTNESS     {1.0}; //unitless
constexpr double   DEFAULT_GAMMA          {1.0}; //unitless

/**
 * This layer of abstraction allows client code to change what cmdline arguments to use without having to recompile
 * options.cpp - they just have to change the OptionsMapping in the header; furthermore, the cmdline parser and config file
 * parser can share a lot of code - different parsers just have to devide on how to map key type to a YellowstoneOption Type
 */
enum class YellowStoneOption: uint8_t {
//  NAME                   FLAG     ARG         EXPLANATION

	Verbose,            // -v       (none)      Verbose output
	Brightness,         // -b       DAY:NIGHT	Screen brightness to apply (between 0.1 and 1.0)
	ConfigFile,         // -c       FILE	    Load settings from specified configuration file
	Provider,           // -L       PROVIDER	Select provider for automatic location updates
	GammaCorrection,    // -g       R:G:B	    Additional gamma correction to apply
	Location,           // -l       LAT:LON	    Your current location
	GammaMethod,        // -m       METHOD	    Method to use to set color temperature (Type 'list' to see available methods)
	Transition,         // -r   	(none) 	    Disable temperature transitions
	Temperature         // -t       DAY:NIGHT	Color temperature to set at daytime/night
};

static auto cmdLineOptMapping = std::unordered_map<char, const YellowStoneOption> {
		{'v', YellowStoneOption::Verbose},
		{'b', YellowStoneOption::Brightness},
		{'c', YellowStoneOption::ConfigFile},
		{'L', YellowStoneOption::Provider},
		{'g', YellowStoneOption::GammaCorrection},
		{'l', YellowStoneOption::Location},
		{'m', YellowStoneOption::GammaMethod},
		{'r', YellowStoneOption::Transition},
		{'t', YellowStoneOption::Temperature}
};

/**
 * Uh, can't think of a clean way of doing this because C++ doesn't support nested enum classes; these opts are
 * technically a "specialization" YellowStoneOption, I would like to include them in the map above but have a member of
 * type "YellowStoneOption::ProgramMode" in Options where you can only pass in ProgramMode enum, and not the rest of
 * YellowStoneOption... sadly this can't be done :(
 */
enum class ProgramMode : uint8_t {
//  NAME		   FLAG	    ARG     EXPLANATION

	Reset,      // -x       (none)	Reset mode (remove adjustment from screen)
	Print,      // -p       (none)	Print mode (only print parameters and exit)
	OneShot,    // -o       (none)	One shot mode (do not continuously adjust color temperature)
	Manual,     // -O       TEMP	One shot manual mode (set color temperature)
	Continual   // (none)           When none of the above is set
};

static auto modeOptMapping = std::unordered_map<char, const ProgramMode> {
		{'x', ProgramMode::Reset},
		{'p', ProgramMode::Print},
		{'o', ProgramMode::OneShot},
		{'O', ProgramMode::Manual}
};

/* Time range.
   Fields are offsets from midnight in seconds. */
struct TimeRange {
	// Nope, can't use auto on member var unless they are const static
	int start   {std::numeric_limits<int>::min()};
	int end     {std::numeric_limits<int>::min()};
};

/* Transition scheme.
   The solar elevations at which the transition begins/ends,
   and the association color settings. */
struct TransitionScheme {
	double high{TRANSITION_HIGH_DEG};
	double low{TRANSITION_LOW_DEG};
	bool useTime{false}; /* When enabled, ignore elevation and use time ranges. */
	TimeRange dawn{};
	TimeRange dusk{};
	color_setting_t day;
	color_setting_t night;
};

class Options {
public:
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

	Options(){
		init();
	}

	// disable copy & move
	Options(const Options& other) = delete;
	Options& operator=(const Options& other) = delete;

	void init();
	void setDefaults();
	void parseArgs();
	void parseConfigFile();
};

// TODO DOC
void options_parse_args(Options *options,
                        int argc, char *argv[],
                        const gamma_method_t *gamma_methods,
                        const location_provider_t *location_providers);

void options_parse_config_file(Options *options,
                               config_ini_state_t *config_state,
                               const gamma_method_t *gamma_methods,
                               const location_provider_t *location_providers);
