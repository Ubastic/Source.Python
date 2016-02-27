/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

#ifndef _STEAM_WRAP_H
#define _STEAM_WRAP_H

//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------
class CSteamIDExt
{
public:
	static CSteamID* Parse(const char* input)
	{
		if (!input || input[0] == '\0')
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Input is empty.")

		uint32 account_id;
		uint32 universe;

		// Try SteamID2
		if (sscanf(input, "STEAM_0:%u:%u", &universe, &account_id) == 2) {
			account_id = account_id * 2 + universe;
		}

		// Try SteamID3
		else if (sscanf(input, "[U:1:%u]", &account_id) == 1) {
			universe = account_id % 2;
		}

		// Try SteamID64
		else {
			uint64 ulSteamid64;
			try
			{
				ulSteamid64 = boost::lexical_cast<uint64>(input);
			}
			catch(const boost::bad_lexical_cast &)
			{
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Input '%s' is not a SteamID2, SteamID3 or SteamID64 value.", input)
			}

			return new CSteamID(ulSteamid64);
		}
	
		return new CSteamID(account_id, (EUniverse) (universe + 1), k_EAccountTypeIndividual);
	}
};

#endif // _STEAM_WRAP_H
