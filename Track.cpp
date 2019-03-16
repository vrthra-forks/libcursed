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

#include "Track.hpp"

#include <algorithm>
#include <vector>

using namespace cursed;

void
Track::addItem(Widget *w)
{
    widgets.push_back(w);
}

void
Track::place(Pos newPos, Size newSize)
{
    // Narrow range of values to avoid integer overflows.
    const int Range = 10000;

    int nFlexible = 0;
    int booked = 0;
    std::vector<int> needed;

    std::vector<Size> sizes;
    sizes.reserve(widgets.size());

    // Place items with fixed size.
    for (Widget *w : widgets) {
        int neededHeight = std::max(std::min(w->desiredHeight(), Range),
                                    -Range);
        needed.push_back(neededHeight);

        if (neededHeight < 0) {
            ++nFlexible;
            booked += -neededHeight;
            sizes.emplace_back();
        } else {
            sizes.emplace_back(neededHeight, newSize.cols);
            newSize.lines -= neededHeight;
        }
    }

    // Place flexible items giving them share of extra space.
    int extraFraction = (nFlexible != 0) ? (newSize.lines - booked)/nFlexible
                                         : 0;
    for (unsigned int i = 0U; i < needed.size(); ++i) {
        if (needed[i] < 0) {
            --nFlexible;
            int height = (nFlexible == 0) ? newSize.lines
                                          : -needed[i] + extraFraction;
            sizes[i] = Size(height, newSize.cols);
            newSize.lines -= height;
        }
    }

    // Apply placement results.
    for (unsigned int i = 0U; i < sizes.size(); ++i) {
        widgets[i]->place(newPos, sizes[i]);
        newPos.y += sizes[i].lines;
    }
}

void
Track::draw()
{
    for (Widget *w : widgets) {
        w->draw();
    }
}

int
Track::desiredHeight()
{
    int minHeight = 0;
    int exactHeight = 0;
    for (Widget *w : widgets) {
        int neededHeight = w->desiredHeight();
        if (neededHeight < 0) {
            minHeight += -neededHeight;
        } else {
            exactHeight += neededHeight;
        }
    }

    if (minHeight == 0) {
        // There are not flexible items, so return the exact part.
        return exactHeight;
    }
    // Add exact part to minimal sizes of flexible items to get a lower bound
    // for size.
    return minHeight - exactHeight;
}
