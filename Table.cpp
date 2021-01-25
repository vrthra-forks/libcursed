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

#include "Table.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace cursed;
using namespace cursed::guts;

static unsigned int measureWidth(const ColorTree &s);
static unsigned int measurePrefixLength(const ColorTree &s, int prefixWidth);

// Helper class that represents single column of a table.
class Table::Column
{
public:
    // Constructs an empty column.
    Column(int idx, ColorTree heading, bool alignLeft)
        : idx(idx), alignLeft(alignLeft), heading(std::move(heading)),
          fullWidth(this->heading.length()), width(fullWidth)
    { }

public:
    // Retrieves index of the column.
    int getIdx() const
    {
        return idx;
    }

    // Checks whether this column should be aligned to the left.
    bool leftAlign() const
    {
        return alignLeft;
    }

    // Retrieves heading of the column.
    const ColorTree getHeading() const
    {
        return truncate(heading);
    }

    // Adds a value to the column.
    void append(ColorTree val)
    {
        fullWidth = std::max(fullWidth, measureWidth(val));
        width = fullWidth;

        values.emplace_back(std::move(val));
    }

    // Retrieves widths of the column.
    unsigned int getWidth() const
    {
        return width;
    }

    // Retrieves widths of the column.
    void resetWidth()
    {
        width = fullWidth;
    }

    // Reduces width of the column by `by` positions.
    void reduceWidthBy(unsigned int by)
    {
        width -= std::min(width, by);
    }

    // Retrieves printable value of the column by index.  The value can be
    // truncated to fit limited width, which is indicated by trailing ellipsis.
    ColorTree operator[](unsigned int i) const
    {
        return truncate(values[i]);
    }

private:
    // Truncates a string with ellipsis to fit into column width.
    ColorTree truncate(const ColorTree &s) const
    {
        if (measureWidth(s) <= width) {
            return s;
        }
        if (width <= 3U) {
            return std::wstring(L"...").substr(0U, width);
        }

        ColorTree truncated;
        unsigned int left = measurePrefixLength(s, width - 3U);
        s.visit([&](const std::wstring &text, const Format &format) {
            if (left >= text.length()) {
                truncated += format(text);
                left -= text.length();
            } else if (left > 0) {
                truncated += format(text.substr(0, left));
                left = 0;
            }
        });
        truncated += L"...";
        return truncated;
    }

private:
    //! Index of the column.
    const int idx;
    //! Whether this column should be aligned to the left.
    bool alignLeft;
    //! Title of the column for printing.
    const ColorTree heading;
    //! Width of the column that wasn't reduced.
    unsigned int fullWidth;
    //! Width of the column.
    unsigned int width;
    //! Contents of the column.
    std::vector<ColorTree> values;
};

static const std::wstring gap = L"  ";

Table::Table() : maxWidth(0), height(0)
{
    currentHi.setReversed(true);
    currentHi.setForeground(Color::Yellow);
}

Table::~Table() = default;

void
Table::addColumn(TableHeader heading)
{
    if (!items.empty()) {
        throw std::invalid_argument("Can't change columns for non-empty "
                                    "table.");
    }

    bool alignLeft = heading.alignment == Align::Left;
    cols.emplace_back(cols.size(), std::move(heading.label), alignLeft);
}

void
Table::append(const std::vector<ColorTree> &item)
{
    if (item.size() != cols.size()) {
        throw std::invalid_argument("Invalid item added to the table.");
    }
    items.emplace_back(item);
}

void
Table::removeAll()
{
    items.clear();
}

void
Table::fillColumns()
{
    for (const std::vector<ColorTree> &item : items) {
        for (Column &col : cols) {
            col.append(item[col.getIdx()]);
        }
    }
}

bool
Table::adjustColumnsWidths()
{
    // The code below assumes that there is at least one column.
    if (cols.empty()) {
        return false;
    }

    // Calculate real width of the table.
    unsigned int realWidth = 0U;
    for (Column &col : cols) {
        col.resetWidth();
        realWidth += col.getWidth();
    }
    realWidth += gap.length()*(cols.size() - 1U);

    // Make ordering of columns that goes from widest to narrowest.
    std::vector<std::reference_wrapper<Column>> sorted {
        cols.begin(), cols.end()
    };
    std::sort(sorted.begin(), sorted.end(),
              [](const Column &a, const Column &b) {
                  return a.getWidth() >= b.getWidth();
              });

    // Repeatedly reduce columns until we reach target width.
    // At each iteration: reduce width of (at most all, but not necessarily) the
    // widest columns by making them at most as wide as the narrower columns
    // that directly follow them.
    while (realWidth > maxWidth) {
        unsigned int toReduce = realWidth - maxWidth;

        // Make list of the widest columns as well as figure out by which amount
        // we can adjust the width (difference between column widths).
        std::vector<std::reference_wrapper<Column>> widest;
        unsigned int maxAdj = static_cast<Column&>(sorted.front()).getWidth();
        for (Column &col : sorted) {
            const unsigned int w = col.getWidth();
            if (w != maxAdj) {
                maxAdj -= w;
                break;
            }
            widest.push_back(col);
        }

        // Reversed order of visiting to ensure that ordering invariant is
        // intact: last visited element can be reduced by smaller amount, which
        // will leave it the biggest.  Actually it doesn't matter because we
        // reach target width at the same time, still it might matter later.
        for (auto it = widest.rbegin(), end = widest.rend(); it != end; ++it ) {
            const unsigned int by = std::min(maxAdj, toReduce);
            it->get().reduceWidthBy(by);
            toReduce -= by;
        }

        // We could exhaust possibilities to reduce column width and all that's
        // left is padding between columns.
        if (maxAdj == 0) {
            break;
        }

        // Update current width of the table.
        realWidth = maxWidth + toReduce;
    }

    return realWidth <= maxWidth;
}

void
Table::printTableHeader()
{
    for (Column &col : cols) {
        win.print(alignCell(col.getHeading(), col));

        if (&col != &cols.back()) {
            win.print(gap);
        }
    }
}

void
Table::printTableRows()
{
    int top = getTop();
    int nItems = items.size();
    int pos = getPos();
    for (int i = top; i < top + height - 1; ++i) {
        if (i == nItems) {
            break;
        }

        Format hi;
        if (i == pos) {
            hi += currentHi;
        }

        wmove(win, i - top + 1, 0);

        for (Column &col : cols) {
            win.print(hi(alignCell(col[i], col)));

            if (&col != &cols.back()) {
                win.print(hi(gap));
            }
        }
    }
}

ColorTree
Table::alignCell(ColorTree s, const Column &col) const
{
    unsigned int lineWidth = measureWidth(s);
    if (lineWidth >= col.getWidth()) {
        return s;
    }

    const unsigned int padWidth = col.getWidth() - lineWidth;
    if (col.leftAlign()) {
        s += std::wstring(padWidth, L' ');
    } else {
        s = std::wstring(padWidth, L' ') + std::move(s);
    }
    return s;
}

void
Table::draw()
{
    fillColumns();

    if (!adjustColumnsWidths()) {
        // Available width is not enough to display table.
        return;
    }

    win.erase();
    wmove(win, 0, 0);

    printTableHeader();
    printTableRows();

    wnoutrefresh(win);
}

int
Table::desiredHeight()
{
    return -1;
}

int
Table::desiredWidth()
{
    return -1;
}

void
Table::placed(Pos newPos, Size newSize)
{
    WindowWidget::placed(newPos, newSize);
    maxWidth = newSize.cols;
    height = newSize.lines;
}

int
Table::getSize() const
{
    return items.size();
}

int
Table::getHeight() const
{
    return height - 1;
}

// Calculates width of a string on the screen.
static unsigned int
measureWidth(const ColorTree &s)
{
    return s.length();
}

// Calculates length of string prefix that takes up specified width on the
// screen.
static unsigned int
measurePrefixLength(const ColorTree &s, int prefixWidth)
{
    return std::min(s.length(), prefixWidth);
}
