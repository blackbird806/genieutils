/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml

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

#ifndef GENIE_UIFILE_H
#define GENIE_UIFILE_H

#include <string>
#include <vector>
#include <stdint.h>
#include <memory>

#include "genie/file/IFile.h"
#include "genie/resource/Color.h"

namespace genie {

class Logger;
class Color;

//------------------------------------------------------------------------------
/// Class for parsing aoe color palettes.
//
class UIFile : public IFile
{

public:
    struct Background
    {
        std::string filename;
        std::string alternateFilename; //always none?
        int32_t fileId;
        int32_t alternateFileId; // always -1
    };

    struct FileReference
    {
        std::string filename;
        int32_t id;
    };

    /// width < 800
    Background backgroundSmall;
    /// width < 1024
    Background backgroundMedium;
    /// width >= 1024
    Background backgroundLarge;
    FileReference paletteFile;
    FileReference cursorFile;
    FileReference buttonFile;
    FileReference popupDialogFile;

    uint32_t shadePercent;
    uint32_t backgroundPosition;

    // This color is used for masking out the background (i. e. making it transparent)
    // Think greenscreen, only usually pink
    uint32_t backgroundMaskColor;

    Color bevelColor1;
    Color bevelColor2;

    Color textColor1;
    Color textColor2;

    Color focusColor1;
    Color focusColor2;

    Color stateColor1;
    Color stateColor2;

private:
    bool readBackground(std::istream *istr, Background *background, const std::string &expectedName);
    bool readFileReference(std::istream *istr, FileReference *fileReference, const std::string &expectedName);
    bool readValue(std::istream *istr, uint32_t *val, const std::string &expectedName, const std::string &type = std::string());
    bool readColor(std::istream *istr, Color *color, const std::string &expectedName);

    static Logger &log;

    std::string windowType;

    void serializeObject() override;
};

typedef std::shared_ptr<UIFile> UIFilePtr;
}

#endif // GENIE_UIFILE_H
