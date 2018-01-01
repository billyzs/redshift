/**
    location-provider.hpp
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
    Copyright (c) 2013-2014    Jon Lund Steffensen <jonlst@gmail.com>
*/
#pragma once

/* Location */
typedef struct {
	float lat;
	float lon;
} location_t;

/* Location provider */
typedef struct location_state location_state_t;

typedef int location_provider_init_func(location_state_t **state);
typedef int location_provider_start_func(location_state_t *state);
typedef void location_provider_free_func(location_state_t *state);
typedef void location_provider_print_help_func(FILE *f);
typedef int location_provider_set_option_func(
		location_state_t *state, const char *key, const char *value);
typedef int location_provider_get_fd_func(location_state_t *state);
typedef int location_provider_handle_func(
		location_state_t *state, location_t *location, int *available);

typedef struct {
	char *name;

	/* Initialize state. Options can be set between init and start. */
	location_provider_init_func *init;
	/* Allocate storage and make connections that depend on options. */
	location_provider_start_func *start;
	/* Free all allocated storage and close connections. */
	location_provider_free_func *free;

	/* Print help on options for this location provider. */
	location_provider_print_help_func *print_help;
	/* Set an option key, value-pair. */
	location_provider_set_option_func *set_option;

	/* Listen and handle location updates. */
	location_provider_get_fd_func *get_fd;
	location_provider_handle_func *handle;
} location_provider_t;
