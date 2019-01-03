// Markus Borris, 2019
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef UTIL_H_sjkdfg34nizfxn437gt7834
#define UTIL_H_sjkdfg34nizfxn437gt7834

#include "uiwrap_build_config.h"
#include <string>


namespace uiw
{
namespace util
{
inline std::string resources_base_path()
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
    return "qrc:/";
#else
#error "not implemented"
#endif
}
} // util
} // uiw

#endif

