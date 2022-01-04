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

#ifndef LIBCURSED__SCREEN_HPP__
#define LIBCURSED__SCREEN_HPP__

#include <vector>

namespace cursed {

namespace guts {
    class Widget;
    class WindowWidget;
}

// Provides information about the whole screen.
class Screen
{
public:
    // Sets/updates top widget and stretches it over the whole screen.
    void replaceTopWidget(guts::Widget *w);

    // Pushes widget as the top one.
    void pushMainWidget(guts::Widget *w);
    // Pops the top widget.
    void popMainWidget();

    // Handles screen resizing.
    void resize();

    // Draws main widget and makes screen updates visible on physical screen.
    void draw();

    // Makes cursor visible.  Optionally sets widget that owns the cursor.
    // Returns `true` if cursor was hidden before the call.
    bool showCursor(guts::WindowWidget *w = nullptr);
    // Makes cursor invisible.  Returns `true` if cursor was visible before the
    // call.
    bool hideCursor();

    // Hides current application's version of the screen.
    void stopTUI();

private:
    // Roots of all currently displayed layers of widgets.
    std::vector<guts::Widget *> mainWidgets;
    // Widget that has the cursor.
    guts::WindowWidget *cursorWidget = nullptr;
};

}

#endif // LIBCURSED__SCREEN_HPP__
