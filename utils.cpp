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

#include "utils.hpp"

#include <cstdlib>

#include <string>

std::wstring
cursed::toWide(const std::string &s)
{
    const std::size_t len = std::mbstowcs(nullptr, s.c_str(), 0);
    if (len == static_cast<std::size_t>(-1)) {
        return std::wstring();
    }

    std::wstring result(len + 1U, L'\0');
    (void)std::mbstowcs(&result[0], s.c_str(), len + 1U);
    result.pop_back();
    return result;
}

std::string
cursed::toNarrow(const std::wstring &ws)
{
    const std::size_t len = std::wcstombs(nullptr, ws.c_str(), 0);
    if (len == static_cast<std::size_t>(-1)) {
        return std::string();
    }

    std::string result(len + 1U, L'\0');
    (void)std::wcstombs(&result[0], ws.c_str(), len + 1U);
    result.pop_back();
    return result;
}
