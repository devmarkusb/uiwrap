# AGENTS.md — uiwrap

Canonical instructions for AI agents and AGENTS.md-compatible tools working in this repository (the **uiwrap** library).

## 1. Project overview

**uiwrap** is a small C++ shared library that abstracts UI-adjacent concerns (filesystem, message boxes, program settings, Qt/QML helpers, etc.) behind implementation choices:

- **`own`** (default): console-oriented implementation; uses **Boost** (via parent CMake defaults).
- **`qt`**: **Qt 6** (`Core`, `Gui`, `Qml`, `Widgets`); `qt_standard_project_setup(REQUIRES 6.8)` in `CMakeLists.txt`.
- **`wx`**: stub / not fully wired in CMake sources.

It links against legacy **`mb-util`** (INTERFACE target, `ul/` headers), pulled in by `cmake_util/util.cmake` via **FetchContent** from GitHub (`devmarkusb/util`) at a **pinned commit** (`MB_UTIL_FETCH_TAG`, default matches branch `last-old-build-on-mac`). The current **`main`** branch of that repo is the **mb.util** rewrite and is **not** CMake/C++ compatible with this tree. Override the tag only when you know the revision matches this API. If the parent project already defines **`mb-util`**, `util.cmake` skips FetchContent (guard: **`TARGET mb-util`**, not `ulBuildEnv`). First configure needs network access unless the dependency is already cached.

This tree can be built **standalone** (open this folder as the CMake source dir) or **as a subdirectory** of a larger project (e.g. a monorepo). When embedded, follow the **parent** project’s configure flags and output layout.

## 2. Build commands

Standalone example (Qt implementation; adjust `CMAKE_PREFIX_PATH` to your Qt 6 install):

```sh
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug \
  -Duiwrap_USE_IMPLEMENTATION=qt \
  -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" \
  -DUL_BUILD_UNITTESTS=ON
cmake --build .
```

**Unverified per machine:** exact `CMAKE_PREFIX_PATH` / kit layout; see **`README.md`** for Qt **6** discovery and optional `dev_sdk_path` layout.

For the non-Qt default (`own`), CMake includes Boost via `ul_find_boost(...)`; versions are defined by the fetched **mb-util** / `ul_*` defaults, not duplicated here.

## 3. Test commands

Tests are registered with CTest only if **`UL_BUILD_UNITTESTS`** is **not** `OFF` (otherwise `CMakeLists.txt` returns before defining `uiwrapTest`).

```sh
cd build
ctest --output-on-failure
```

CI (`.github/workflows/build.yml`): **ubuntu-latest**, **Qt 6.9.3** via `jurplel/install-qt-action@v4` (desktop, `qt5compat` module), CMake with `-Duiwrap_USE_IMPLEMENTATION=qt` and `-DUL_BUILD_UNITTESTS=ON`, then `ctest -C <Debug|RelWithDebInfo> --output-on-failure` from the build directory (see the workflow file for the exact one-liner).

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
| `cmake_util/util.cmake` | FetchContent for legacy **mb-util**; early return when **`TARGET mb-util`** already exists. Pin `MB_UTIL_FETCH_TAG` vs util `main` (incompatible). |
| `CMakeLists.txt` | Library target `uiwrap`, optional `uiwrapTest`, implementation switch. |

## 6. Coding conventions

- **C++:** C++17 or as required by linked Qt 6 / mb-util (follow existing code).
- **Namespaces:** primary **`mb::uiw::`** (see `UIW_DISABLE_NAMESPACE_ALIAS` / `config_gen.h.in`).
- **Headers:** include guards like `#ifndef FILESYS_H_...` (not `#pragma once`) in existing style.
- **Implementation selection:** respect `UIW_LINKLIB_IMPL_CHOICE_*` / `uiwrap_build_config.h` patterns already used in `.cpp` dispatch files.
- **Qt:** prefer existing patterns in `impl_Qt/`; keep GUI-agnostic contracts in public headers where possible.

## 7. Testing expectations

- Tests use **Google Test** (`gtest_main`) and live as `*.test.cpp` next to or under the feature directories (see `CMakeLists.txt` `targetTest_SOURCES`).
- New behavior should add or extend tests under the same implementation branch (`qt` vs `own`) as in `CMakeLists.txt`.

## 8. Files and directories agents must not edit without explicit approval

- **`LICENSE`** — legal text.
- **Generated / configured outputs:** `include/uiwrap/config_gen.h` (output of `configure_file`; edit `.in` instead).
- **Upstream-only or policy-sensitive:** changing **`cmake_util/util.cmake`** FetchContent pins (URL/tag) affects reproducibility and supply chain — discuss before changing.
- **Do not** add secrets, signing material, or production credentials to this repo.

## 9. Security and privacy constraints

- This library deals with filesystem and settings; avoid logging paths or values that could contain user PII unless explicitly requested for debugging and scoped.
- Do not commit tokens, passwords, or machine-specific absolute paths meant for sharing.

## 10. Review checklist before final response

1. Does the change compile for the intended `uiwrap_USE_IMPLEMENTATION` (`qt` / `own`)?
2. Are Qt-specific includes and code confined to `impl_Qt/` (or existing Qt-specific test files)?
3. If tests exist for the area, were they run (`ctest` or equivalent)?
4. Does **clang-format** match `.clang-format` for touched C/C++ files?
5. If `config_gen.h.in` changed, is regenerated `config_gen.h` consistent (or left to CMake as appropriate)?
6. When embedded in a parent repo, does the change avoid breaking the parent’s CMake assumptions (`mb-util` / include order, `ulBuildEnv`)?

## Maintenance policy (stacking)

- **Canonical:** this `AGENTS.md` for uiwrap-specific facts.
- **Parent monorepo:** if the parent has its own `AGENTS.md`, treat it as authoritative for global build paths; this file applies under `sdks/uiwrap/` (or when this folder is the workspace root).
- **Global / user:** tool permissions, MCP, personal clang-format paths — stay in user or parent config, not here.
- **Task prompts:** session-only instructions; do not duplicate them into long-lived rules.
- **Cursor:** `.cursor/rules` under this subtree is useful when **this repo is the workspace root**. If you only open a **parent** monorepo in Cursor, the IDE may not load nested rules; add a thin wrapper under the parent’s `.cursor/rules/` (outside this subrepo) or use a multi-root workspace if you need automatic scoping.
