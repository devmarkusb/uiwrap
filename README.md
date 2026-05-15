# uiwrap library

[![build](https://github.com/devmarkusb/uiwrap/actions/workflows/build.yml/badge.svg)](https://github.com/devmarkusb/uiwrap/actions/workflows/build.yml)

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

### Qt (`uiwrap_USE_IMPLEMENTATION=qt`)

**CMake** uses **Qt 6** (`find_package(Qt6 ...)` with `qt_standard_project_setup(REQUIRES 6.8)` in `CMakeLists.txt`).

Point CMake at your Qt 6 install, for example:

```sh
export CMAKE_PREFIX_PATH=/path/to/Qt/6.9.3/gcc_64   # typical Linux layout
# or on macOS: .../Qt/6.9.3/macos
```

Optional environment layout (if your toolchain expects it):

* `export dev_sdk_path=/path-to-sdks/` where Qt (and possibly Boost) live.
* `export dev_qt_base=$dev_sdk_path/qt_linux` (or similar) with **versioned** Qt 6 directories under it (e.g. `6.9.3/gcc_64`), then set `CMAKE_PREFIX_PATH` to that kit’s root.

CI (`.github/workflows/build.yml`) installs **Qt 6.9.3** via `jurplel/install-qt-action@v4` (desktop, `qt5compat` module) and configures with `-Duiwrap_USE_IMPLEMENTATION=qt`.

### Console / “own” (`uiwrap_USE_IMPLEMENTATION=own`, default)

Uses the non-Qt implementations and **Boost** via the shared CMake helpers from **mb-util** (`cmake_util/util.cmake`).

Set the following CMake variables when using the `own` backend (versions come from the fetched **mb-util** defaults unless you override):

* `UL_USE_BOOST_ver1`, `UL_USE_BOOST_ver2`, `UL_USE_BOOST_ver3` — Boost version triple (`"ver1.ver2.ver3"`).

### CMake options (all backends)

* `uiwrap_USE_IMPLEMENTATION` — `qt`, `own` (default), or `wx` (stub).
* `UIW_DISABLE_NAMESPACE_ALIAS` — see FAQ below.
* `UL_BUILD_UNITTESTS` — set to `ON` to build the `uiwrapTest` target and register CTest tests.

When **uiwrap** is embedded in a larger repository, follow that parent’s CMake variables and install paths for Qt and Boost. The parent should create the **`mb-util`** target (legacy util) **before** adding uiwrap, so `cmake_util/util.cmake` does not FetchContent a second copy.

Standalone clones pull **devmarkusb/util** at a **pinned git ref** (`MB_UTIL_FETCH_TAG` in `cmake_util/util.cmake`). That ref tracks the legacy layout compatible with `ul::` / `mb-util`; the **`main`** branch of util is a different library (**mb.util**) and will not configure uiwrap as-is.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `uiw`
around, you can set `UIW_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::uiw` instead.
