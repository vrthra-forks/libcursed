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

#include "Input.hpp"

#include <curses.h>

using namespace cursed;

InputElement::InputElement(int status, std::wint_t code)
    : status(status), code(code)
{ }

bool
InputElement::isEndOfInput() const
{
    return (status == ERR);
}

bool
InputElement::isTerminalResize() const
{
    return (status == KEY_CODE_YES && code == KEY_RESIZE);
}

InputElement::operator wchar_t() const
{
    return (status == OK ? code : L'\0');
}

bool
InputElement::isFunctionalKey() const
{
    return (status == KEY_CODE_YES);
}

bool
InputElement::operator==(wchar_t wch) const
{
    return (status == OK && static_cast<wchar_t>(*this) == wch);
}

InputElement
Input::read()
{
    std::wint_t wch;
    int status = get_wch(&wch);

    InputElement ie(status, wch);

    if (ie.isTerminalResize()) {
        // Prompt curses to resize `stdscr`.
        refresh();
    }

    return ie;
}
