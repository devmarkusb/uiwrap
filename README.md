# uiwrap library

A library to wrap and select an underlying (G)UI implementation
of your choice.
The idea is to support:
* console/terminal
* Qt
* ... and others (e.g. wxWidgets) when needed.

The idea is to provide an abstraction layer for apps which
is GUI-agnostic.
Such that, as much app code as possible doesn't need to use
GUI implementation specific code, at least concerning the most
common tasks like message boxes, file system access, settings,
strings, type conversions, etc.

Of course, it should also make it easier to provide an additional
command-line version of certain GUI apps.

## Usage

* Set CMake variable `uiwrap_USE_IMPLEMENTATION` to either
  * "qt"
  * "" (nothing), or leave out entirely, meaning console
  * ...

* Set CMake variables UL_USE_BOOST_ver1, UL_USE_BOOST_ver2, UL_USE_BOOST_ver3
according to the Boost library version you want to use ("ver1.ver2.ver3").

## Requirements

Subsections according to `uiwrap_USE_IMPLEMENTATION`.

### qt

Seems to require
* Qt5Core, Qt5Gui, Qt5Network, Qt5Qml, Qt5Widgets

So in order to run the tests successfully from within a comprising
project, you might need to copy the two files
    qtdeploy_autodepends_bin_win_uiwraptest.bat
    qtdeploy_autodepends_bindbg_win_uiwraptest.bat
to your project's root dir.
Compiling and testing the library standalone should just work,
but this is untested.
