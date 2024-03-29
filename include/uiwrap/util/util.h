//! \file

#ifndef UTIL_H_sjkdfg34nizfxn437gt7834
#define UTIL_H_sjkdfg34nizfxn437gt7834

#include "uiwrap_build_config.h"
#include "../config.h"
#include <string>

namespace mb::uiw::util {
inline const std::string& resources_base_path() {
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
    static const std::string ret{":/"};
    return ret;
#else
#error "not implemented"
#endif
}
} // namespace mb::uiw::util

UIW_HEADER_END

#endif
