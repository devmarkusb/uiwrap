#!/usr/bin/env bash

set -euo pipefail

./devenv/install-boost.sh --components property-tree

git clone --depth 1 --recurse-submodules --shallow-submodules https://github.com/devmarkusb/util.git /tmp/mb-util
cmake \
  -S /tmp/mb-util \
  -B /tmp/mb-util-build \
  -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=/tmp/mb-util/devenv/cmake/fetch-content-from-lockfile.cmake \
  -DMB_DEVENV_ROOT=/tmp/mb-util \
  -DMB_UTIL_BUILD_TESTS=OFF \
  -DMB_UTIL_BUILD_EXAMPLES=OFF \
  -DCMAKE_INSTALL_PREFIX=/tmp/mb-util-install
cmake --build /tmp/mb-util-build --parallel
cmake --install /tmp/mb-util-build

echo "CMAKE_PREFIX_PATH=/tmp/mb-util-install:${CMAKE_PREFIX_PATH:-}" >> "${GITHUB_ENV}"
