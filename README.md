# uiwrap library

[![build](https://github.com/devmarkusb/uiwrap/actions/workflows/ci.yml/badge.svg)](https://github.com/devmarkusb/uiwrap/actions/workflows/ci.yml)

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

### Qt (`MB_UIWRAP_USE_IMPLEMENTATION=qt`)

**CMake** uses **Qt 6** (`find_package(Qt6 ...)` with `qt_standard_project_setup(REQUIRES 6.8)` in
`CMakeLists.txt`).

The library builds as **C++23**.

Point CMake at your Qt 6 install, for example:

```sh
export CMAKE_PREFIX_PATH=/path/to/Qt/6.9.3/gcc_64   # typical Linux layout
# or on macOS: .../Qt/6.9.3/macos
```

Optional environment layout (if your toolchain expects it):

* `export dev_sdk_path=/path-to-sdks/` where Qt lives.
* `export dev_qt_base=$dev_sdk_path/qt_linux` (or similar) with **versioned** Qt 6 directories under
  it (e.g. `6.9.3/gcc_64`), then set `CMAKE_PREFIX_PATH` to that kit’s root.

CI installs **Boost** via `setup_boost: true` (emerge in Beman Linux containers, apt on plain Linux,
cached vcpkg on Windows, Homebrew on macOS).
The **Qt** clang-tidy job uses **`qt_version: '6.9.3'`** (`jurplel/install-qt-action`, cached) with
`-DMB_UIWRAP_USE_IMPLEMENTATION=qt`.

### Console / “own” (`MB_UIWRAP_USE_IMPLEMENTATION=own`, default)

Uses the non-Qt implementations and **Boost** (`find_package(Boost ... CONFIG)`), discovered via
`CMAKE_PREFIX_PATH` or a normal Boost install layout. Locally:
`python3 devenv/install-boost.py --ensure --print-prefix-path`.

Public tuning:

* `MB_UIWRAP_BOOST_MIN_VERSION` — minimum Boost version (default `1.67`).

### CMake options (all backends)

* `MB_UIWRAP_USE_IMPLEMENTATION` — `qt`, `own` (default), or `wx` (stub). Legacy
  `-Duiwrap_USE_IMPLEMENTATION=` is still read on first configure if `MB_UIWRAP_USE_IMPLEMENTATION` is
  unset.
* `MB_UIWRAP_DISABLE_NAMESPACE_ALIAS` — see FAQ below. Legacy `-DUIW_DISABLE_NAMESPACE_ALIAS=` is
  honored if set in the cache.
* `MB_UIWRAP_BUILD_TESTS` — `ON` to build the `mb.uiwrap.test` target and register CTest tests (same
  naming idea as [cpp-lib-template](https://github.com/devmarkusb/cpp-lib-template) `MB_*_BUILD_TESTS`).
  Default is **`PROJECT_IS_TOP_LEVEL`** only; when uiwrap is **`add_subdirectory`’d**, pass
  **`-DMB_UIWRAP_BUILD_TESTS=ON`** explicitly if you want its tests (independent of a parent
  **`UL_BUILD_UNITTESTS`**).

When **uiwrap** is embedded in a larger repository, follow that parent’s CMake variables and install
paths for Qt and Boost. If the parent already defines **`mb::util`**, uiwrap uses that target.

Standalone / first-time configure uses the `devenv` FetchContent lockfile; **devmarkusb/util** is
pinned there as `mb_util`.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `uiw`
around, you can set `MB_UIWRAP_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::uiw` instead.
