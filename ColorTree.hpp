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

#ifndef LIBCURSED__COLORTREE_HPP__
#define LIBCURSED__COLORTREE_HPP__

#include <functional>
#include <string>
#include <vector>

// Usage example:
//
//     cursed::Format fmt;
//     fmt.setBold(true);
//
//     cursed::ColorTree tree = fmt(L"first") + L"middle" + fmt(L"last");
//     tree += L"trailing";

namespace cursed {

class ColorTree;

// Describes formatting details for a piece of text.
class Format
{
public:
    // Sets whether text piece is bold.
    void setBold(bool isBold)
    { bold = isBold; }
    // Checks whether text piece is bold.
    bool isBold() const
    { return bold; }

    // Sets whether text piece has reversed colors.
    void setReversed(bool isReversed)
    { reversed = isReversed; }
    // Checks whether text piece has reversed colors.
    bool isReversed() const
    { return reversed; }

    // Sets whether text piece is underlined.
    void setUnderlined(bool isUnderlined)
    { underlined = isUnderlined; }
    // Checks whether text piece is underlined.
    bool isUnderlined() const
    { return underlined; }

    // Applies this formatting to specified piece of text.
    ColorTree operator()(std::wstring text) const;

private:
    bool bold = false;       // Whether text is bold.
    bool reversed = false;   // Whether text has reversed colors.
    bool underlined = false; // Whether text is underlined.
};

// Describes hierarchically colourable piece of text.
class ColorTree
{
    class FormatState;

    // Type of callback function invoked for leaf nodes on visit.
    using visitorFunc = std::function<void(const std::wstring &text,
                                           const Format &format)>;

public:
    // Constructs an empty tree.
    ColorTree() = default;
    // Constructs a tree with specified format at its root.
    explicit ColorTree(Format format);
    // Constructs a leaf node with specified text and empty format.
    ColorTree(std::wstring text);
    // Constructs a leaf node with specified text specified as a literal.
    template <std::size_t N>
    ColorTree(const wchar_t (&text)[N]) : text(text, text + N)
    { }
    // Constructs a leaf node with specified text and format.
    ColorTree(std::wstring text, Format format);

public:
    // Extends tree by appending another branch to it.
    void append(ColorTree &&branch);

    // Invokes visitor per leaf node of the tree.
    void visit(const visitorFunc &visitor) const;

private:
    Format format;                   // Format of the tree.
    std::wstring text;               // Text of a leaf.
    std::vector<ColorTree> branches; // Child trees.
};

// Builds a new tree out of two existing ones.
ColorTree operator+(ColorTree &&lhs, ColorTree &&rhs);
// Extends a tree by appending another branch to it.
ColorTree & operator+=(ColorTree &lhs, ColorTree &&rhs);

}

#endif // LIBCURSED__COLORTREE_HPP__
