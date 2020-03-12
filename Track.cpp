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
using namespace cursed::guts;

static std::vector<int> doLayout(const std::vector<int> &lengths, int max);
static int calculateDesired(const std::vector<int> &lengths);

// Boundary to narrow range of values to avoid integer overflows.
constexpr int Range = 10000;

Track::Track(Orientation orientation) : orientation(orientation)
{ }

void
Track::addItem(Widget *w)
{
    widgets.push_back(w);
}

void
Track::place(Pos newPos, Size newSize)
{
    if (orientation == Orientation::Vertical) {
        placeVertically(newPos, newSize);
    } else {
        placeHorizontally(newPos, newSize);
    }
}

void
Track::placeVertically(Pos newPos, Size newSize)
{
    std::vector<int> lengths;
    lengths.reserve(widgets.size());
    for (Widget *w : widgets) {
        lengths.push_back(w->desiredHeight());
    }

    lengths = doLayout(lengths, newSize.lines);

    // Apply placement results.
    for (unsigned int i = 0U; i < lengths.size(); ++i) {
        widgets[i]->place(newPos, Size(lengths[i], newSize.cols));
        newPos.y += lengths[i];
    }
}

void
Track::placeHorizontally(Pos newPos, Size newSize)
{
    std::vector<int> lengths;
    lengths.reserve(widgets.size());
    for (Widget *w : widgets) {
        lengths.push_back(w->desiredWidth());
    }

    lengths = doLayout(lengths, newSize.cols);

    // Apply placement results.
    for (unsigned int i = 0U; i < lengths.size(); ++i) {
        widgets[i]->place(newPos, Size(newSize.lines, lengths[i]));
        newPos.x += lengths[i];
    }
}

// Performs distribution of `max` units among widgets that want specified
// lengths.
static std::vector<int>
doLayout(const std::vector<int> &lengths, int max)
{
    int nFlexible = 0;
    int booked = 0;

    std::vector<int> layout;
    int lengthLeft = max;

    auto getBooked = [](int len) { return (len <= -Range ? 1 : -len); };

    for (int length : lengths) {
        length = std::max(std::min(length, Range), -Range);

        if (length < 0) {
            ++nFlexible;
            booked += getBooked(length);
            layout.emplace_back();
        } else {
            layout.emplace_back(length);
            lengthLeft = std::max(0, lengthLeft - length);
        }
    }

    if (lengthLeft < booked) {
        int have = lengthLeft;
        int left = nFlexible;
        for (unsigned int i = 0U; i < lengths.size(); ++i) {
            if (lengths[i] < 0) {
                int length = getBooked(lengths[i]);

                --left;
                layout[i] = (left == 0)
                          ? have
                          : lengthLeft*(float(length)/booked);
                have -= layout[i];
            }
        }
        return layout;
    }

    // Place flexible items giving them share of extra space.
    int extraFraction = (nFlexible != 0)
                      ? (lengthLeft - booked)/nFlexible
                      : 0;
    for (unsigned int i = 0U; i < lengths.size(); ++i) {
        if (lengths[i] < 0) {
            --nFlexible;
            layout[i] = (nFlexible == 0)
                      ? lengthLeft
                      : std::min(lengthLeft, -lengths[i] + extraFraction);
            lengthLeft -= layout[i];
        }
    }

    return layout;
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
    std::vector<int> lengths;
    lengths.reserve(widgets.size());
    for (Widget *w : widgets) {
        lengths.push_back(w->desiredHeight());
    }
    return calculateDesired(lengths);
}

int
Track::desiredWidth()
{
    std::vector<int> lengths;
    lengths.reserve(widgets.size());
    for (Widget *w : widgets) {
        lengths.push_back(w->desiredWidth());
    }
    return calculateDesired(lengths);
}

// Calculates desired length of the track based on desired lengths of widgets.
static int
calculateDesired(const std::vector<int> &lengths)
{
    int minLength = 0;
    int sign = 1;
    for (int length : lengths) {
        length = std::max(std::min(length, Range), -Range);
        if (length != -Range) {
            minLength = std::max(minLength, std::abs(length));
            if (length < 0) {
                sign = -1;
            }
        }
    }
    return sign*minLength;
}
