# uiwrap library

A library to wrap and select an underlying (G)UI implementation
of your choice.
We would like to support:
* console/terminal
* Qt
* ... and others (e.g. wxWidgets) when needed.

The idea is to provide at least to some extent an abstraction layer
for apps which is GUI-agnostic.
Such that, as much app code as possible doesn't need to use
GUI implementation specific code, at least concerning the most
common tasks like message boxes, file system access, settings,
strings, type conversions, etc.

Of course, it should also make it easier to provide an additional
command-line version of certain GUI apps.

## Usage

Set environment variables:
* `export dev_sdk_path=/path-to-sdks/` where versions of Qt reside
(and possibly also boost).
* `export dev_qt_base=$dev_sdk_path/qt_linux` (e.g.) where Qt versions
can be found in subdirs like `5.9.1`. 

Set the following CMake variables:
* `uiwrap_USE_IMPLEMENTATION` to either
  * `qt`
  * `own`, or leave out entirely, meaning console through an
'own' implementation
  * ...

* `UL_USE_BOOST_ver1, UL_USE_BOOST_ver2, UL_USE_BOOST_ver3`
according to the Boost library version you want to use ("ver1.ver2.ver3").
* `UL_QT5_VERSION` to e.g. `5.9.1`
* `UL_QT_COMPILER_SUBDIR` to e.g. `gcc_64`

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `uiw`
around, you can set `UIW_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::uiw` instead.
