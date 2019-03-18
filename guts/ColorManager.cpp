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

#include "ColorManager.hpp"

#include <curses.h>

#include <algorithm>

using namespace cursed::guts;

// Number of pairs that are reserved.
constexpr int PreallocatedPairsCount = 1;

static int colorPairMatches(int pair, int fg, int bg);

ColorManager::ColorManager()
{
    reset();
}

ColorManager &
ColorManager::get()
{
    static ColorManager instance;
    return instance;
}

void
ColorManager::reset()
{
    usedPairsCount = PreallocatedPairsCount;
}

int
ColorManager::makePair(int fg, int bg)
{
    fg = std::max(fg, -1);
    bg = std::max(bg, -1);

    if (fg == -1 && bg == -1) {
        return 0;
    }

    int p = findPair(fg, bg);
    if (p != -1) {
        return p;
    }

    p = allocatePair(fg, bg);
    return (p == -1 ? 0 : p);
}

int
ColorManager::findPair(int fg, int bg)
{
    // TODO: consider using a map instead of linear search.
    for (int i = PreallocatedPairsCount; i < usedPairsCount; ++i) {
        if (colorPairMatches(i, fg, bg)) {
            return i;
        }
    }

    return -1;
}

// Checks whether color pair has specified foreground (fg) and background (bg)
// colors.
static int
colorPairMatches(int pair, int fg, int bg)
{
    short pairFg, pairBg;
    pair_content(pair, &pairFg, &pairBg);
    return (pairFg == fg && pairBg == bg);
}

int
ColorManager::allocatePair(int fg, int bg)
{
    if (usedPairsCount == COLOR_PAIRS) {
        return -1;
    }

    if (init_pair(usedPairsCount, fg, bg) != 0) {
        return -1;
    }

    return usedPairsCount++;
}
