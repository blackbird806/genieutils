/*
    Reading blkedge.dat and tileedge.dat
    Copyright (C) 2019 Martin Sandsmark

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

#pragma once

#include <istream>
#include <vector>
#include <cassert>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"
#include "Slope.h"
#include "TileSpan.h"

namespace genie {

struct VisibilityMask {
    enum Visibility {
        None = 0,
        East = 1,
        NorthEast = 2,
        North = 4,
        NorthWest = 8,
        West = 16,
        SouthWest = 32,
        South = 64,
        SouthEast = 128,
    };

    static const VisibilityMask null;
    std::vector<TileSpan> lines;
};

template<int EdgeCount>
class EdgeFile : public IFile
{
public:
    const VisibilityMask &visibilityMask(const Slope slope, const int edges) const
    {
        if (edges >= EdgeCount) {
            log.error("Invalid edge %", edges);
            return VisibilityMask::null;
        }

        if (slope >= SlopeCount) {
            log.error("Invalid slope %", slope);
            return VisibilityMask::null;
        }

        return edgeSlopes[slope].tileMasks[edges];
    }

private:
    struct EdgeSlope {
        VisibilityMask tileMasks[EdgeCount];
    };

    VisibilityMask readMask()
    {
        VisibilityMask mask;

        while (!getIStream()->eof()) {
            TileSpan span;
            serialize(span);

            if (span.xStart == -1 && span.xEnd == -1) {
                break;
            }

            mask.lines.push_back(std::move(span));

            // just in case we read random gurba (good norwegian expression)
            if (mask.lines.size() > 200) {
                log.error("invalid size %", mask.lines.size());
                return {};
            }
        }

        return mask;
    }

    void serializeObject() override
    {
        const std::istream::pos_type start = tellg();

        serialize(slopeOffsets, SlopeCount);

        for (int slope = 0; slope < SlopeCount; slope++) {
            getIStream()->seekg(start + std::istream::pos_type(slopeOffsets[slope]));

            std::vector<int> maskOffsets;
            serialize(maskOffsets, EdgeCount);

            for (int edge = 0; edge < EdgeCount; edge++) {
                getIStream()->seekg(start + std::istream::pos_type(maskOffsets[edge]));
                edgeSlopes[slope].tileMasks[edge] = readMask();
            }
        }
    }
    std::vector<int> slopeOffsets;
    EdgeSlope edgeSlopes[SlopeCount];
    static Logger &log;
};

typedef EdgeFile<94> TileEdgeFile;
typedef EdgeFile<47> BlkEdgeFile;


} //namespace genie
