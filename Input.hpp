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

#ifndef LIBCURSED__INPUT_HPP__
#define LIBCURSED__INPUT_HPP__

#include <cwctype>

#include "guts/Window.hpp"

namespace cursed {

// Keypad keycodes support flag.
enum class Keypad {
    Enabled, // Functional keys are returned as special key codes.
    Disabled // Functional keys are returned as escape-sequences.
};

// Describes single input event.
class InputElement
{
    friend class Input;

private:
    // Remembers its arguments.
    InputElement(int status, std::wint_t code);

public:
    // Checks if input has ended.
    bool isEndOfInput() const;
    // Checks if input is a terminal resize notification.
    bool isTerminalResize() const;
    // Checks if input is a functional key.
    bool isFunctionalKey() const;

    // Checks for end of input.
    explicit operator bool() const { return !isEndOfInput(); }
    // Converts input to a character if possible, otherwise to `L'\0'`.
    operator wchar_t() const;
    // Compares, it differs from conversion operator in always returning `false`
    // for non-character input element.
    bool operator==(wchar_t wch) const;

private:
    int status;       // Characterization of the key code.
    std::wint_t code; // Key code.
};

// Class responsible for handling input.
class Input
{
public:
    // Initializes state configuring whether functional keys are treated
    // specially or returned as escape-sequences.
    explicit Input(Keypad keypad);

    Input(const Input &rhs) = delete;
    Input(Input &&rhs) = delete;
    Input & operator=(const Input &rhs) = delete;
    Input & operator=(Input &&rhs) = delete;

public:
    // Retrieves single input event.
    InputElement read();
    // Peeks on a next input event without consuming it.  Might throw
    // `std::runtime_error` if ungetting input fails.
    InputElement peek();

private:
    guts::Window readWin; // Window for reading input.
    guts::Window peekWin; // Window for peeking (needs zero timeout).
};

}

#endif // LIBCURSED__INPUT_HPP__
