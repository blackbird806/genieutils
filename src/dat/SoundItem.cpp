/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "genie/dat/SoundItem.h"

#include "TestHelpers.h"

#include <string.h>

namespace genie {

unsigned short SoundItem::getFileNameSize()
{
    if (getGameVersion() < GV_SWGB) {
        return 13;
    }

    return 27;
}

bool SoundItem::compareTo(const SoundItem &other) const
{
    COMPARE_MEMBER(FileName);
    COMPARE_MEMBER(ResourceID);
    COMPARE_MEMBER(Probability);
    COMPARE_MEMBER(Civilization);
    COMPARE_MEMBER(IconSet);
    return true;
}

void SoundItem::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serialize(FileName, getFileNameSize());
    } else {
        serializeDebugString(FileName);
    }

    if (gv < GV_TEST) {
        int16_t res = ResourceID;
        serialize<int16_t>(res);
        ResourceID = res;
    } else {
        serialize<int32_t>(ResourceID);
    }

    serialize<int16_t>(Probability);

    if (gv >= GV_AoKE3) {
        serialize<int16_t>(Civilization);
        serialize<int16_t>(IconSet);
    }
}
} // namespace genie
