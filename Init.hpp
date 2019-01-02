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

#ifndef LIBCURSED__INIT_HPP__
#define LIBCURSED__INIT_HPP__

namespace cursed {

// Objects of this class are responsible for initialization and deinitialization
// of the library.  A single instance should be created and exist as long as
// other parts of the library are used.
class Init
{
public:
    // Initializes curses, which includes clearing the screen.
    explicit Init();
    // Deinitializes curses.
    ~Init();

    Init(const Init &rhs) = delete;
    Init(Init &&rhs) = delete;
    Init & operator=(const Init &rhs) = delete;
    Init & operator=(Init &&rhs) = delete;
};

}

#endif // LIBCURSED__INIT_HPP__
