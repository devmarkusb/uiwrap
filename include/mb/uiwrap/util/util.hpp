//! \file

#ifndef UTIL_H_sjkdfg34nizfxn437gt7834
#define UTIL_H_sjkdfg34nizfxn437gt7834

#include "../config.hpp"
#include "mb/uiwrap/uiwrap_build_config.hpp"
#include <string>

namespace mb::uiw::util {
inline const std::string& resources_base_path() {
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
    static const std::string ret{":/"};
    return ret;
#else
    static const std::string ret{};
    return ret;
#endif
}
} // namespace mb::uiw::util

UIW_HEADER_END

#endif
