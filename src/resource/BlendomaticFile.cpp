/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P

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

#include "genie/resource/BlendomaticFile.h"

#include <stdexcept>
#include <chrono>
#include <cmath>

#include "genie/resource/BlendomaticFile.h"

namespace genie
{

Logger& BlendomaticFile::log = Logger::getLogger("genie.BlendomaticFile");

//------------------------------------------------------------------------------
BlendomaticFile::BlendomaticFile() : IFile()
{
}

//------------------------------------------------------------------------------
BlendomaticFile::~BlendomaticFile()
{
}

//------------------------------------------------------------------------------
void BlendomaticFile::serializeObject(void)
{
    serialize(modeCount_);
    std::cout << modeCount_ << " modes" << std::endl;
    serialize(tileCount_);
    std::cout << tileCount_ << " tiles" << std::endl;

    for (uint32_t i=0; i < modeCount_; i++) {
        BlendModePtr mode;
        if (isOperation(OP_WRITE)) {
            mode = modes_[i];
        } else {
            mode = std::make_unique<BlendMode>();
            modes_.push_back(mode);
        }

        std::cout << "start mode: " << tellg() << std::endl;

        serialize(mode->tileSize);
        std::cout << "tilesize: " << mode->tileSize << std::endl;
        if (mode->tileSize > 3000) {
            std::cout << "invalid tilesize" << std::endl;
            exit(0);
//            return;
        }

        serialize(mode->flags, tileCount_);

        std::cout << "start bitmasks: " << std::hex << tellg() << std::dec << std::endl;
        std::streampos posBefore = tellg();

        for (size_t i=0; i<mode->bitmasks.size(); i++){
            serialize(mode->bitmasks[i], ::ceil(mode->tileSize/8));
        }
        std::cout << "bitmask size: " << std::hex << (tellg() - posBefore) << std::dec << std::endl;

        std::cout << "start bytemasks: " << std::hex << tellg() << std::dec << std::endl;
        posBefore = tellg();

        uint32_t dummy;
        serialize(dummy);
        std::cout << "dummy " << dummy << std::endl;

        serialize(mode->bytemasks, tileCount_, mode->tileSize);


        std::cout << "end tiles: " << std::hex << tellg() << std::dec << " read size: " << std::hex << (tellg() - posBefore) << std::dec << std::endl;
    }
}

//------------------------------------------------------------------------------
void BlendomaticFile::unload(void)
{
    modes_.clear();
    modeCount_ = 0;
    tileCount_ = 0;
}

BlendModePtr BlendomaticFile::getBlendMode(uint32_t id)
{
    if (id < 0 || id > modes_.size()) {
        log.error("Invalid blendomatic id %d", id);
        return nullptr;
    }

    return modes_[id];
}


}
