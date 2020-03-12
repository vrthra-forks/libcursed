// libcursed -- C++ classes for dealing with curses
// Copyright (C) 2016 xaizek <xaizek@posteo.net>
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

#ifndef LIBCURSED__TABLE_HPP__
#define LIBCURSED__TABLE_HPP__

#include <vector>

#include "guts/WindowWidget.hpp"
#include "ColorTree.hpp"
#include "ListLike.hpp"

namespace cursed {

// Types of alignment of columns.
enum class Align
{
    Left, // By left border.
    Right // By right border.
};

// Information about a column.
struct TableHeader
{
    TableHeader(ColorTree label, Align alignment)
        : label(std::move(label)), alignment(alignment)
    { }

    ColorTree label; // Title of the column.
    Align alignment; // Alignment of both label and contents.
};

// Multicolumn list.
class Table : public guts::WindowWidget, public ListLike
{
    class Column;

public:
    // Initializes an empty table.
    Table();
    // Emit destructing code in corresponding source file.
    ~Table();

public:
    // Adds a column.  Throws std::runtime_error if list of items isn't empty.
    void addColumn(TableHeader heading);
    // Adds a row.  Throws std::invalid_argument if item length doesn't match
    // columns.
    void append(const std::vector<ColorTree> &item);

private:
    // Populates columns with items' data.
    void fillColumns();
    // Ensures that columns fit into required width limit.  Returns `true` on
    // successful shrinking.
    bool adjustColumnsWidths();
    // Print table heading.
    void printTableHeader();
    // Prints table lines.
    void printTableRows();
    // Pads string to align it according to column parameters.
    ColorTree alignCell(ColorTree s, const Column &col) const;

    // Updates state of this widget to be published on the screen.
    virtual void draw() override;

    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;
    // Retrieves horizontal size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredWidth() override;

    // Notifies widget of new position and size.
    virtual void placed(guts::Pos newPos, guts::Size newSize) override;

    // Retrieves number of elements in the list.
    virtual int getSize() const override;

    // Retrieves viewport height.
    virtual int getHeight() const override;

private:
    // Maximum allowed table width.
    unsigned int maxWidth;
    // Maximum allowed table height.
    int height;
    // List of columns of the table.
    std::vector<Column> cols;
    // List of items to display.
    std::vector<std::vector<ColorTree>> items;
    // Visual style of the current item.
    Format currentHi;
};

}

#endif // LIBCURSED__TABLE_HPP__
