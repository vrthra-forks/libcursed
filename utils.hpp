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

#ifndef LIBCURSED__UTILS_HPP__
#define LIBCURSED__UTILS_HPP__

#include <string>

namespace cursed {

// Converts narrow string into wide one.  Returns new string or an empty one on
// error.
std::wstring toWide(const std::string &s);

// Converts wide string into narrow one.  Returns new string or an empty one on
// error.
std::string toNarrow(const std::wstring &ws);

}

#endif // LIBCURSED__UTILS_HPP__
