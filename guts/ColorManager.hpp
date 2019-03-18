// libcursed -- C++ classes for dealing with curses
// Copyright (C) 2019 xaizek <xaizek@posteo.net>
//
// This file is part of libcursed.
//
// libcursed is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libcursed is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libcursed.  If not, see <https://www.gnu.org/licenses/>.

#ifndef LIBCURSED__GUTS__COLORMANAGER_HPP__
#define LIBCURSED__GUTS__COLORMANAGER_HPP__

namespace cursed { namespace guts {

// Manages allocation of color pairs.
class ColorManager
{
private:
    // Initializes the manager.
    ColorManager();

    ColorManager(const ColorManager &rhs) = delete;
    ColorManager(ColorManager &&rhs) = delete;
    ColorManager & operator=(const ColorManager &rhs) = delete;
    ColorManager & operator=(ColorManager &&rhs) = delete;

public:
    // Retrieves instance of this class.
    static ColorManager & get();

public:
    // Resets all color pairs that are available for dynamic allocation.
    void reset();
    // Retrieves or allocates color pair number for specified foreground and
    // background colors.  Returns the number.  On failure fall back to color
    // pair 0.
    int makePair(int fg, int bg);

private:
    // Tries to find a pair with specified colors among already allocated pairs.
    // Returns -1 when search fails.
    int findPair(int fg, int bg);
    // Allocates new color pair.  Returns new pair index, or -1 on failure.
    int allocatePair(int fg, int bg);

private:
    int usedPairsCount; // How many pairs are currently in use.
};

} }

#endif // LIBCURSED__GUTS__COLORMANAGER_HPP__
