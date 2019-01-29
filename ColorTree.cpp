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

#include <stack>
#include <utility>

using namespace cursed;

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
    Format current;            // Current format.
    std::stack<int> fg;        // Stack of active foreground colors.
    std::stack<int> bg;        // Stack of active background colors.
    int boldCounter = 0;       // Count bold attribute was encountered.
    int underlinedCounter = 0; // Count underline attribute was encountered.
};

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

void
ColorTree::append(ColorTree &&branch)
{
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
