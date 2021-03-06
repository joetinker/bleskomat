/*
	Copyright (C) 2020 Samotari (Charles Hill, Carlos Garcia Ortiz)
	Contributor: Joe Tinker

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define STRINGIFY(s) STRINGIFY1(s)
#define STRINGIFY1(s) #s

#ifndef API_KEY_ID
	#error "Missing required build flag: API_KEY_ID"
#endif

#ifndef API_KEY_SECRET
	#error "Missing required build flag: API_KEY_SECRET"
#endif

#ifndef CALLBACK_URL
	#error "Missing required build flag: CALLBACK_URL"
#endif

#ifndef FIAT_CURRENCY
	#error "Missing required build flag: FIAT_CURRENCY"
#endif

#ifndef DEFAULT_DESCRIPTION
	#define DEFAULT_DESCRIPTION ""
#endif

#include <string>
#include "logger.h"

namespace config {
	extern std::string apiKeyId;
	extern std::string apiKeySecret;
	extern std::string callbackUrl;
	extern std::string fiatCurrency;
	extern std::string defaultDescription;
	void init();
}
