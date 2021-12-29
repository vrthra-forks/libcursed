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

#include "ColorTree.hpp"

#include <sstream>
#include <stack>
#include <utility>

#include <curses.h>

using namespace cursed;

static int colorToInt(Color color);
static ColorTree fromEscapeCodes(const std::wstring &line, std::size_t offset);

// Manages combining of multiple formats.  The idea is that formats are added to
// the state when they become active and are removed after they become inactive.
class ColorTree::FormatState
{
public:
    FormatState() = default;

    FormatState(const FormatState &rhs) = delete;
    FormatState(FormatState &&rhs) = delete;
    FormatState & operator=(const FormatState &rhs) = delete;
    FormatState & operator=(FormatState &&rhs) = delete;

public:
    // Adds a format.
    FormatState & operator+=(const Format &format)
    {
        if (format.isStandalone()) {
            previous.push(current);
            current = {};
            return *this;
        }

        if (format.isBold()) {
            if (boldCounter++ == 0) {
                current.setBold(true);
            }
        }
        if (format.isReversed()) {
            current.setReversed(!current.isReversed());
        }
        if (format.isUnderlined()) {
            if (underlinedCounter++ == 0) {
                current.setUnderlined(true);
            }
        }
        if (format.hasForeground()) {
            fg.push(format.getForeground());
            current.setForeground(fg.top());
        }
        if (format.hasBackground()) {
            bg.push(format.getBackground());
            current.setBackground(bg.top());
        }
        return *this;
    }

    // Subtracts a format.
    FormatState & operator-=(const Format &format)
    {
        if (format.isStandalone()) {
            current = previous.top();
            previous.pop();
            return *this;
        }

        if (format.isBold()) {
            if (--boldCounter == 0) {
                current.setBold(false);
            }
        }
        if (format.isReversed()) {
            current.setReversed(!current.isReversed());
        }
        if (format.isUnderlined()) {
            if (--underlinedCounter == 0) {
                current.setUnderlined(false);
            }
        }
        if (format.hasForeground()) {
            fg.pop();
            current.setForeground(fg.empty() ? -1 : fg.top());
        }
        if (format.hasBackground()) {
            bg.pop();
            current.setBackground(bg.empty() ? -1 : bg.top());
        }
        return *this;
    }

    // Retrieves current format.
    const Format & getCurrent() const
    { return current; }

private:
    Format current;              // Current format.
    std::stack<Format> previous; // Previous formats.
    std::stack<int> fg;          // Stack of active foreground colors.
    std::stack<int> bg;          // Stack of active background colors.
    int boldCounter = 0;         // Count bold attribute was encountered.
    int underlinedCounter = 0;   // Count underline attribute was encountered.
};

void
Format::setForeground(Color color)
{
    fg = colorToInt(color);
}

void
Format::setBackground(Color color)
{
    bg = colorToInt(color);
}

// Maps member of the `Color` enumeration to curses color number.
static int
colorToInt(Color color)
{
    switch (color) {
        case Color::Black:   return COLOR_BLACK;
        case Color::Red:     return COLOR_RED;
        case Color::Green:   return COLOR_GREEN;
        case Color::Yellow:  return COLOR_YELLOW;
        case Color::Blue:    return COLOR_BLUE;
        case Color::Magenta: return COLOR_MAGENTA;
        case Color::Cyan:    return COLOR_CYAN;
        case Color::White:   return COLOR_WHITE;
    }
    return -1;
}

ColorTree
Format::operator()(std::wstring text) const
{
    return ColorTree(std::move(text), *this);
}

ColorTree
Format::operator()(ColorTree &&tree) const
{
    ColorTree newTree(*this);
    newTree.append(std::move(tree));
    return newTree;
}

Format &
cursed::operator+=(Format &lhs, const Format &rhs)
{
    if (rhs.isStandalone()) {
        lhs = {};
    }

    if (rhs.isBold()) {
        lhs.setBold(true);
    }
    if (rhs.isReversed()) {
        lhs.setReversed(!lhs.isReversed());
    }
    if (rhs.isUnderlined()) {
        lhs.setUnderlined(true);
    }
    if (rhs.hasForeground()) {
        lhs.setForeground(rhs.getForeground());
    }
    if (rhs.hasBackground()) {
        lhs.setBackground(rhs.getBackground());
    }
    return lhs;
}

ColorTree::ColorTree(Format format) : format(std::move(format))
{ }

ColorTree::ColorTree(std::wstring text) : text(std::move(text))
{ }

ColorTree::ColorTree(std::wstring text, Format format)
    : format(std::move(format)), text(std::move(text))
{ }

ColorTree
ColorTree::fromEscapeCodes(const std::wstring &line)
{
    return ::fromEscapeCodes(line, 0);
}

// Builds a ColorTree out of a substring of its argument starting with offset by
// parsing escape codes.
static ColorTree
fromEscapeCodes(const std::wstring &line, std::size_t offset)
{
    if (line.empty() || offset == line.length()) {
        return {};
    }

    auto start = line.find(L'\033', offset);
    if (start == std::wstring::npos || line[start + 1] != L'[') {
        return line.substr(offset);
    }

    auto end = line.find(L'm', start + 2);
    if (end == std::wstring::npos) {
        return line.substr(offset);
    }

    std::wistringstream iwss(line.substr(start + 2, end - start - 1));
    int n;
    wchar_t separator;
    cursed::Format fmt;
    while (iwss >> n >> separator) {
        if (n == 0) {
            fmt.setStandalone(true);
        } else if (n == 1) {
            fmt.setBold(true);
        } else if (n == 4 || n == 24) {
            fmt.setUnderlined(n == 4);
        } else if (n == 7 || n == 27) {
            fmt.setReversed(n == 7);
        } else if (n == 22) {
            fmt.setBold(false);
            fmt.setUnderlined(false);
            fmt.setReversed(false);
        } else if (n >= 30 && n <= 37) {
            fmt.setForeground(n - 30);
        } else if (n >= 40 && n <= 47) {
            fmt.setBackground(n - 40);
        } else if (n == 39) {
            fmt.setForeground(-1);
        } else if (n == 49) {
            fmt.setBackground(-1);
        } else if (n == 38) {
            int nn;
            wchar_t ss;
            if (iwss >> nn >> ss >> n >> separator) {
                if (nn == 5) {
                    fmt.setForeground(n);
                }
            }
        } else if (n == 48) {
            int nn;
            wchar_t ss;
            if (iwss >> nn >> ss >> n >> separator) {
                if (nn == 5) {
                    fmt.setBackground(n);
                }
            }
        }

        if (separator == L'm') {
            return line.substr(offset, start - offset)
                 + fmt(fromEscapeCodes(line, end + 1));
        }
    }

    return line.substr(offset);
}

void
ColorTree::append(ColorTree &&branch)
{
    if (!text.empty()) {
        // A child is being added to a leaf, turn contents into a child first.
        branches.emplace_back(std::move(text), format);
        text = std::wstring();
        format = Format();
    }
    branches.emplace_back(std::move(branch));
}

void
ColorTree::visit(const visitorFunc &visitor) const
{
    std::stack<const ColorTree *> trees;
    trees.push(this);

    std::stack<const Format *> formats;
    Format fmt;
    formats.push(&fmt);

    FormatState formatState;
    while (!trees.empty()) {
        const ColorTree &tree = *trees.top();
        trees.pop();

        formatState -= *formats.top();
        formats.top() = &tree.format;
        formatState += tree.format;

        if (tree.branches.empty()) {
            visitor(tree.text, formatState.getCurrent());
        } else {
            for (auto it = tree.branches.crbegin();
                 it != tree.branches.crend();
                 ++it) {
                trees.push(&*it);
                formats.emplace(&fmt);
            }
        }
    }
}

int
ColorTree::length() const
{
    std::stack<const ColorTree *> trees;
    trees.push(this);

    int len = 0;
    while (!trees.empty()) {
        const ColorTree &tree = *trees.top();
        trees.pop();

        if (tree.branches.empty()) {
            len += tree.text.length();
        } else {
            for (auto it = tree.branches.crbegin();
                 it != tree.branches.crend();
                 ++it) {
                trees.push(&*it);
            }
        }
    }

    return len;
}

ColorTree
cursed::operator+(ColorTree &&lhs, ColorTree &&rhs)
{
    ColorTree branch;
    branch.append(std::move(lhs));
    branch.append(std::move(rhs));
    return branch;
}

ColorTree &
cursed::operator+=(ColorTree &lhs, ColorTree &&rhs)
{
    lhs.append(std::move(rhs));
    return lhs;
}
