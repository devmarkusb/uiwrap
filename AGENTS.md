# AGENTS.md — uiwrap

Canonical instructions for AI agents and AGENTS.md-compatible tools working in this repository (the **uiwrap** library). For **CMake layout and option naming**, treat [devmarkusb/cpp-lib-template](https://github.com/devmarkusb/cpp-lib-template) as the reference (`project(... DESCRIPTION ...)`, `MB_*_BUILD_*` options, `[CMAKE.SKIP_TESTS]` marker, optional `mb-devenv-ctest-root` when present in mb.util’s `devenv/`).

## 1. Project overview

**uiwrap** is a small C++ shared library that abstracts UI-adjacent concerns (filesystem, message boxes, program settings, Qt/QML helpers, etc.) behind implementation choices:

- **`own`** (default): console-oriented implementation; uses **Boost** (`find_package(Boost ... CONFIG)` in this repo’s `CMakeLists.txt`).
- **`qt`**: **Qt 6** (`Core`, `Gui`, `Qml`, `Widgets`); `qt_standard_project_setup(REQUIRES 6.8)` in `CMakeLists.txt`.
- **`wx`**: stub / not fully wired in CMake sources.

It links against **`mb::util`** (INTERFACE target, `mb/ul/` headers), pulled in by `cmake_util/util.cmake` via **FetchContent** from GitHub (`devmarkusb/util`) at **`main`** (shallow clone, submodules recurse). If the parent project already defines **`mb.util`**, `util.cmake` skips FetchContent (`if(TARGET mb.util)`). First configure needs network access unless the dependency is already cached.

**Boost (implementation `own`):** `find_package(Boost ${MB_UIWRAP_BOOST_MIN_VERSION} CONFIG REQUIRED)`; link **`Boost::headers`** when available. No `MB_UL_SDK_PATH` / `dev_sdk_path` layout is required for Boost if CMake can find a suitable install (e.g. system, Homebrew, or vcpkg via `CMAKE_PREFIX_PATH` as in CI).

**C++ standard:** **C++20** is required for current mb.util headers (e.g. `std::integral`, concepts). `CMakeLists.txt` sets `CMAKE_CXX_STANDARD` to 20 for this project.

This tree can be built **standalone** (open this folder as the CMake source dir) or **as a subdirectory** of a larger project (e.g. a monorepo). When embedded, follow the **parent** project’s configure flags and output layout.

## 2. Build commands

Standalone example (Qt implementation; adjust `CMAKE_PREFIX_PATH` to your Qt 6 install):

```sh
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug \
  -DMB_UIWRAP_USE_IMPLEMENTATION=qt \
  -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" \
  -DMB_UIWRAP_BUILD_TESTS=ON
cmake --build .
```

**Unverified per machine:** exact `CMAKE_PREFIX_PATH` / kit layout; see **`README.md`** for Qt **6** discovery and optional `dev_sdk_path` layout.

For the non-Qt default (`own`), ensure **Boost** is installed and visible to CMake (`CMAKE_PREFIX_PATH` or a default search path). CI installs it in **`.github/workflows/preset-test-with-boost.yml`** and **`build-and-test-with-boost.yml`** (vcpkg on Beman containers / Windows, Homebrew on macOS).

## 3. Test commands

When **`MB_UIWRAP_BUILD_TESTS`** is **ON** (see [devmarkusb/cpp-lib-template](https://github.com/devmarkusb/cpp-lib-template) for the same `MB_<LIB>_BUILD_TESTS` pattern), `uiwrapTest` is built and **`add_test(NAME uiwrap_unit_tests ...)`** registers it with CTest. Default is **`PROJECT_IS_TOP_LEVEL`** only (same as the template). When uiwrap is embedded, set **`MB_UIWRAP_BUILD_TESTS`** explicitly if you want its tests; it is **not** tied to the parent’s **`UL_BUILD_UNITTESTS`** (that flag remains for the main app’s test target in WealthPlanner). The library half of `CMakeLists.txt` ends with **`return()`** when tests are off so the test block stays a single tail section.

```sh
cd build
ctest --output-on-failure
```

CI (`.github/workflows/ci.yml`): preset and matrix jobs install **Boost** for the default `own` backend (vcpkg / Homebrew) and use **`-DMB_UIWRAP_USE_IMPLEMENTATION=qt`** where Qt matrix jobs apply; see workflow files for details.

## 4. Formatting and linting

- **Formatter:** **clang-format**, style from **`.clang-format`** at this repo root (LLVM-based; column and brace rules match the checked-in config).
- **Manual format:** `clang-format -i -style=file <path-to-files>`
- **Pre-commit:** `.githooks/pre-commit` delegates to `clangformat.sh` in the repo root. The script expects a resolved `clangformat` path (see comments in `clangformat.sh` — placeholder `CLANGFORMAT_EXE-NOTFOUND` if not customized).
- **clang-tidy:** not configured in this tree (**unverified** whether the parent `ul_*` toolchain adds it when embedded).

## 5. Architecture and important directories

| Path | Role |
|------|------|
| `include/uiwrap/` | Public headers; `config_gen.h` is **generated** from `config_gen.h.in` — do not hand-edit the generated file. |
| `src/` | Implementation; `impl_Qt/`, `impl_/`, etc. |
| `cmake_util/util.cmake` | FetchContent for **mb.util** on **`main`**; skip when **`TARGET mb.util`** already exists. |
| `CMakeLists.txt` | Library target `uiwrap`, optional `uiwrapTest`, implementation switch. |

## 6. Coding conventions

- **C++:** **C++20** for mb.util and this library; Qt 6 as required by linked modules.
- **Namespaces:** primary **`mb::uiw::`** (see `MB_UIWRAP_DISABLE_NAMESPACE_ALIAS` / `config_gen.h.in`).
- **Headers:** include guards like `#ifndef FILESYS_H_...` (not `#pragma once`) in existing style.
- **Implementation selection:** respect `UIW_LINKLIB_IMPL_CHOICE_*` / `uiwrap_build_config.h` patterns already used in `.cpp` dispatch files.
- **Qt:** prefer existing patterns in `impl_Qt/`; keep GUI-agnostic contracts in public headers where possible.

## 7. Testing expectations

- Tests use **Google Test** (`gtest_main`) and live as `*.test.cpp` next to or under the feature directories (see `CMakeLists.txt` `targetTest_SOURCES`).
- New behavior should add or extend tests under the same implementation branch (`qt` vs `own`) as in `CMakeLists.txt`.

## 8. Files and directories agents must not edit without explicit approval

- **`LICENSE`** — legal text.
- **Generated / configured outputs:** `include/uiwrap/config_gen.h` (output of `configure_file`; edit `.in` instead).
- **Upstream-only or policy-sensitive:** changing **`cmake_util/util.cmake`** FetchContent URL/tag affects reproducibility and supply chain — discuss before pinning away from `main`.
- **Do not** add secrets, signing material, or production credentials to this repo.

## 9. Security and privacy constraints

- This library deals with filesystem and settings; avoid logging paths or values that could contain user PII unless explicitly requested for debugging and scoped.
- Do not commit tokens, passwords, or machine-specific absolute paths meant for sharing.

## 10. Review checklist before final response

1. Does the change compile for the intended `MB_UIWRAP_USE_IMPLEMENTATION` (`qt` / `own`)?
2. Are Qt-specific includes and code confined to `impl_Qt/` (or existing Qt-specific test files)?
3. If tests exist for the area, were they run (`ctest` or equivalent)?
4. Does **clang-format** match `.clang-format` for touched C/C++ files?
5. If `config_gen.h.in` changed, is regenerated `config_gen.h` consistent (or left to CMake as appropriate)?
6. When embedded in a parent repo, does the change avoid breaking the parent’s CMake assumptions (`mb.util` / `mb::util`, include order, FetchContent guard)?

## Maintenance policy (stacking)

- **Canonical:** this `AGENTS.md` for uiwrap-specific facts.
- **Parent monorepo:** if the parent has its own `AGENTS.md`, treat it as authoritative for global build paths; this file applies under `sdks/uiwrap/` (or when this folder is the workspace root).
- **Global / user:** tool permissions, MCP, personal clang-format paths — stay in user or parent config, not here.
- **Task prompts:** session-only instructions; do not duplicate them into long-lived rules.
- **Cursor:** `.cursor/rules` under this subtree is useful when **this repo is the workspace root**. If you only open a **parent** monorepo in Cursor, the IDE may not load nested rules; add a thin wrapper under the parent’s `.cursor/rules/` (outside this subrepo) or use a multi-root workspace if you need automatic scoping.
