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

#include <array>
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

// List of colors.
enum class Color
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

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

    // Sets whether this format should not be mixed in with parent formats.
    void setStandalone(bool isStandalone)
    { standalone = isStandalone; }
    // Checks whether this format should not be mixed in with parent formats.
    bool isStandalone() const
    { return standalone; }

    // Sets foreground color (negative value means "no color").
    void setForeground(int color)
    { fg = (color < 0 ? -1 : color); }
    // Sets named color as foreground.
    void setForeground(Color color);
    // Retrieves foreground color.
    int getForeground() const
    { return fg; }
    // Checks if foreground color is set.
    bool hasForeground() const
    { return fg >= 0; }

    // Sets background color (negative value means "no color").
    void setBackground(int color)
    { bg = (color < 0 ? -1 : color); }
    // Sets named color as background.
    void setBackground(Color color);
    // Retrieves background color.
    int getBackground() const
    { return bg; }
    // Checks if background color is set.
    bool hasBackground() const
    { return bg >= 0; }

    // Resolves ambiguity on invoking `operator()` with a wide literal.
    template <std::size_t N>
    ColorTree operator()(const wchar_t (&text)[N]);
    // Applies this formatting to specified piece of text.
    ColorTree operator()(std::wstring text) const;
    // Applies this formatting to a tree.
    ColorTree operator()(ColorTree &&tree) const;

private:
    int fg = -1;             // Foreground color or negative number.
    int bg = -1;             // Background color or negative number.
    bool bold = false;       // Whether text is bold.
    bool reversed = false;   // Whether text has reversed colors.
    bool underlined = false; // Whether text is underlined.
    bool standalone = false; // Whether this format should be mixed to parent.
};

// Mixes one format with another.
Format & operator+=(Format &lhs, const Format &rhs);

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
    // Constructs a leaf node with text specified as a literal.
    template <std::size_t N>
    ColorTree(const wchar_t (&text)[N]) : text(text, text + N - 1)
    { }
    // Constructs a leaf node with text specified as `wchar_t` array.
    ColorTree(const wchar_t text[]) : text(text)
    { }
    // Constructs a leaf node from text specified as `std::array<wchar_t, N>`.
    template <std::size_t N>
    ColorTree(const std::array<wchar_t, N> &text) : text(text.data())
    { }
    // Constructs a leaf node with specified text and format.
    ColorTree(std::wstring text, Format format);

public:
    // Builds an instance out of a string with escape codes.
    static ColorTree fromEscapeCodes(const std::wstring &line);

public:
    // Extends tree by appending another branch to it.
    void append(ColorTree &&branch);

    // Invokes visitor per leaf node of the tree.
    void visit(const visitorFunc &visitor) const;

    // Retrieves cumulative length of all pieces of the tree.
    int length() const;

private:
    Format format;                   // Format of the tree.
    std::wstring text;               // Text of a leaf.
    std::vector<ColorTree> branches; // Child trees.
};

// Builds a new tree out of two existing ones.
ColorTree operator+(ColorTree &&lhs, ColorTree &&rhs);
// Extends a tree by appending another branch to it.
ColorTree & operator+=(ColorTree &lhs, ColorTree &&rhs);

template <std::size_t N>
ColorTree
Format::operator()(const wchar_t (&text)[N])
{
    return (*this)(std::wstring(text, text + N - 1));
}

}

#endif // LIBCURSED__COLORTREE_HPP__
