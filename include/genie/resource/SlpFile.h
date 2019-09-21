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

#ifndef GENIE_SLPFILE_H
#define GENIE_SLPFILE_H

#include <istream>
#include <vector>
#include <sstream>
#include <map>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"
#include "PalFile.h"
#include "SlpFrame.h"

namespace genie {

//------------------------------------------------------------------------------
/// A slp file stores one or several images encoded using simple commands.
/// The image is stored as 8 bits per pixel, that means only the index of a
/// color in a palette is saved.
//
class SlpFile : public IFile
{

public:
    //----------------------------------------------------------------------------
    /// Constructor
    //
    SlpFile(const size_t size);

    //----------------------------------------------------------------------------
    /// Frees all content of a slp file.
    //
    void unload() override;

    //----------------------------------------------------------------------------
    /// Check whether the files content is loaded or not.
    //
    bool isLoaded() const;

    //----------------------------------------------------------------------------
    /// Return number of frames stored in the file. Available after load.
    ///
    /// @return number of frames
    //
    uint32_t getFrameCount();
    void setFrameCount(uint32_t);

    //----------------------------------------------------------------------------
    /// Returns the slp frame at given frame index.
    ///
    /// @param frame frame index
    /// @return SlpFrame
    //
    const SlpFramePtr &getFrame(uint32_t frame = 0);

    void setFrame(uint32_t, SlpFramePtr);

    /// For normal SLPs, e. g. '2.0N', for new SMP from Aoe2:DE 'SMP$'
    std::string version;

    /// AoE2:DE only, "256, 0x00000100 (same value for almost all units)" from heinezen
    uint32_t unknown1 = 256;

    /// AoE2:DE only, apparently they write number of frames twice
    uint32_t numFrames2 = 0;

    /// AoE2:DE only, "1, 0x0000001 (almost always 0x00000001)" from heinezen
    uint32_t unknown2 = 1;

    /// AoE2:DE only, Example value from doc from heinezen
    uint32_t maybeChecksom = 0x8554F6F3;

    /// AoE2:DE only, file size in bytes
    uint32_t size2 = 0;

    /// AoE2:DE only, 0x0B or 0x0C according to heinezen
    uint32_t maybeVersion = 0;

    std::string comment;

    const std::vector<uint8_t> &fileData() const { return m_graphicsFileData; }

    int frameCommandsOffset(const size_t frame, const int row);
    int frameHeight(const size_t frame);
    int frameWidth(const size_t frame);

private:
    friend class SlpTemplateFile;

    static Logger &log;

    size_t size_ = 0;
    bool loaded_ = false;

    uint32_t num_frames_ = 0;

    typedef std::vector<SlpFramePtr> FrameVector;
    FrameVector frames_;

    // Used to calculate offsets when saving the SLP.
    uint32_t slp_offset_;

    //----------------------------------------------------------------------------
    void serializeObject() override;

    //----------------------------------------------------------------------------
    /// Loads the file and its frames.
    //
    void loadFile();
    void saveFile();

    //----------------------------------------------------------------------------
    void serializeHeader();

    std::vector<uint8_t> m_graphicsFileData;
};

typedef std::shared_ptr<SlpFile> SlpFilePtr;
} // namespace genie

#endif // GENIE_SLPFILE_H
