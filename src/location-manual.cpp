/* location-manual.c -- Manual location provider source
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

   Copyright (c) 2017-2018  Billy Zhou <billyzs.728@gmail.com>
   Copyright (c) 2010-2017  Jon Lund Steffensen <jonlst@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#include "location-manual.h"
#include <yellowstone/location-provider-impl.hpp>
//#ifdef ENABLE_NLS
//# include <libintl.h>
//# define _(s) gettext(s)
//#else
//# define _(s) s
//#endif
# define _(s) s

typedef struct {
	location_t loc;
} location_manual_state_t;

static auto uPtrLPM = std::make_unique<LocationProviderManual>();

static int
location_manual_init(location_manual_state_t **state)
{
	*state = reinterpret_cast<location_manual_state_t*>(malloc(sizeof(location_manual_state_t)));
	if (*state == NULL) return -1;

	location_manual_state_t *s = *state;
	s->loc.lat = NAN;
	s->loc.lon = NAN;

	location_T loc{};
	loc.setLon(42.0);
	loc.setLat(71.0);
	const auto opt = LocationProviderOption(std::move(loc));
	uPtrLPM = std::make_unique<LocationProviderManual>(opt);
	return 0;
}

static int
location_manual_start(location_manual_state_t *state)
{
	/* Latitude and longitude must be set */
	if (isnan(state->loc.lat) || isnan(state->loc.lon)) {
		fputs(_("Latitude and longitude must be set.\n"), stderr);
		exit(EXIT_FAILURE);
	}

	uPtrLPM->init();
	return 0;
}

static void
location_manual_free(location_manual_state_t *state)
{

	free(state);
}

static void
location_manual_print_help(FILE *f)
{
	fputs(_("Specify location manually.\n"), f);
	fputs("\n", f);

	/* TRANSLATORS: Manual location help output
	   left column must not be translated */
	fputs(_("  lat=N\t\tLatitude\n"
		"  lon=N\t\tLongitude\n"), f);
	fputs("\n", f);
	fputs(_("Both values are expected to be floating point numbers,\n"
		"negative values representing west / south, respectively.\n"), f);
	fputs("\n", f);
	uPtrLPM->printHelp();
}

static int
location_manual_set_option(location_manual_state_t *state, const char *key,
			   const char *value)
{
	/* Parse float value */
	char *end;
	errno = 0;
	float v = strtof(value, &end);
	if (errno != 0 || *end != '\0') {
		fputs(_("Malformed argument.\n"), stderr);
		return -1;
	}

	if (strcasecmp(key, "lat") == 0) {
		state->loc.lat = v;
		uPtrLPM->setLat(static_cast<double>(v));
	} else if (strcasecmp(key, "lon") == 0) {
		state->loc.lon = v;
		uPtrLPM->setLon(static_cast<double>(v));
	} else {
		fprintf(stderr, _("Unknown method parameter: `%s'.\n"), key);
		return -1;
	}

	return 0;
}

static int
location_manual_get_fd(location_manual_state_t *state)
{
	return -1;
}

static int
location_manual_handle(
	location_manual_state_t *state, location_t *location, int *available)
{
	*location = state->loc;
	*available = 1;

	return 0;
}


const location_provider_t manual_location_provider = {
	strdup("manual"),
	(location_provider_init_func *)location_manual_init,
	(location_provider_start_func *)location_manual_start,
	(location_provider_free_func *)location_manual_free,
	(location_provider_print_help_func *)location_manual_print_help,
	(location_provider_set_option_func *)location_manual_set_option,
	(location_provider_get_fd_func *)location_manual_get_fd,
	(location_provider_handle_func *)location_manual_handle
};
