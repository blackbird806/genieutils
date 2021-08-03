/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P

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

#include "genie/dat/UnitHeader.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
void UnitHeader::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(TaskList);
}

bool UnitHeader::compareTo(const UnitHeader &other) const
{
    COMPARE_MEMBER(Exists);
    COMPARE_MEMBER_OBJ_VEC(TaskList);

    return true;
}

//------------------------------------------------------------------------------
void UnitHeader::serializeObject()
{
<<<<<<< HEAD
    serialize<int8_t>(Exists);
=======
  serialize<uint8_t>(Exists);

  if (Exists)
  {
    int16_t task_count;
    serializeSize<int16_t>(task_count, TaskList.size());
    serializeSub<Task>(TaskList, task_count);
  }
}
>>>>>>> 65dd660 (More accurate signedness.)

    if (Exists) {
        uint16_t task_count{};
        serializeSize<uint16_t>(task_count, TaskList.size());
        serialize(TaskList, task_count);
    }
}
} // namespace genie
