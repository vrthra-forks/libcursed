**libcursed**, _2019_

_This file last updated on 19 March, 2019_

### Brief Description ###

This is a C++11 library that provides classes for using
[*curses* library][curses] which don't just copy original C API.

#### Goals ####

This is not a wrapping library, e.g. you won't find much correspondence between
what *libcursed* and *curses* provide.  Instead the goal is to make it easier to
write more or less typical applications with text user interface without
flickering, the headache of manual handling of screen resizing or computing
positions of elements on the screen.

Therefore the library concentrates mainly on functionality for drawing and does
not attempt to implement some kind of framework.  It doesn't enforce much
structure on your application and thus shouldn't be disruptive to code bases.
There are means for reading input, however it's completely up to the client to
handle it and request the library to update the graphics (including handling of
terminal resize events, which count as input).

On the implementation side the library should never pull in `<curses.h>` via its
headers nor clients should have to include it in order to use the library.
`<curses.h>` uses some very generic names, some of which are macros,
consequently they can cause conflicts in completely unrelated code.

#### Implementation state ####

It was created for a couple of projects, has minimally necessary functionality
and can change quite a bit in the future.

#### Throw-in library state ####

There is no build system and nothing is getting built for the client.  To use
it clone the repository (possibly as a submodule) and handle the building with
the build system that's used by the main project.  Compile with C++11 enabled
and link against `cursesw`.

### Prerequisites ###

* C++11 capable compiler
* [*curses* library][curses] with support of wide characters

### Structure ###

The realm of the library is structuring displayed information by automatically
arranging it on the screen as widgets.  The client's responsibility is to
construct the widgets and fill them in specifying formatting in the process.
After that the library will do what it's supposed to do on calls to draw and
resize methods.

The client is expected to manage lifetime of objects, the library accepts
pointers and uses the objects assuming that they are valid.  This doesn't seem
to be a burden as having access to all widgets is needed to update their
contents anyway.

There are three main structural elements:

1. Widgets
2. Layers
3. Formatted strings

#### Widgets ####

| Name     | Description
|----------|-------------
| Expander | non-drawing widget that takes up as much space as possible
| Label    | static text field
| List     | list of items
| Prompt   | text field that displays cursor
| Text     | static text area
| Track    | container that organizes widgets vertically

#### Layers ####

Widgets can't be drawn at client's will, instead they need to be organized in a
hierarchy and root of the hierarchy should be passed to the `Screen` class.

Multiple roots can be active at the same time forming layers (the same `Screen`
class manages them) which can be drawn one on top of the other.  This allows
adding elements to the screen while keeping most of what is already there
visible as a background.

#### Formatted strings ####

The library works with wide strings and provides corresponding utility
conversion functions (`toNarrow()` and `toWide()`).  `ColorTree` and `Format`
classes are basically responsible for handling all of the formatting.

`ColorTree` is a type used in place of plain strings in *libcursed*.  Instead of
specifying format separately, clients need to build an instance of this type
that describes formatting as a hierarchy of nested formatted substrings where
final format of a character is formed by merging all formats it's nested within.

Usage example:

```cxx
cursed::Format fmt;
fmt.setBold(true);
cursed::Format totalFmt;
totalFmt.setReversed(true);

cursed::ColorTree tree = totalFmt(fmt(L"first") + L"middle" + fmt(L"last"));
tree += L"trailing";
```

Displayed substrings will have the following formatting:

| Substring  | Format
|------------|-------------
| `first`    | bold and reversed
| `middle`   | reversed
| `last`     | bold and reversed
| `trailing` | no formatting

`Format` class is also used separately for specifying backgrounds of widgets
that display content (i.e. not `Expander` or `Track`).

#### Object management ####

Use of the library requires creation of `Init` object before the use of any
other classes and destroying it at the end of use.

Any number of `Screen` and `Input` objects can be created and used at the same
time.

#### API ####

The API consists of classes in `cursed` namespace.  Things in `cursed::guts`
namespace as well as files in `guts/` subdirectory are implementation details.

### Sample ###

Very minimal application:

```cxx
#include "cursed/Init.hpp"
#include "cursed/Input.hpp"
#include "cursed/Label.hpp"
#include "cursed/List.hpp"
#include "cursed/Screen.hpp"
#include "cursed/Track.hpp"

int
main()
{
    cursed::Init init;

    cursed::Label title;
    title.setText(L"libcursed sample");

    cursed::List list;
    list.setItems({
        L"j -- move down",
        L"k -- move up",
        L"q -- quit"
    });

    cursed::Track track;
    track.addItem(&title);
    track.addItem(&list);

    cursed::Screen screen;
    screen.replaceTopWidget(&track);
    screen.draw();

    cursed::Input input(cursed::Keypad::Enabled);
    while (cursed::InputElement ie = input.read()) {
        if (ie.isTerminalResize()) {
            screen.resize();
            screen.draw();
            continue;
        }

        if (!ie.isFunctionalKey()) {
            switch (ie) {
                case L'j': list.moveDown(); break;
                case L'k': list.moveUp(); break;
                case L'q': return 0;
            };
        }

        screen.draw();
    }
    return 0;
}
```

### Alternatives ###

* C++ bindings shipped with [ncurses][ncurses].  It is suffering while using
  these what prompted creation of *libcursed*.  While complete, the quality of
  API implementation is low, which makes it very painful to use (needs way to
  much time to work around its rather unexpected issues).  See
  [this post][cursesxx-post].
* [NDK++][ndkxx] with its two kinds of APIs.  Seems abandoned, but probably
  still works.  Unlike *libcursed* has lots of UI elements.
* [cursespp][cursespp] is a more recent library that seems to be quite high
  level and sort of framework-like.

[curses]: https://en.wikipedia.org/wiki/Curses_(programming_library)
[ncurses]: https://invisible-island.net/ncurses/
[cursesxx-post]: https://reversed.top/2016-04-24/ncurses-for-cpp/
[ndkxx]: http://ndk-xx.sourceforge.net/
[cursespp]: https://github.com/clangen/cursespp
